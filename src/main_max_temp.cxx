#include "data_types.h"
#include "average_temp.h"
#include "max_temp_per_year.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

void plotMaxTempPerYear(const std::map<int, double> &yearlyMax);

int main() 
{
    std::vector<DataRow> data;
    std::ifstream file("Preprocessed_data/Halmstad.csv");
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open Preprocessed_data/Halmstad.csv" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        std::stringstream ss(line);
        DataRow row;
        std::string tempStr;

        std::getline(ss, row.date, ',');
        std::getline(ss, row.time, ',');
        std::getline(ss, tempStr, ',');

        try
        {
            row.temperature = std::stod(tempStr);
            row.sourceFile = "Preprocessed_data/Halmstad.csv";
            data.push_back(row);
        }
        catch (const std::invalid_argument &)
        {
            std::cerr << "Warning: Skipping invalid line: " << line << std::endl;
            continue;
        }
    }

    file.close();

    // Step 1: compute average temperature per date
    auto avgPerDate = calc_average_temp(data);

    // Step 2: compute hottest (max avg) temperature per year
    auto maxPerYear = computeMaxTemperaturePerYear(avgPerDate);

    // Step 3: print the results
    std::cout << "Year\tMax Avg Temp (°C)" << std::endl;
    for (const auto &entry : maxPerYear)
    {
        std::cout << entry.first << "\t" << entry.second << std::endl;
    }

    // Step 4: plot results using ROOT
    plotMaxTempPerYear(maxPerYear);

    return 0;
}
