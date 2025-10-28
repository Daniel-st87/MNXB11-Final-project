#include "temp_analysis.h"
#include "average_temp.h"
#include <limits>
#include <cctype>

// ---- helpers: leap year + day_of_year (no C++20 needed) ----
static bool is_leap(int y) {
    return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

int day_of_year(int y, int m, int d) {
    static const int mdays_norm[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    static const int mdays_leap[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
    const int* mdays = is_leap(y) ? mdays_leap : mdays_norm;

    int doy = d;
    for (int i = 0; i < m - 1; ++i) doy += mdays[i];
    return doy; // 1..366
}

// ---- daily means: "YYYY-MM-DD" -> average temperature ----
std::map<std::string, double>
calc_average_temp(const std::vector<DataRow>& data)
{
    std::map<std::string, std::pair<double,int>> sumCnt;
    for (const auto& row : data) {
        auto& sc = sumCnt[row.date]; // default-inits to {0.0, 0} if new
        sc.first  += row.temperature;
        sc.second += 1;
    }

    std::map<std::string,double> out;
    for (const auto& kv : sumCnt) {
        const auto& date = kv.first;
        double sum = kv.second.first;
        int    cnt = kv.second.second;
        if (cnt > 0) out[date] = sum / static_cast<double>(cnt);
    }
    return out;
}

// per-year extremes (hottest/coldest) by daily mean ----
std::map<int, YearExtremes>
find_hottest_coldest_by_year(const std::map<std::string,double>& dailyMean)
{
    std::map<int, YearExtremes> perYear;

    for (const auto& kv : dailyMean) {
        const std::string& date = kv.first;   // "YYYY-MM-DD"
        const double mean       = kv.second;

        if (date.size() < 10 || date[4] != '-' || date[7] != '-') continue;

        int Y = 0, M = 0, D = 0;
        try {
            Y = std::stoi(date.substr(0,4));
            M = std::stoi(date.substr(5,2));
            D = std::stoi(date.substr(8,2));
        } catch (...) {
            continue;
        }

        int doy = day_of_year(Y, M, D);

        auto& y = perYear[Y]; // creates default YearExtremes if missing
        if (!y.initialized) {
            y.hottest = {doy, date, mean};
            y.coldest = {doy, date, mean};
            y.initialized = true;
        } else {
            if (mean > y.hottest.mean) y.hottest = {doy, date, mean};
            if (mean < y.coldest.mean) y.coldest = {doy, date, mean};
        }
    }

    return perYear;
}
