#ifndef RECORDTABLEMANIPULATOR_H
#define RECORDTABLEMANIPULATOR_H

#include <memory>
#include <map>

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QFormLayout>

#include "databasewrapper.h"

class RecordTableManipulator : public QWidget
{
    Q_OBJECT
public:
    enum typeManipulate { addRecord, editRecord };
private:
    DatabaseWrapper &_database;
    typeManipulate _manipulationTableType;
private:
    std::shared_ptr<QHBoxLayout> _mainLayout;
    std::shared_ptr<QFormLayout> _formLayout;

    std::shared_ptr<QLabel> _idLabel;
    std::shared_ptr<QLabel> _typeTableLabel;
    std::shared_ptr<QLabel> _moneyLabel;
    std::shared_ptr<QLabel> _peopleIdLabel;
    std::shared_ptr<QLabel> _dateLabel;
    std::shared_ptr<QLabel> _commentLabel;

    std::shared_ptr<QLineEdit> _idEdit;
    std::shared_ptr<QLineEdit> _typeTableEdit;
    std::shared_ptr<QLineEdit> _moneyEdit;
    std::shared_ptr<QLineEdit> _peopleIdEdit;
    std::shared_ptr<QLineEdit> _dateEdit;
    std::shared_ptr<QPlainTextEdit> _commentEdit;

    std::shared_ptr<QPushButton> _sendRequestBut;
private:
    void initializeElements();
    void sendToOtherTables(std::map<std::string, std::string> tableSqlCommandMap);
    void editRecordInTable();
    void addRecordIntoTable();
private slots:
    void sendRequest();
public:
    explicit RecordTableManipulator(DatabaseWrapper &refDatabase, typeManipulate typeM, QWidget *parent = nullptr);
    ~RecordTableManipulator();
};

#endif // RECORDTABLEMANIPULATOR_H
