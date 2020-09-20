#include "databasewrapper.h"

static int show_callback(void *data, int argc, char **argv, char **azColName)
{
    CallbackResult *resultPtr = static_cast<CallbackResult*>(data);
    OneRecord recordVal;

    for(int i = 0; i < argc; i++)
    {
        std::string colName(azColName[i]);
        std::string colValue(argv[i] ? argv[i] : "NULL");
        recordVal.results.push_back(std::pair<std::string, std::string>(colName, colValue));
    }

    resultPtr->records.push_back(recordVal);

    return 0;
}

/*---------------------------------------------------------------------------------*/
/*---------------------------------DATABASE_WRAPPPER-------------------------------*/
/*---------------------------------------------------------------------------------*/
DatabaseWrapper::DatabaseWrapper() : _isNeedProcess(true)
{
    int res = sqlite3_open(DB_NAME, &_db);
    if(res)
    {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(_db) << std::endl;
        std::terminate();
    }

    initTables();
}

DatabaseWrapper::~DatabaseWrapper()
{
    int res = 0;
    while((res = sqlite3_close(_db)) == SQLITE_BUSY)
    {
        std::cerr << "Not all operations done yet, waiting for 1 second" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void DatabaseWrapper::initTables()
{
    int res = 0;
    std::string sqlQuery;
    sqlite3_stmt *stmt = nullptr;
    std::vector<std::string> tables = { PEOPLE_TABLE, CATEGORY_TABLE, STUFF_TABLE,
                                        INCOME_TABLE, CONSUMPTION_TABLE, JOURNAL_TABLE };

    for(size_t i(0); i < tables.size(); i++)
    {
        /*Здесь идет проверка на существование таблицы.
          Если таблица не сущетсвует, то создаем ее*/

        sqlQuery = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + tables[i] + "';";

        res = sqlite3_prepare_v2(_db, sqlQuery.c_str(), static_cast<int>(sqlQuery.size() + 1), &stmt, nullptr);
        res = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if(res == SQLITE_DONE)
        {
            try
            {
                TableCreator::createTable(tables[i], _db);
            }
            catch(std::exception const &ex)
            {
                std::cerr << "Cannot create table: " << tables[i] << std::endl;
                std::cerr << "Msg: " << ex.what() << std::endl;
                std::terminate();
            }
        }
    }
}

void DatabaseWrapper::executeCommand(int period)
{
    bool isReadyNewCommand = false;
    while(_isNeedProcess.load())
    {
        std::unique_lock<std::mutex> processLc(_databaseWrapperMutex);
        isReadyNewCommand = _executeSyncCond.wait_for(processLc, std::chrono::milliseconds(period),
                                                      [this]() -> bool { return !_listCommands.empty(); });

        if(isReadyNewCommand == false)
        {
            continue;
        }
        else
        {
            std::string sqlCommand = _listCommands.front();
            _listCommands.pop_front();

            char *errMsg = nullptr;
            int res = sqlite3_exec(_db, sqlCommand.c_str(), nullptr, nullptr, &errMsg);
            processLc.unlock();

            if(res != SQLITE_OK)
            {
                std::cerr << "SQL error: " << errMsg << std::endl;
                sqlite3_free(errMsg);
            }
        }
    }
}

CallbackResult DatabaseWrapper::selectRequest(std::string selectCommand)
{
    CallbackResult resutStruct;
    char *errMsg = nullptr;

    _databaseWrapperMutex.lock();
    int res = sqlite3_exec(_db, selectCommand.c_str(), show_callback, static_cast<void*>(&resutStruct), &errMsg);
    _databaseWrapperMutex.unlock();

    if(res != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        resutStruct.errMsg = std::string(errMsg) + std::string("\n");
    }

    return resutStruct;
}

void DatabaseWrapper::pushCommand(std::string sqlCommand)
{
    std::lock_guard<std::mutex> pushGuard(_databaseWrapperMutex);
    _listCommands.push_back(sqlCommand);
    _executeSyncCond.notify_one();
}

CallbackResult DatabaseWrapper::showTable(std::string tablename)
{
    std::string sqlRequest = "SELECT * FROM " + tablename;
    return selectRequest(sqlRequest);
}

CallbackResult DatabaseWrapper::customSelect(std::string tablename, std::string key, std::string value)
{
    std::string sqlCommand = "SELECT * FROM " + tablename + " WHERE " + key + "='" + value + "'";
    return selectRequest(sqlCommand);
}

void DatabaseWrapper::stopExecution()
{
    _isNeedProcess.store(false);
}

void DatabaseWrapper::startExecution(int period)
{
    executeCommand(period);
}

/*------------------------------------------------------------------------------*/
/*--------------------------------TABLE_CREATOR---------------------------------*/
/*------------------------------------------------------------------------------*/
void TableCreator::createTable(std::string tableName, sqlite3 *dbPtr)
{
    std::string createSqlCommand;

    if(tableName == PEOPLE_TABLE)
    {
        createSqlCommand = getPeopleTableCreateCommand();
    }
    else if(tableName == CATEGORY_TABLE)
    {
        createSqlCommand = getCategoryTableCreateCommand();
    }
    else if(tableName == STUFF_TABLE)
    {
        createSqlCommand = getStuffTableCreateCommand();
    }
    else if(tableName == INCOME_TABLE)
    {
        createSqlCommand = getIncomeTableCreateCommand();
    }
    else if(tableName == CONSUMPTION_TABLE)
    {
        createSqlCommand = getConsumptionTableCreateCommand();
    }
    else if(tableName == JOURNAL_TABLE)
    {
        createSqlCommand = getJournalTableCreateCommand();
    }
    else
    {
        throw std::runtime_error("Tablename incorrect");
    }

    char *errMsg = nullptr;

    int res = sqlite3_exec(dbPtr, createSqlCommand.c_str(), nullptr, nullptr, &errMsg);
    if(res != SQLITE_OK)
    {
        std::cerr << "Create table error: " << errMsg << std::endl;
        std::cerr << "Tablename: " << tableName << std::endl;
        sqlite3_free(errMsg);
        std::terminate();
    }
}

std::string TableCreator::getPeopleTableCreateCommand()
{
    std::string sqlCommand = "CREATE TABLE people(id INTEGER PRIMARY KEY NOT NULL, " \
                             "name TEXT, age INTEGER, gender CHAR(1), avatar CHAR(100))";
    return sqlCommand;
}

std::string TableCreator::getCategoryTableCreateCommand()
{
    std::string sqlCommand = "CREATE TABLE stuff_category(id INTEGER PRIMARY KEY NOT NULL, " \
                             "category_name TEXT, icon CHAR(100))";
    return sqlCommand;
}

std::string TableCreator::getStuffTableCreateCommand()
{
    std::string sqlCommand = "CREATE TABLE stuff(id INTEGER PRIMARY KEY NOT NULL, " \
                             "name_stuff TEXT, id_category INTEGER, icon CHAR(100))";
    return sqlCommand;
}

std::string TableCreator::getIncomeTableCreateCommand()
{
    std::string sqlCommand = "CREATE TABLE source_income(id INTEGER PRIMARY KEY NOT NULL, " \
                             "source TEXT, people_id INTEGER)";
    return sqlCommand;
}

std::string TableCreator::getConsumptionTableCreateCommand()
{
    std::string sqlCommand = "CREATE TABLE source_consumption(id INTEGER PRIMARY KEY NOT NULL, " \
                             "stuff_id INTEGER, people_id INTEGER)";
    return sqlCommand;
}

std::string TableCreator::getJournalTableCreateCommand()
{
    std::string sqlCommand = "CREATE TABLE journal(id INTEGER PRIMARY KEY NOT NULL, " \
                             "type_table CHAR(15), money INTEGER, people_id INTEGER, " \
                             "record_time DATETIME, comment TEXT)";
    return sqlCommand;
}
