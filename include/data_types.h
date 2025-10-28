#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <string>

struct DataRow // shared data structure for all of the processing of CSV
{
    std::string date;
    std::string time;
    double temperature;
    std::string sourceFile;
};

struct Wave // used for the heat and cold wave figures
{
    std::string start_date;
    std::string end_date;
    int duration;
};

#endif // DATA_TYPES_H