#include "data_types.h"
#include "average_temp.h"
#include <iostream>
#include <map>
#include <numeric>

std::map<std::string, double> calc_average_temp(const std::vector<DataRow> &data)
{
    std::map<std::string, std::pair<double, int>> sumAndCount; // dictionary where the date is the key, and the thing associated is a pair of the sum and number of entries

    for (const auto &row : data)
    {
        auto &entry = sumAndCount[row.date]; // this creates the key, does so its the date
        entry.first += row.temperature;      // this gets the sum
        entry.second += 1;                   // this gets the count
    }

    std::map<std::string, double> averages; // creates another dictionary where key will be date, and entry will be the average temp

    for (const auto &pair : sumAndCount)
    {
        const std::string &date = pair.first;
        double sum = pair.second.first;
        int count = pair.second.second; // does so it takes the second part of the second part of the key value pair (the count part of the value part)

        averages[date] = sum / count; // assigns a value to each key in this dictionary
    }

    return averages;
}
