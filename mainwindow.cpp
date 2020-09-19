#include "mainwindow.h"
#include "ui_mainwindow.h"


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

void MainWindow::initializeAllQtElements()
{
    initializeTablesManipulationElements();
    initializeSqlCommandElements();
    initializeReportElements();
}

void MainWindow::initializeTablesManipulationElements()
{
    tablenameInputField = std::make_shared<QLineEdit>(this);
    tablenameInputField->setGeometry(this->width() - 160, 20, 140, 20);

    showTableBut = std::make_shared<QPushButton>("Show table", this);
    showTableBut->setGeometry(this->width() - 115, tablenameInputField->height() + 30, 90, 30);

    addRecordBut = std::make_shared<QPushButton>("Add record", this);
    addRecordBut->setGeometry(this->width() - 115, showTableBut->height() + 80, 90, 30);

    editRecordBut = std::make_shared<QPushButton>("Edit record", this);
    editRecordBut->setGeometry(this->width() - 115, addRecordBut->height() + 120, 90, 30);
}

void MainWindow::initializeSqlCommandElements()
{
    commandHistory = std::make_shared<QPlainTextEdit>(this);
    commandHistory->setGeometry(40, 20, 550, 500);
    commandHistory->setReadOnly(true);

    sqlCommandInput = std::make_shared<QLineEdit>(this);
    sqlCommandInput->setGeometry(40, commandHistory->height() + 30, 550, 20);
}

void MainWindow::initializeReportElements()
{
    firstReportBut = std::make_shared<QRadioButton>("First record", this);
    firstReportBut->setChecked(true);
    firstReportBut->setGeometry(40, commandHistory->height() + sqlCommandInput->height() + 45,
                                100, 20);

    secondReportBut = std::make_shared<QRadioButton>("Second record", this);
    secondReportBut->setGeometry(firstReportBut->width() + 60, commandHistory->height() + sqlCommandInput->height() + 45,
                                120, 20);


    exportReportBut = std::make_shared<QPushButton>("Export", this);
    exportReportBut->setGeometry(300, commandHistory->height() + sqlCommandInput->height() + 40,
                                 90, 30);
}

void MainWindow::setSignals()
{
    connect(exportReportBut.get(), SIGNAL(clicked()), this, SLOT(exportRecordSlot()));
    connect(showTableBut.get(), SIGNAL(clicked()), this, SLOT(showTableSlot()));
    connect(addRecordBut.get(), SIGNAL(clicked()), this, SLOT(addRecordSlot()));
    connect(editRecordBut.get(), SIGNAL(clicked()), this, SLOT(editRecordSlot()));
    connect(sqlCommandInput.get(), SIGNAL(returnPressed()), this, SLOT(sqlCommandInputSlot()));
}

void MainWindow::showSqlCommandElements()
{
    commandHistory->show();
    sqlCommandInput->show();
}

void MainWindow::showExportElements()
{
    firstReportBut->show();
    secondReportBut->show();
    exportReportBut->show();
}

void MainWindow::showTablesManipulationElements()
{
    tablenameInputField->show();
    showTableBut->show();
    addRecordBut->show();
    editRecordBut->show();
}

/*------------------------------------------------------------*/
/*----------------------SLOTS---------------------------------*/
/*------------------------------------------------------------*/

void MainWindow::exportRecordSlot()
{
    // TODO: create RecordCreater class
}

void MainWindow::showTableSlot()
{
    QWidget *showTableWidget = new QWidget;
    showTableWidget->setAttribute(Qt::WA_DeleteOnClose, true);
    showTableWidget->setWindowTitle("Show table");
    showTableWidget->show();

    //TODO: write show table by tablename code.
}

void MainWindow::addRecordSlot()
{
    QWidget *addRecordWidget = new QWidget;
    addRecordWidget->setAttribute(Qt::WA_DeleteOnClose, true);
    addRecordWidget->setWindowTitle("Add new record");
    addRecordWidget->show();

    //TODO: write adding a new value into record table code.
}

void MainWindow::editRecordSlot()
{
    QWidget *editRecordWidget = new QWidget;
    editRecordWidget->setAttribute(Qt::WA_DeleteOnClose, true);
    editRecordWidget->setWindowTitle("Edit record");
    editRecordWidget->show();

    //TODO: write editing an exists record code.
}

void MainWindow::sqlCommandInputSlot()
{
    QString command = sqlCommandInput->text();
    sqlCommandInput->clear();
    commandHistory->appendPlainText(command);
    //TODO: call DatabaseWrapper::pushCommand method.
}
