
#include "decade_average.h"

std::map<int, double> calculate_decade_averages(const std::map<int, int> &events_per_year)
{
    std::map<int, double> decade_averages;
    std::map<int, int> decade_counts;
    std::map<int, int> decade_totals;

    // Group years by decade
    for (std::map<int, int>::const_iterator it = events_per_year.begin(); it != events_per_year.end(); ++it)
    {
        int year = it->first;
        int events = it->second;
        int decade_start = (year / 10) * 10;

        decade_counts[decade_start] = decade_counts[decade_start] + 1;
        decade_totals[decade_start] = decade_totals[decade_start] + events;
    }

    // Calculate averages
    for (std::map<int, int>::iterator it = decade_counts.begin(); it != decade_counts.end(); ++it)
    {
        int decade = it->first;
        int count = it->second;
        int total = decade_totals[decade];
        double average = (double)total / count;
        decade_averages[decade] = average;
    }

    return decade_averages;
}