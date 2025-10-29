#include "average_temp.h"
#include "max_temp_per_year.h"
#include "CSV_reader.h"
#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include <map>

std::map<int, double> computeMaxTemperaturePerYear(const std::map<std::string, double> &avgPerDate)
{ // computing the average max temp per year 
    std::map<int, double> yearlyMax; //storing the maximun temp found for each year in a map

    for (const auto &entry : avgPerDate) // loop through every date-temp pair in the input map
    {
        double temperature = entry.second; // finding the respective average temp for that date 

        int year = year_of(entry.first); // takes 4 first characters of the date (the year) and turns it into an integer

        if (yearlyMax.find(year) == yearlyMax.end() || temperature > yearlyMax[year])
        { //checking if the year is already inputed into the map, if yes compare to use the hottest value, if no input it into the data / map
            yearlyMax[year] = temperature; //updating or setting the correct value
        }
    }

    return yearlyMax; //producing the final result of year --> temperature
}
