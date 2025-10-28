#include "events_per_year.h"

std::map<int, int> count_events_per_year(const std::map<std::string, double> &daily, double threshold, int min_days, bool is_heat)
{
    std::map<int, int> yearly_counts;

    int consecutive_days = 0;
    for (const auto &day : daily)
    {
        double temperature = day.second;
        bool is_extreme;
        if (is_heat)
        {
            if (temperature >= threshold)
            {
                is_extreme = true;
            }
            else
            {
                is_extreme = false;
            }
        }
        else
        {
            if (temperature <= threshold)
            {
                is_extreme = true;
            }
            else
            {
                is_extreme = false;
            }
        }
        if (is_extreme)
        {
            consecutive_days++;
        }
        else
        {
            if (consecutive_days >= min_days)
            {
                int year = year_of(day.first);
                yearly_counts[year]++;
            }
            consecutive_days = 0;
        }
    }

    return yearly_counts;
}