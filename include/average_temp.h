#ifndef AVERAGE_TEMP_H
#define AVERAGE_TEMP_H

#include "data_types.h"
#include <vector>
#include <map>

std::map<std::string, double> computeAveragePerDate(const std::vector<DataRow> &data); // calculates the average temperature

#endif // AVERAGE_TEMP_H
