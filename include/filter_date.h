#ifndef FILTER_DATE_H
#define FILTER_DATE_H

#include <string>
#include <vector>
#include "data_types.h"

std::vector<DataRow> filterDate(const std::string &csvFile, const std::string &targetDate); // reads CSV file adn returns the row that match the date

bool isDate(const std::string &date, const std::string &targetDate); // makes sure that its the correct date

void writeData(const std::string &outputFile, const std::vector<DataRow> &data); // This writes the data to a new CSV file

#endif // FILTER_DATE_H
