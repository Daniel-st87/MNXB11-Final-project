#include "filter_date.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool isDate(const std::string &date, const std::string &targetMonthDay)
{
    std::string monthDay = date.substr(5, 5); // extract MM-DD from YYYY-MM-DD // makes sure that the date is same as the target date
    return monthDay == targetMonthDay;
}

std::vector<DataRow> filterDate(const std::string &csvFile, const std::string &targetMonthDay)
{
    std::ifstream file(csvFile);       // opens csv file for reading
    std::vector<DataRow> filteredData; // this is where data is stored
    std::string line;                  // temporary variable to read each line from csv

    if (!file.is_open()) // checks that the file can be opened
    {
        std::cerr << "Could not open file " << csvFile << "\n";
        return filteredData;
    }

    while (std::getline(file, line)) // reads the csv file line by line until the end of the file
    {
        std::stringstream ss(line);
        std::string date, time, tempStr;
        double temperature;

        std::getline(ss, date, ','); // This does so it splits the columns correctly
        std::getline(ss, time, ',');
        std::getline(ss, tempStr, ',');

        try // this does so program doesnt crash when it doesnt work
        {
            temperature = std::stod(tempStr); // converts the temperature from a string to double
        }
        catch (...)
        {
            continue; // skip invalid rows
        }

        if (isDate(date, targetMonthDay))
        {
            filteredData.push_back({date, time, temperature, csvFile}); // if it matches the correct date then creates row and adds it to filtered data
        }
    }

    file.close();
    return filteredData;
}

void writeData(const std::string &outputFile, const std::vector<DataRow> &data)
{
    std::ofstream outFile(outputFile);
    if (!outFile.is_open())
    {
        std::cerr << "Could not open output file " << outputFile << "\n";
        return;
    }

    outFile << "date,time,temperature,sourceFile\n"; // writes the header

    for (const auto &row : data)
    {
        outFile << row.date << "," << row.time << "," << row.temperature << "," << row.sourceFile << "\n";
    }

    outFile.close();
}
