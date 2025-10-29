#include "data_types.h"
#include "CSV_reader.h"
#include "average_temp.h"
#include "max_temp_per_year.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

void plotMaxTempPerYear(const std::map<int, double> &yearlyMax);

int main_max_Soderarm()
{
    std::vector<DataRow> data = readCSV("Preprocessed_data/Soderarm.csv"); //uses readCSV function to compute data 
    
    // compute average temperature per date
    auto avgPerDate = calc_average_temp(data);

    // compute hottest (max avg) temperature per year
    auto maxPerYear = computeMaxTemperaturePerYear(avgPerDate);

    // print the results
    std::cout << "Year\tMax Avg Temp (°C)" << std::endl;
    for (const auto &entry : maxPerYear)
    {
        std::cout << entry.first << "\t" << entry.second << std::endl;
    }

    // plot results using ROOT
    plotMaxTempPerYear(maxPerYear);

    return 0;
}
