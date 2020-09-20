#include "tabledescriptor.h"

#define ICON "icon"
#define AVATAR "avatar"

TableDescriptor::TableDescriptor(CallbackResult tableData, QWidget *parent) : QWidget(parent), _tableData(tableData)
{
    showTable();
}

TableDescriptor::~TableDescriptor()
{
}

void TableDescriptor::showTable()
{
    _mainLayout = std::make_shared<QFormLayout>();
    this->setLayout(_mainLayout.get());

    _scrollArea = std::make_shared<QScrollArea>();
    _scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    _scrollArea->setWidgetResizable(true);
    _scrollArea->setWidget(this);
    _scrollArea->setWindowTitle("Table description");
    _scrollArea->setFixedSize(500, 400);


    for(size_t i(0); i < _tableData.records.size(); i++)
    {
        std::shared_ptr<QLabel> imageLabel;
        std::shared_ptr<QPlainTextEdit> descriptionPlain;

        std::string imagePath = getImagePath(_tableData.records[i]);

        if((imagePath.length() == 0) || !(std::filesystem::exists(imagePath)))
        {
            imageLabel = std::make_shared<QLabel>("Nan image");
        }
        else
        {
            imageLabel = std::make_shared<QLabel>();
            QPixmap imagePixMap(imagePath.c_str());
            imagePixMap = imagePixMap.scaled(100, 100);
            imageLabel->setPixmap(imagePixMap);
        }

        std::string descriptionInfo = getDescription(_tableData.records[i]);
        descriptionPlain = std::make_shared<QPlainTextEdit>();
        descriptionPlain->setReadOnly(true);
        descriptionPlain->appendPlainText(QString::fromStdString(descriptionInfo));

        _mainLayout->addRow(imageLabel.get(), descriptionPlain.get());
        _graphicRecords.push_back({imageLabel, descriptionPlain});
    }

    _scrollArea->show();
}

std::string TableDescriptor::getImagePath(OneRecord const &record)
{
    std::vector<std::pair<std::string, std::string>>::const_iterator it =
            std::find_if(record.results.begin(), record.results.end(),
            [](std::pair<std::string, std::string> const &element) -> bool
    {
            return element.first == ICON || element.first == AVATAR;
    });

    return it == std::end(record.results) ? "" : it->second;
}

std::string TableDescriptor::getDescription(OneRecord const &record)
{
    std::string resultStr;

    for(size_t i(0); i < record.results.size(); i++)
    {
        std::string columnName = record.results[i].first;
        if((columnName != AVATAR) && (columnName != ICON))
        {
            resultStr += (columnName + ": " + record.results[i].second + "\n");
        }
    }

    return resultStr;
}
