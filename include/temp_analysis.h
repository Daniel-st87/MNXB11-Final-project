#pragma once

#include <string>
#include <vector>
#include <map>
#include "average_temp.h"
#include "data_types.h"

// Return 1..366 for a given Y-M-D 
int day_of_year(int y, int m, int d);



// Per-year hottest/coldest day (by daily mean)
struct DayStat {
    int         doy;   // 1..366
    std::string date;  // "YYYY-MM-DD"
    double      mean;  // daily mean temperature
};

struct YearExtremes {
    DayStat hottest;
    DayStat coldest;
    bool    initialized{false};
};

// year -> extremes (hottest, coldest)
std::map<int, YearExtremes>
find_hottest_coldest_by_year(const std::map<std::string,double>& dailyMean);
