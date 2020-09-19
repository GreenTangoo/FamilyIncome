#ifndef DATABASEWRAPPER_H
#define DATABASEWRAPPER_H

#include <iostream>
#include <exception>
#include <list>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include <cstdio>
#include <sqlite3.h>

#define DB_NAME "family.sqlite3"

#define PEOPLE_TABLE "people"
#define CATEGORY_TABLE "stuff_category"
#define STUFF_TABLE "stuff"
#define INCOME_TABLE "source_income"
#define CONSUMPTION_TABLE "source_consumption"
#define JOURNAL_TABLE "journal"

struct CallbackResult
{
    std::vector<std::pair<std::string, std::string>> results;
};

class TableCreator
{
private:
    static std::string getPeopleTableCreateCommand();
    static std::string getCategoryTableCreateCommand();
    static std::string getStuffTableCreateCommand();
    static std::string getIncomeTableCreateCommand();
    static std::string getConsumptionTableCreateCommand();
    static std::string getJournalTableCreateCommand();
public:
    static void createTable(std::string tableName, sqlite3 *dbPtr);
};

class DatabaseWrapper
{
private:
    sqlite3 *_db;
    std::list<std::string> _listCommands;
    std::condition_variable _executeSyncCond;
    std::mutex _databaseWrapperMutex;
    std::atomic<bool> _isNeedProcess;
private:
    void initTables();
    void executeCommand();
public:
    DatabaseWrapper();
    ~DatabaseWrapper();
    void pushCommand(std::string sqlCommand);
    CallbackResult showTable(std::string tablename);
    void stopExecution();
    void startExecution();
};

#endif // DATABASEWRAPPER_H
