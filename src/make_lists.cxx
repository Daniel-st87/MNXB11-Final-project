#include "make_lists.h"
#include <string>
#include <algorithm>
#include <iostream>

void makeLists(const std::map<std::string, double> &averages,
               std::vector<double> &years,
               std::vector<double> &temps)
{
    years.clear(); // to make sure that the lists start empty
    temps.clear();

    for (const auto &entry : averages)
    {
        try
        {
            int year = std::stoi(entry.first.substr(0, 4)); // turns the text year into a double so it can be stored better in the list
            years.push_back(static_cast<double>(year));
            temps.push_back(entry.second);
        }
        catch (...)
        {
            std::cerr << "Warning: invalid date format in " << entry.first << "\n";
        }
    }
}
