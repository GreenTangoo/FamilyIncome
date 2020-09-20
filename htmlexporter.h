#ifndef HTMLEXPORTER_H
#define HTMLEXPORTER_H

#include <fstream>

#include "databasewrapper.h"

class HtmlExporter
{
private:
    DatabaseWrapper &_database;
private:
    void exportFirstReport();
    void exportSecondReport();
public:
    enum typeReport { first = 0, second };
    HtmlExporter(DatabaseWrapper &database);
    ~HtmlExporter();
    void exportReport(typeReport tReport);
};

#endif // HTMLEXPORTER_H
