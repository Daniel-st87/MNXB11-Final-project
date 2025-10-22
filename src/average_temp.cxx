#include "data_types.h"
#include "average_temp.h"
#include <iostream>
#include <map>
#include <numeric>

std::map<std::string, double> computeAveragePerDate(const std::vector<DataRow> &data)
{
    std::map<std::string, std::pair<double, int>> sumAndCount; // {date: (sum, count)}

    for (const auto &row : data)
    {
        auto &entry = sumAndCount[row.date];
        entry.first += row.temperature; // sum
        entry.second += 1;              // count
    }

    std::map<std::string, double> averages;

    for (const auto &pair : sumAndCount)
    {
        const std::string &date = pair.first;
        double sum = pair.second.first;
        int count = pair.second.second;

        averages[date] = sum / count;
    }

    return averages;
}
