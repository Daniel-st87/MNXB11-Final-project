#ifndef DECADE_AVERAGE_H
#define DECADE_AVERAGE_H

#include "data_types.h"
#include <vector>
#include <map>

std::map<int, double> calculate_decade_averages(const std::map<int, int> &events_per_year);

#endif // DECADE_AVERAGE_H
