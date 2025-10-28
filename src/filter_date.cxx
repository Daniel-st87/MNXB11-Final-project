#include "filter_date.h"
#include "CSV_reader.h"
#include "data_types.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

bool isDate(const std::string &date, const std::string &targetMonthDay)
{
    std::string monthDay = date.substr(5, 5); // extract MM-DD from YYYY-MM-DD, makes sure that the date is same as the target date
    return monthDay == targetMonthDay;
}

// filter CSV by target date using readCSV()
std::vector<DataRow> filterDate(const std::string &csvFile, const std::string &targetMonthDay)
{
    std::vector<DataRow> allData = readCSV(csvFile); // reuse CSV reader
    std::vector<DataRow> filteredData;

    for (const auto &row : allData)
    {
        if (isDate(row.date, targetMonthDay))
        {
            filteredData.push_back(row);
        }
    }

    return filteredData;
}

void writeData(const std::string &outputFile, const std::vector<DataRow> &data)
{
    std::ofstream outFile(outputFile); // opens the output file that will have DataRows in it
    if (!outFile.is_open())
    {
        std::cerr << "Could not open output file " << outputFile << "\n";
        return;
    }

    outFile << "date,time,temperature,sourceFile\n"; // writes the header

    for (const auto &row : data)
    {
        outFile << row.date << "," << row.time << "," << row.temperature << "," << row.sourceFile << "\n"; // writes each datarow object as a CSV line
    }

    outFile.close();
}
