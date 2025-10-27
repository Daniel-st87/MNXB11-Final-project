#ifndef CSV_READER_H
#define CSV_READER_H

#include "data_types.h"
#include <vector>
#include <string>

std::vector<DataRow> readCSV(const std::string &path);
int year_of(const std::string &iso);

#endif // CSV_READER_H