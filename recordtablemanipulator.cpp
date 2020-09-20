#include "recordtablemanipulator.h"

RecordTableManipulator::RecordTableManipulator(DatabaseWrapper &refDatabase, typeManipulate typeM, QWidget *parent) :
    QWidget(parent), _database(refDatabase), _manipulationTableType(typeM)
{
    this->setFixedSize(400, 300);

    initializeElements();
}

RecordTableManipulator::~RecordTableManipulator()
{

}

void RecordTableManipulator::initializeElements()
{
    _mainLayout = std::make_shared<QHBoxLayout>();
    _formLayout = std::make_shared<QFormLayout>();

    if(_manipulationTableType == addRecord)
    {
        this->setWindowTitle("Adding record");
    }
    else
    {
        this->setWindowTitle("Editing record");
        _idLabel = std::make_shared<QLabel>("Id: ");
        _idEdit = std::make_shared<QLineEdit>();

        _formLayout->addRow(_idLabel.get(), _idEdit.get());
    }

    _typeTableLabel = std::make_shared<QLabel>("Table: ");
    _typeTableEdit = std::make_shared<QLineEdit>();
    _formLayout->addRow(_typeTableLabel.get(), _typeTableEdit.get());

    _moneyLabel = std::make_shared<QLabel>("Money: ");
    _moneyEdit = std::make_shared<QLineEdit>();
    _formLayout->addRow(_moneyLabel.get(), _moneyEdit.get());

    _peopleIdLabel = std::make_shared<QLabel>("People id: ");
    _peopleIdEdit = std::make_shared<QLineEdit>();
    _formLayout->addRow(_peopleIdLabel.get(), _peopleIdEdit.get());

    _dateLabel = std::make_shared<QLabel>("Date: ");
    _dateEdit = std::make_shared<QLineEdit>();
    _formLayout->addRow(_dateLabel.get(), _dateEdit.get());

    _commentLabel = std::make_shared<QLabel>("Comment: ");
    _commentEdit = std::make_shared<QPlainTextEdit>();
    _formLayout->addRow(_commentLabel.get(), _commentEdit.get());

    _sendRequestBut = std::make_shared<QPushButton>("Send");

    _mainLayout->addLayout(_formLayout.get());
    _mainLayout->addWidget(_sendRequestBut.get());

    this->setLayout(_mainLayout.get());
}

void RecordTableManipulator::sendToOtherTables(std::map<std::string, std::string> tableSqlCommandMap)
{

}

void RecordTableManipulator::editRecordInTable()
{

}
void RecordTableManipulator::addRecordIntoTable()
{

}

void RecordTableManipulator::sendRequest()
{

}
