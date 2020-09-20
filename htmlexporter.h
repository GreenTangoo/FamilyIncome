#ifndef HTMLEXPORTER_H
#define HTMLEXPORTER_H

#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cctype>

#include "databasewrapper.h"

#define REPORT_FILE "report.html"

struct ReportInformation
{
    std::string stuffId;
    std::string categoryId;
    int money;
};

struct PeopleReportInformation
{
    std::string peopleId;
    int spendMoney;
    std::pair<std::string, std::string> minMaxDate;
    std::vector<ReportInformation> stuffs;
};

class HtmlExporter
{
private:
    DatabaseWrapper &_database;
private:
    void exportFirstReport();
    void exportSecondReport();
    std::pair<std::string, std::string> getMinMaxDateByPeopleId(std::string peopleId);
    std::vector<std::string> getAllStuffs();
    std::vector<std::string> getAllPeoples();
    std::vector<std::string> getAllStuffsByPeopleId(std::string peopleId);
    int getAllMoneyByStuffId(std::string stuffId);
    int getAllMoneyByPeopleId(std::string peopleId);
    bool isConsumptionTable(OneRecord const &record);
    std::string getCategoryIdByStuffId(std::string stuffId);
    void createHtmlDocForFirstReport(std::vector<PeopleReportInformation> const &infoVec);
    void createHtmlDocForSecondReport(std::vector<ReportInformation> const &infoVec);
public:
    enum typeReport { first = 0, second };
    HtmlExporter(DatabaseWrapper &database);
    ~HtmlExporter();
    void exportReport(typeReport tReport);
};

#endif // HTMLEXPORTER_H
