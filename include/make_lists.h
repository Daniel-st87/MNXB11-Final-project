#ifndef MAKE_LISTS_H
#define MAKE_LISTS_H

#include <vector>
#include <map>
#include <string>

void makeLists(const std::map<std::string, double> &averages, // splits the map into two lists, one for years and the other for temps
               std::vector<double> &years,
               std::vector<double> &temps);

#endif // MAKE_LISTS_H
