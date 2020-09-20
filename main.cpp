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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseWrapper database;
    MainWindow w(database);

    std::thread databaseThread(&DatabaseWrapper::startExecution, &database, 500);
    ThreadWrapper thWrapper(databaseThread);

    std::string sqlCommand = "INSERT INTO people(name, age, gender, avatar) " \
                             "VALUES('Alex', 22, 'M', '/home/alex/avata.jpg')";

    database.pushCommand(sqlCommand);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    CallbackResult result = database.customSelect(PEOPLE_TABLE, "name", "Alex");

    w.show();

    return a.exec();
}
