#include "max_temp_per_year.h"
#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include <map>

/*
  computeMaxTemperaturePerYear

  Takes as input a map of {date → average temperature},
  extracts the year from each date, and finds the hottest (maximum)
  temperature recorded in that year.

  Expected date format: "YYYY-MM-DD"
*/

std::map<int, double> computeMaxTemperaturePerYear(const std::map<std::string, double> &avgPerDate)
{
    std::map<int, double> yearlyMax;

    for (const auto &entry : avgPerDate)
    {
        const std::string &date = entry.first;
        double temperature = entry.second;

        // Extract year from "YYYY-MM-DD"
        std::istringstream iss(date.substr(0, 4));
        int year;
        iss >> year;

        // Initialize if year not present or update if temperature is higher
        if (yearlyMax.find(year) == yearlyMax.end() || temperature > yearlyMax[year])
        {
            yearlyMax[year] = temperature;
        }
    }

    return yearlyMax;
}
