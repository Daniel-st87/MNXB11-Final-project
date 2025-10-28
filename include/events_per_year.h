#ifndef EVENTS_PER_YEAR_H
#define EVENTS_PER_YEAR_H

#include <map>
#include <string>
#include "CSV_reader.h"

std::map<int, int> count_events_per_year(const std::map<std::string, double> &daily, double threshold, int min_days, bool is_heat);

#endif // EVENTS_PER_YEAR_H