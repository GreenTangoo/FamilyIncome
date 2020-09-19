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

#include "databasewrapper.h"

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
    std::vector<QWidget*> childWidgets;
private:
    std::shared_ptr<QLineEdit> tablenameInputField;
    std::shared_ptr<QPushButton> showTableBut;
    std::shared_ptr<QPushButton> addRecordBut;
    std::shared_ptr<QPushButton> editRecordBut;

    std::shared_ptr<QPlainTextEdit> commandHistory;
    std::shared_ptr<QLineEdit> sqlCommandInput;

    std::shared_ptr<QRadioButton> firstReportBut;
    std::shared_ptr<QRadioButton> secondReportBut;
    std::shared_ptr<QPushButton> exportReportBut;
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
public:
    explicit MainWindow(DatabaseWrapper &database, QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
