#include "filter_date.h"
#include "average_temp.h"
#include "make_lists.h"
#include "data_types.h"

#include <TGraph.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

int compare_temps()
{
    std::string csvCity1 = "~/git/MNXB11-Final-project/Preprocessed_data/Lund.csv"; // selects the correct csv files
    std::string csvCity2 = "~/git/MNXB11-Final-project/Preprocessed_data/Lulea.csv";
    std::string targetDay = "12-25"; // Christmas

    int startYear = 1949; // chose a year that both of the .csv files have

    auto dataCity1 = filterDate(csvCity1, targetDay); // filters both the .csv files to only select wanted date
    auto dataCity2 = filterDate(csvCity2, targetDay);

    auto avgCity1 = computeAveragePerDate(dataCity1); // get the average of that year
    auto avgCity2 = computeAveragePerDate(dataCity2);

    std::vector<double> years1, temps1, years2, temps2;
    makeLists(avgCity1, years1, temps1); // convert this to lists
    makeLists(avgCity2, years2, temps2);

    auto filterByYear = [startYear](std::vector<double> &years, std::vector<double> &temps)
    {
        std::vector<double> newYears, newTemps; // This function makes sure that only data after the chosen startyear is kept
        for (size_t i = 0; i < years.size(); ++i)
        {
            if (years[i] >= startYear)
            {
                newYears.push_back(years[i]);
                newTemps.push_back(temps[i]);
            }
        }
        years = std::move(newYears);
        temps = std::move(newTemps);
    };

    filterByYear(years1, temps1);
    filterByYear(years2, temps2);

    std::vector<double> years, diffTemps;
    for (size_t i = 0; i < years1.size(); ++i) // This matches the years and calculates the temperature difference
    {
        double year = years1[i];
        auto it = std::find(years2.begin(), years2.end(), year);
        if (it != years2.end())
        {
            size_t j = std::distance(years2.begin(), it);
            double diff = temps1[i] - temps2[j];
            years.push_back(year);
            diffTemps.push_back(diff);
        }
    }

    if (years.empty())
    {
        std::cerr << "No overlapping years found after " << startYear << ".\n";
        return 1;
    }

    TCanvas *c = new TCanvas("c", "Temperature Difference on Christmas", 800, 600); // creates the canvas for the graph
    TGraph *gr = new TGraph(years.size(), years.data(), diffTemps.data());
    gr->SetTitle(Form("Temperature Difference (Lund - Lulea) on Christmas (from %d)", startYear)); // sets title
    gr->GetXaxis()->SetTitle("Year");                                                              // sets xaxis title
    gr->GetYaxis()->SetTitle("Temperature Difference (#circ C)");
    gr->SetLineColor(kRed);     // does so line connecting dots is red
    gr->SetMarkerColor(kGreen); // does so the dots are green
    gr->SetMarkerStyle(20);     // filled circle
    gr->SetLineWidth(2);

    gr->Draw("ALP"); // A=axis, L=line, P=points

    c->Update();

    c->SaveAs("temp_diff_christmas.pdf");

    std::cout << "Temperature difference graph saved to temp_diff_christmas.pdf\n";

    return 0;
}
