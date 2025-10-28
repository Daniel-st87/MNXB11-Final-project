#include "CSV_reader.h"
#include "data_types.h"
#include <fstream>
#include <sstream>
#include <cstdlib>

std::vector<DataRow> readCSV(const std::string &path)
{
    std::vector<DataRow> data;
    std::ifstream file(path);
    if (!file.is_open())
        return data;

    std::string line;
    while (std::getline(file, line))
    {
        DataRow row;
        std::stringstream ss(line);
        std::string temp_string;
        std::getline(ss, row.date, ',');
        std::getline(ss, row.time, ',');
        std::getline(ss, temp_string, ',');
        row.temperature = std::stod(temp_string);
        data.push_back(row);
    }
    return data;
}

int year_of(const std::string &iso)
{
    std::string year_str = iso.substr(0, 4);
    int year = std::stoi(year_str);
    return year;
}
