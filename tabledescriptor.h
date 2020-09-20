#ifndef TABLEDESCRIPTOR_H
#define TABLEDESCRIPTOR_H

#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>

#include <QWidget>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPixmap>
#include <QFormLayout>
#include <QScrollArea>

#include "databasewrapper.h"

class TableDescriptor : public QWidget
{
    Q_OBJECT
private:
    CallbackResult _tableData;
    std::vector<std::pair<std::shared_ptr<QLabel>, std::shared_ptr<QPlainTextEdit>>> _graphicRecords;
private:
    std::shared_ptr<QFormLayout> _mainLayout;
    std::shared_ptr<QScrollArea> _scrollArea;
private:
    void showTable();
    std::string getImagePath(OneRecord const &record);
    std::string getDescription(OneRecord const &record);
public:
    explicit TableDescriptor(CallbackResult tableData, QWidget *parent = nullptr);
    ~TableDescriptor();
};

#endif // TABLEDESCRIPTOR_H
