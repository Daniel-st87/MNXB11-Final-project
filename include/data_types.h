#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <string>

struct DataRow // shared data_structure for all of the processing of CSV
{
    std::string date;
    std::string time;
    double temperature;
    std::string sourceFile;
};

#endif // DATA_TYPES_H