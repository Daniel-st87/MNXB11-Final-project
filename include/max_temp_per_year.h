#ifndef MAX_TEMP_PER_YEAR_H
#define MAX_TEMP_PER_YEAR_H

#include "data_types.h" //includes data row structure for use
#include <vector> //enables the use of std::vector
#include <map> //enables the use of std::map

//Max temp function
std::map<int, double> computeMaxTemperaturePerYear(const std::map<std::string, double> &avgPerDate);

#endif //end of include guard
