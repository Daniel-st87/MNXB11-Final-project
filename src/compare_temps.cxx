#include "compare_temps.h"
#include "filter_date.h"
#include "average_temp.h"
#include "make_lists.h"
#include "data_types.h"

#include <TGraph.h>
#include <TString.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

int compare_temps()
{
    std::string csvCity1 = "Preprocessed_data/Lund.csv";
    std::string csvCity2 = "Preprocessed_data/Lulea.csv";
    std::string targetDay = "12-25"; // This selects the specific day that I want to analyse

    int startYear = 1500; // Selects earliest year for data to be analysed

    auto dataCity1 = filterDate(csvCity1, targetDay); // does so dataCity1 only has the info about lund from christmas
    auto dataCity2 = filterDate(csvCity2, targetDay);

    auto avgCity1 = calc_average_temp(dataCity1); // calculates the average temp
    auto avgCity2 = calc_average_temp(dataCity2);

    std::vector<double> years1, temps1, years2, temps2; // declares 4 empty vectors of type double
    makeLists(avgCity1, years1, temps1);                // converts the maps into two separate lists
    makeLists(avgCity2, years2, temps2);

    // this function below removes any data points with years before the startyear
    auto filterByYear = [startYear](std::vector<double> &years, std::vector<double> &temps)
    {
        std::vector<double> newYears, newTemps;
        for (size_t i = 0; i < years.size(); ++i)
        {
            if (years[i] >= startYear)
            {
                newYears.push_back(years[i]);
                newTemps.push_back(temps[i]);
            }
        }
        years = std::move(newYears); // Replaces the old vectors with the new filtered ones
        temps = std::move(newTemps);
    };

    filterByYear(years1, temps1);
    filterByYear(years2, temps2);

    std::vector<double> years, diffTemps;      // creates two new empty vectors, years is the years that they share, and diffTemps is difference in temperature
    for (size_t i = 0; i < years1.size(); ++i) // loops over all years in lunds dataset
    {
        double year = years1[i];
        auto find_year = std::find(years2.begin(), years2.end(), year); // tries to find if year is in the year2 vector, if not will return year2.end()
        if (find_year != years2.end())
        {
            size_t j = std::distance(years2.begin(), find_year); // This does so we know which element of the years2 vector the year is in, needed when finding the difference
            double diff = temps1[i] - temps2[j];                 // calculates the difference in temperature
            years.push_back(year);
            diffTemps.push_back(diff);
        }
    }

    if (years.empty())
    {
        std::cerr << "No overlapping years found after " << startYear << ".\n";
        return 1;
    }

    const int window = 5;                            // Calculate the moving average- in this case 5 years
    std::vector<double> movingAvg(diffTemps.size()); // creates a vector that is the same size as diffTemps
    for (size_t i = 0; i < diffTemps.size(); ++i)
    {
        int start;
        if (i >= window - 1)
        {
            start = i - (window - 1); // enough previous years for full window
        }
        else
        {
            start = 0; // not enough previous years, start from beginning
        }
        double sum = 0.0;
        int count = 0;
        for (size_t j = start; j <= i; ++j)
        {
            sum += diffTemps[j];
            count++;
        }
        movingAvg[i] = sum / count;
    }

    TCanvas *c = new TCanvas("c", "Temperature Difference on Christmas", 900, 600); // creates the canvas

    TGraph *grDiff = new TGraph(years.size(), years.data(), diffTemps.data());
    grDiff->SetLineColor(kBlack);
    grDiff->SetMarkerColor(kRed);
    grDiff->SetMarkerStyle(20);
    grDiff->SetTitle(Form("Temperature Difference (Lund - Lulea) on Christmas (from %d)", static_cast<int>(years.front()))); // This last part does so title has year that the data starts from
    grDiff->GetXaxis()->SetTitle("Year");
    grDiff->GetYaxis()->SetTitle("Temperature Difference (#circ C)");

    TGraph *grMoving = new TGraph(years.size(), years.data(), movingAvg.data());
    grMoving->SetLineColor(kBlue);
    grMoving->SetLineStyle(2);

    grDiff->Draw("ALP");
    grMoving->Draw("L SAME");

    auto legend = new TLegend(0.60, 0.15, 0.88, 0.32); // creates the legend and places it in bottom right
    legend->AddEntry(grDiff, "Yearly temperature difference", "p");
    legend->AddEntry(grMoving, "5-year moving average", "l");
    legend->Draw();

    c->SaveAs("figures/temp_diff_christmas.png");

    std::cout << "\nSaved plot to temp_diff_christmas.png\n";

    return 0;
}
