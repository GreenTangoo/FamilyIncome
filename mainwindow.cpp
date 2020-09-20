#include "mainwindow.h"
#include "ui_mainwindow.h"

extern void fill_tables(DatabaseWrapper &database);

MainWindow::MainWindow(DatabaseWrapper &database, QWidget *parent) : QMainWindow(parent), _ui(new Ui::MainWindow), _database(database)
{
    _ui->setupUi(this);
    this->setWindowTitle("Family budget");
    this->setFixedSize(800, 600);

    initializeAllQtElements();
    setSignals();

    showSqlCommandElements();
    showExportElements();
    showTablesManipulationElements();
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    _database.stopExecution();
    event->accept();
}

void MainWindow::initializeAllQtElements()
{
    initializeTablesManipulationElements();
    initializeSqlCommandElements();
    initializeReportElements();
}

void MainWindow::initializeTablesManipulationElements()
{
    _tablenameInputField = std::make_shared<QLineEdit>(this);
    _tablenameInputField->setGeometry(this->width() - 160, 20, 140, 20);

    _showTableBut = std::make_shared<QPushButton>("Show table", this);
    _showTableBut->setGeometry(this->width() - 115, _tablenameInputField->height() + 30, 90, 30);

    _addRecordBut = std::make_shared<QPushButton>("Add record", this);
    _addRecordBut->setGeometry(this->width() - 115, _showTableBut->height() + 80, 90, 30);

    _editRecordBut = std::make_shared<QPushButton>("Edit record", this);
    _editRecordBut->setGeometry(this->width() - 115, _addRecordBut->height() + 120, 90, 30);

    _standartAddingBut = std::make_shared<QPushButton>("Fill tables", this);
    _standartAddingBut->setGeometry(this->width() - 115, this->height() - 40, 90, 30);
}

void MainWindow::initializeSqlCommandElements()
{
    _commandHistory = std::make_shared<QPlainTextEdit>(this);
    _commandHistory->setGeometry(40, 20, 550, 500);
    _commandHistory->setReadOnly(true);

    _sqlCommandInput = std::make_shared<QLineEdit>(this);
    _sqlCommandInput->setGeometry(40, _commandHistory->height() + 30, 550, 20);
}

void MainWindow::initializeReportElements()
{
    _firstReportBut = std::make_shared<QRadioButton>("First record", this);
    _firstReportBut->setChecked(true);
    _firstReportBut->setGeometry(40, _commandHistory->height() + _sqlCommandInput->height() + 45,
                                100, 20);

    _secondReportBut = std::make_shared<QRadioButton>("Second record", this);
    _secondReportBut->setGeometry(_firstReportBut->width() + 60, _commandHistory->height() + _sqlCommandInput->height() + 45,
                                120, 20);


    _exportReportBut = std::make_shared<QPushButton>("Export", this);
    _exportReportBut->setGeometry(300, _commandHistory->height() + _sqlCommandInput->height() + 40,
                                 90, 30);
}

void MainWindow::setSignals()
{
    connect(_exportReportBut.get(), SIGNAL(clicked()), this, SLOT(exportRecordSlot()));
    connect(_showTableBut.get(), SIGNAL(clicked()), this, SLOT(showTableSlot()));
    connect(_addRecordBut.get(), SIGNAL(clicked()), this, SLOT(addRecordSlot()));
    connect(_editRecordBut.get(), SIGNAL(clicked()), this, SLOT(editRecordSlot()));
    connect(_sqlCommandInput.get(), SIGNAL(returnPressed()), this, SLOT(sqlCommandInputSlot()));
    connect(_standartAddingBut.get(), SIGNAL(clicked()), this, SLOT(fillTablesSlot()));
}

void MainWindow::showSqlCommandElements()
{
    _commandHistory->show();
    _sqlCommandInput->show();
}

void MainWindow::showExportElements()
{
    _firstReportBut->show();
    _secondReportBut->show();
    _exportReportBut->show();
}

void MainWindow::showTablesManipulationElements()
{
    _tablenameInputField->show();
    _showTableBut->show();
    _addRecordBut->show();
    _editRecordBut->show();
}

/*------------------------------------------------------------*/
/*----------------------SLOTS---------------------------------*/
/*------------------------------------------------------------*/

void MainWindow::exportRecordSlot()
{
    HtmlExporter::typeReport typeRep = _firstReportBut->isChecked() ?
                HtmlExporter::first : HtmlExporter::second;

    HtmlExporter exporter(_database);
    exporter.exportReport(typeRep);
    QMessageBox::information(this, "Export information", "Экспорт данных прошел успешно");
}

void MainWindow::showTableSlot()
{
    QString tablename = _tablenameInputField->text();

    CallbackResult result = _database.showTable(tablename.toStdString());

    QWidget *showTableWidget = new TableDescriptor(result);
    showTableWidget->setAttribute(Qt::WA_DeleteOnClose, true);
    showTableWidget->show();
}

void MainWindow::addRecordSlot()
{
    QWidget *addRecordWidget = new RecordTableManipulator(_database, RecordTableManipulator::addRecord);
    addRecordWidget->setAttribute(Qt::WA_DeleteOnClose, true);
    addRecordWidget->show();
}

void MainWindow::editRecordSlot()
{
    QWidget *editRecordWidget = new RecordTableManipulator(_database, RecordTableManipulator::editRecord);
    editRecordWidget->setAttribute(Qt::WA_DeleteOnClose, true);
    editRecordWidget->show();
}

void MainWindow::sqlCommandInputSlot()
{
    QString command = _sqlCommandInput->text();
    _sqlCommandInput->clear();
    _commandHistory->appendPlainText(command);
    _database.pushCommand(command.toStdString());
}

void MainWindow::fillTablesSlot()
{
    fill_tables(_database);
}
