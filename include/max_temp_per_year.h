#ifndef MAX_TEMP_PER_YEAR_H
#define MAX_TEMP_PER_YEAR_H

#include "data_types.h"
#include <vector>
#include <map>

// Function that computes the hottest (maximum average) temperature for each year
std::map<int, double> computeMaxTemperaturePerYear(const std::map<std::string, double> &avgPerDate);

#endif // MAX_TEMP_PER_YEAR_H
