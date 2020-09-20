#include <QApplication>

#include "mainwindow.h"
#include "databasewrapper.h"


class ThreadWrapper
{
private:
    std::thread &_th;
public:
    ThreadWrapper(std::thread &refTh);
    ~ThreadWrapper();
};

ThreadWrapper::ThreadWrapper(std::thread &refTh) : _th(refTh) {}
ThreadWrapper::~ThreadWrapper()
{
    if(_th.joinable())
    {
        _th.join();
    }
}

void fill_tables(DatabaseWrapper &database)
{
    std::string sqlCommand = "INSERT INTO people(name, age, gender, avatar) " \
                             "VALUES('Chewbakka', 225, 'M', 'images/chewbakka.jpg')";
    database.pushCommand(sqlCommand);

    sqlCommand = "INSERT INTO people(name, age, gender, avatar) " \
                 "VALUES('Han Solo', 49, 'M', 'images/han_solo.jpg')";
    database.pushCommand(sqlCommand);

    sqlCommand = "INSERT INTO people(name, age, gender, avatar) " \
                 "VALUES('Ben Kenobi', 57, 'M', 'images/ben_kenobi.jpg')";
    database.pushCommand(sqlCommand);

    sqlCommand = "INSERT INTO people(name, age, gender, avatar) " \
                 "VALUES('Luke Skywalker', 39, 'M', 'images/luke_skywalker.jpg')";
    database.pushCommand(sqlCommand);

    sqlCommand = "INSERT INTO people(name, age, gender, avatar) " \
                 "VALUES('Leia Organa', 39, 'F', 'images/leia_organa.jpg')";
    database.pushCommand(sqlCommand);

    sqlCommand = "INSERT INTO stuff(name_stuff, id_category, icon) "\
                 "VALUES('Lightsaber', 1, 'images/lightsaber.jpg')";
    database.pushCommand(sqlCommand);

    sqlCommand = "INSERT INTO stuff(name_stuff, id_category, icon) "\
                     "VALUES('Crossbow', 2, 'images/crossbow.jpg')";
    database.pushCommand(sqlCommand);

    sqlCommand = "INSERT INTO stuff_category(category_name, icon) "\
                 "VALUES('Melee weapon', 'images/melee.jpg')";
    database.pushCommand(sqlCommand);

    sqlCommand = "INSERT INTO stuff_category(category_name, icon) "\
                 "VALUES('Range weapon', 'images/range.jpg')";
    database.pushCommand(sqlCommand);

    /*sqlCommand = "INSERT INTO journal(type_table, money, people_id, stuff_id, record_time, comment) "\
                 "VALUES('source_consumption', 3000, 1, 2, '2020-09-15', 'CrowssBOW!')";
    database.pushCommand(sqlCommand);

    sqlCommand = "INSERT INTO journal(type_table, money, people_id, stuff_id, record_time, comment) "\
                 "VALUES('source_consumption', 5000, 2, 2, '2020-09-16', 'New CrowssBOW!')";
    database.pushCommand(sqlCommand);

    sqlCommand = "INSERT INTO journal(type_table, money, people_id, stuff_id, record_time, comment) "\
                 "VALUES('source_consumption', 25000, 4, 1, '2020-09-14', 'LigthSaber')";
    database.pushCommand(sqlCommand);*/
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseWrapper database;
    MainWindow w(database);

    std::thread databaseThread(&DatabaseWrapper::startExecution, &database, 500);
    ThreadWrapper thWrapper(databaseThread);

    w.show();

    return a.exec();
}
