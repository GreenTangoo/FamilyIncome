#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QMessageBox>

#include "tabledescriptor.h"
#include "recordtablemanipulator.h"
#include "databasewrapper.h"
#include "htmlexporter.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *_ui;
    DatabaseWrapper &_database;
private:
    std::shared_ptr<QLineEdit> _tablenameInputField;
    std::shared_ptr<QPushButton> _showTableBut;
    std::shared_ptr<QPushButton> _addRecordBut;
    std::shared_ptr<QPushButton> _editRecordBut;

    std::shared_ptr<QPlainTextEdit> _commandHistory;
    std::shared_ptr<QLineEdit> _sqlCommandInput;

    std::shared_ptr<QRadioButton> _firstReportBut;
    std::shared_ptr<QRadioButton> _secondReportBut;
    std::shared_ptr<QPushButton> _exportReportBut;

    std::shared_ptr<QPushButton> _standartAddingBut;
private:
    void initializeAllQtElements();
    void initializeTablesManipulationElements();
    void initializeSqlCommandElements();
    void initializeReportElements();
    void setSignals();
    void showSqlCommandElements();
    void showTablesManipulationElements();
    void showExportElements();
private slots:
    void exportRecordSlot();
    void showTableSlot();
    void addRecordSlot();
    void editRecordSlot();
    void sqlCommandInputSlot();
    void fillTablesSlot();
protected:
    void closeEvent(QCloseEvent *event);
public:
    explicit MainWindow(DatabaseWrapper &database, QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
