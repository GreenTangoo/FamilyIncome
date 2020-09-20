#include "recordtablemanipulator.h"

RecordTableManipulator::RecordTableManipulator(DatabaseWrapper &refDatabase, typeManipulate typeM, QWidget *parent) :
    QWidget(parent), _database(refDatabase), _manipulationTableType(typeM)
{
    this->setFixedSize(450, 400);

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

    _stuffIdLabel = std::make_shared<QLabel>("Stuff id: ");
    _stuffIdEdit = std::make_shared<QLineEdit>();
    _formLayout->addRow(_stuffIdLabel.get(), _stuffIdEdit.get());

    _dateLabel = std::make_shared<QLabel>("Date: ");
    _dateEdit = std::make_shared<QLineEdit>();
    _formLayout->addRow(_dateLabel.get(), _dateEdit.get());

    _commentLabel = std::make_shared<QLabel>("Comment: ");
    _commentEdit = std::make_shared<QPlainTextEdit>();
    _formLayout->addRow(_commentLabel.get(), _commentEdit.get());

    _sendRequestBut = std::make_shared<QPushButton>("Send");
    connect(_sendRequestBut.get(), SIGNAL(clicked()), this, SLOT(sendRequest()));

    _mainLayout->addLayout(_formLayout.get());
    _mainLayout->addWidget(_sendRequestBut.get());

    this->setLayout(_mainLayout.get());
}

void RecordTableManipulator::sendToTables(std::vector<std::string> const &sqlCommandsVec)
{
    for(size_t i(0); i < sqlCommandsVec.size(); i++)
    {
        _database.pushCommand(sqlCommandsVec[i]);
    }
}

void RecordTableManipulator::editRecordInTable()
{
    std::string idStr = _idEdit->text().toStdString();
    std::string typeTableStr = _typeTableEdit->text().toStdString();
    std::string moneyStr = _moneyEdit->text().toStdString();
    std::string peopleIdStr = _peopleIdEdit->text().toStdString();
    std::string stuffIdStr = _stuffIdEdit->text().toStdString();
    std::string dateStr = _dateEdit->text().toStdString();
    std::string commentStr = _commentEdit->toPlainText().toStdString();

    std::string sqlCommand = "UPDATE " + std::string(JOURNAL_TABLE) +
            " SET type_table='" + typeTableStr + "', money=" + moneyStr +
            ", people_id=" + peopleIdStr + ", stuff_id=" + stuffIdStr +
            ", record_time='" + dateStr + "', comment='" + commentStr +
            "' WHERE id=" + idStr;

    if((typeTableStr != INCOME_TABLE) && (typeTableStr != CONSUMPTION_TABLE))
    {
        QMessageBox::critical(this, "Error", "Неверно указано имя таблицы");
        return;
    }

    std::vector<std::string> commands = {sqlCommand};
    sendToTables(commands);
    QMessageBox::information(this, "Done", "Запись успешно обновлена");
}
void RecordTableManipulator::addRecordIntoTable()
{
    std::string typeTableStr = _typeTableEdit->text().toStdString();
    std::string moneyStr = _moneyEdit->text().toStdString();
    std::string peopleIdStr = _peopleIdEdit->text().toStdString();
    std::string stuffIdStr = _stuffIdEdit->text().toStdString();
    std::string dateStr = _dateEdit->text().toStdString();
    std::string commentStr = _commentEdit->toPlainText().toStdString();

    std::string journalSqlCommand = "INSERT INTO " + std::string(JOURNAL_TABLE) +
            "(type_table, money, people_id, stuff_id, record_time, comment) " +
            "VALUES('" + typeTableStr + "', " + moneyStr + ", " + peopleIdStr +
            ", " + stuffIdStr + ", '" + dateStr + "', '" + commentStr + "')";

    std::string incomeConsumptionCommand;

    if(typeTableStr == INCOME_TABLE)
    {
        incomeConsumptionCommand = "INSERT INTO " + std::string(INCOME_TABLE) +
                "(money, people_id) VALUES(" + moneyStr + ", " + peopleIdStr + ")";
    }
    else if(typeTableStr == CONSUMPTION_TABLE)
    {
        incomeConsumptionCommand = "INSERT INTO " + std::string(CONSUMPTION_TABLE) +
                "(stuff_id, people_id) VALUES(" + stuffIdStr + ", " + peopleIdStr + ")";
    }
    else
    {
        QMessageBox::critical(this, "Error", "Неверно указано имя таблицы");
        return;
    }

    std::vector<std::string> commands = { journalSqlCommand, incomeConsumptionCommand };
    sendToTables(commands);
    QMessageBox::information(this, "Done", "Запись успешно добавлена");
}

void RecordTableManipulator::sendRequest()
{
    _manipulationTableType == addRecord ? addRecordIntoTable() : editRecordInTable();
}
