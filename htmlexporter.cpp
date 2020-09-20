#include "htmlexporter.h"

#define HTML_HEADER "<!DOCTYPE HTML>\n"\
                    "<html> \n <head>\n"\
                    "<meta charset=\"utf-8\">\n"\
                    "<title>Report</title>\n"\
                    "</head>\n"
#define BODY "<body>"
#define END_BODY "</body>"
#define END_HTML "</html>"
#define PARAGRAPH "<p>"
#define END_PARAGRAPH "</p>"
#define NEW_LINE "<br>"


HtmlExporter::HtmlExporter(DatabaseWrapper &database) : _database(database)
{

}

HtmlExporter::~HtmlExporter()
{

}

void HtmlExporter::exportReport(typeReport tReport)
{
    tReport == first ? exportFirstReport() : exportSecondReport();
}

void HtmlExporter::exportFirstReport()
{
    std::vector<PeopleReportInformation> mainInfoAboutPeoples;

    std::vector<std::string> peoples = getAllPeoples();
    for(size_t i(0); i < peoples.size(); i++)
    {
        PeopleReportInformation onePeopleInfo;
        onePeopleInfo.peopleId = peoples[i];
        onePeopleInfo.spendMoney = getAllMoneyByPeopleId(onePeopleInfo.peopleId);
        onePeopleInfo.minMaxDate = getMinMaxDateByPeopleId(onePeopleInfo.peopleId);

        std::vector<ReportInformation> onePeopleStuffsInfo;
        std::vector<std::string> stuffs = getAllStuffsByPeopleId(onePeopleInfo.peopleId);
        for(size_t j(0); j < stuffs.size(); j++)
        {
            ReportInformation oneStuffInfo;
            oneStuffInfo.stuffId = stuffs[j];
            oneStuffInfo.categoryId = getCategoryIdByStuffId(oneStuffInfo.stuffId);

            onePeopleStuffsInfo.push_back(oneStuffInfo);
        }

        onePeopleInfo.stuffs = onePeopleStuffsInfo;
        mainInfoAboutPeoples.push_back(onePeopleInfo);
    }

    std::sort(mainInfoAboutPeoples.begin(), mainInfoAboutPeoples.end(),
              [](PeopleReportInformation const &first, PeopleReportInformation const &second) -> bool
    {
        return first.spendMoney > second.spendMoney;
    });

    createHtmlDocForFirstReport(mainInfoAboutPeoples);
}

void HtmlExporter::exportSecondReport()
{
    std::vector<ReportInformation> mainInfoAboutStuffs;

    std::vector<std::string> stuffs = getAllStuffs();
    for(size_t i(0); i < stuffs.size(); i++)
    {
        ReportInformation oneStuffInfo;
        oneStuffInfo.stuffId = stuffs[i];
        oneStuffInfo.money = getAllMoneyByStuffId(oneStuffInfo.stuffId);
        oneStuffInfo.categoryId = getCategoryIdByStuffId(oneStuffInfo.stuffId);

        mainInfoAboutStuffs.push_back(oneStuffInfo);
    }

    std::sort(mainInfoAboutStuffs.begin(), mainInfoAboutStuffs.end(),
              [](ReportInformation const &first, ReportInformation const &second) ->bool
    {
        return first.money > second.money;
    });

    createHtmlDocForSecondReport(mainInfoAboutStuffs);
}

std::pair<std::string, std::string> HtmlExporter::getMinMaxDateByPeopleId(std::string peopleId)
{
    CallbackResult dateResult = _database.singleSelect(JOURNAL_TABLE, "record_time", "people_id", peopleId);
    std::vector<std::string> dates;

    if(dateResult.records.size() > 0)
    {
        for(size_t i(0); i < dateResult.records.size(); i++)
        {
            std::string dateString = dateResult.records[i].results[0].second;
            dates.push_back(dateString);
        }
    }

    std::sort(dates.begin(), dates.end(), [](std::string const &first, std::string const &second) -> bool
    {
        return first > second;
    });

    if(dates.size() > 0)
    {
        return {dates[0], dates[dates.size() - 1]};
    }
    else return {"", ""};
}

std::vector<std::string> HtmlExporter::getAllStuffs()
{
    std::vector<std::string> allStuffs;

    CallbackResult stuffsIdResult = _database.showTable(STUFF_TABLE);

    for(size_t i(0); i < stuffsIdResult.records.size(); i++)
    {
        OneRecord record = stuffsIdResult.records[i];

        std::vector<std::pair<std::string, std::string>>::iterator it =
                std::find_if(record.results.begin(), record.results.end(),
                             [](std::pair<std::string, std::string> const &elem) -> bool
        {
            return elem.first == "id";
        });

        if(it != std::end(record.results))
        {
            allStuffs.push_back(it->second);
        }
    }

    return allStuffs;
}

std::vector<std::string> HtmlExporter::getAllPeoples()
{
    std::vector<std::string> allPeoples;

    CallbackResult peoplesResult = _database.showTable(PEOPLE_TABLE);

    for(size_t i(0); i < peoplesResult.records.size(); i++)
    {
        OneRecord record = peoplesResult.records[i];

        std::vector<std::pair<std::string, std::string>>::iterator it =
                std::find_if(record.results.begin(), record.results.end(),
                             [](std::pair<std::string, std::string> const &elem) -> bool
        {
            return elem.first == "id";
        });

        if(it != std::end(record.results))
        {
            allPeoples.push_back(it->second);
        }
    }

    return allPeoples;
}

std::vector<std::string> HtmlExporter::getAllStuffsByPeopleId(std::string peopleId)
{
    std::vector<std::string> allStuffs;

    CallbackResult stuffsResult = _database.singleSelect(CONSUMPTION_TABLE, "stuff_id", "people_id", peopleId);

    if(stuffsResult.records.size() > 0)
    {
        for(size_t i(0); i < stuffsResult.records.size(); i++)
        {
            OneRecord record = stuffsResult.records[i];

            allStuffs.push_back(record.results[0].second);
        }
    }

    return allStuffs;
}

int HtmlExporter::getAllMoneyByStuffId(std::string stuffId)
{
    int allMoney = 0;
    CallbackResult moneyColumns = _database.customSelect(JOURNAL_TABLE, "stuff_id", stuffId);

    for(size_t i(0); i < moneyColumns.records.size(); i++)
    {
        OneRecord record = moneyColumns.records[i];

        if(isConsumptionTable(record))
        {
            std::vector<std::pair<std::string, std::string>>::iterator it =
                    std::find_if(record.results.begin(), record.results.end(),
                                 [](std::pair<std::string, std::string> const &elem) -> bool
            {
                return elem.first == "money";
            });

            allMoney += it != std::end(record.results) ? atoi(it->second.c_str()) : 0;
        }
    }

    return allMoney;
}

int HtmlExporter::getAllMoneyByPeopleId(std::string peopleId)
{
    int allMoney = 0;
    CallbackResult moneyColumns = _database.customSelect(JOURNAL_TABLE, "people_id", peopleId);

    for(size_t i(0); i < moneyColumns.records.size(); i++)
    {
        OneRecord record = moneyColumns.records[i];

        if(isConsumptionTable(record))
        {
            std::vector<std::pair<std::string, std::string>>::iterator it =
                    std::find_if(record.results.begin(), record.results.end(),
                                 [](std::pair<std::string, std::string> const &elem) -> bool
            {
                return elem.first == "money";
            });

            allMoney += it != std::end(record.results) ? atoi(it->second.c_str()) : 0;
        }
    }

    return allMoney;
}

bool HtmlExporter::isConsumptionTable(const OneRecord &record)
{
    std::vector<std::pair<std::string, std::string>>::const_iterator it =
            std::find_if(record.results.begin(), record.results.end(),
                         [](std::pair<std::string, std::string> const &elem) -> bool
    {
        return elem.first == "type_table";
    });

    if(it != std::end(record.results))
    {
        if(it->second == CONSUMPTION_TABLE)
        {
            return true;
        }
    }
    return false;
}

std::string HtmlExporter::getCategoryIdByStuffId(std::string stuffId)
{
    CallbackResult categoryId = _database.singleSelect(STUFF_TABLE, "id_category", "id", stuffId);
    if(categoryId.records.size() > 0)
    {
        return categoryId.records[0].results[0].second;
    }
    return "";
}

void HtmlExporter::createHtmlDocForFirstReport(const std::vector<PeopleReportInformation> &infoVec)
{
    std::ofstream fout(REPORT_FILE, std::ios_base::trunc);

    std::string htmlDocument = std::string(HTML_HEADER);
    htmlDocument += std::string(BODY) + "\n";

    for(size_t i(0); i < infoVec.size(); i++)
    {
        htmlDocument += std::string(PARAGRAPH) + "\n";
        htmlDocument += "People id: " + infoVec[i].peopleId + std::string(NEW_LINE) + "\n";
        htmlDocument += "Spend money: " + std::to_string(infoVec[i].spendMoney) + std::string(NEW_LINE) + "\n";
        htmlDocument += "Min-Max Date: " + infoVec[i].minMaxDate.first + " - " + infoVec[i].minMaxDate.second +
                std::string(NEW_LINE) + "\n";
        for(size_t j(0); j < infoVec[i].stuffs.size(); j++)
        {
            htmlDocument += "Stuff id: " + infoVec[i].stuffs[j].stuffId + std::string(NEW_LINE) + "\n";
            htmlDocument += "Category id: " + infoVec[i].stuffs[j].categoryId + std::string(NEW_LINE) + "\n";
        }
        htmlDocument += std::string(END_PARAGRAPH) + "\n";
    }

    htmlDocument += std::string(END_BODY) + "\n";
    htmlDocument += std::string(END_HTML) + "\n";

    fout.write(htmlDocument.c_str(), htmlDocument.size());
    fout.close();
}

void HtmlExporter::createHtmlDocForSecondReport(std::vector<ReportInformation> const &infoVec)
{
    std::ofstream fout(REPORT_FILE, std::ios_base::trunc);

    std::string htmlDocument = std::string(HTML_HEADER);
    htmlDocument += std::string(BODY) + "\n";

    for(size_t i(0); i < infoVec.size(); i++)
    {
        htmlDocument += std::string(PARAGRAPH) + "\n";
        htmlDocument += "Stuff id: " + infoVec[i].stuffId + std::string(NEW_LINE) + "\n";
        htmlDocument += "Category id: " + infoVec[i].categoryId + std::string(NEW_LINE) + "\n";
        htmlDocument += "Spend money: " + std::to_string(infoVec[i].money) + std::string(NEW_LINE) + "\n";
        htmlDocument += std::string(END_PARAGRAPH) + "\n";
    }

    htmlDocument += std::string(END_BODY) + "\n";
    htmlDocument += std::string(END_HTML) + "\n";

    fout.write(htmlDocument.c_str(), htmlDocument.size());
    fout.close();
}
