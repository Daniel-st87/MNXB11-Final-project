/*
gSystem->AddIncludePath("-I/home/marcus1/git/MNXB11-Final-project/include");
.L /home/marcus1/git/MNXB11-Final-project/src/filter_date.cxx+
.L /home/marcus1/git/MNXB11-Final-project/src/average_temp.cxx+
.L /home/marcus1/git/MNXB11-Final-project/src/make_lists.cxx+
.L compare_temps.cxx+

*/

#include "filter_date.h"
#include "average_temp.h"
#include "make_lists.h"
#include "data_types.h"

#include <TGraph.h>
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
    const char *home = getenv("HOME");
    std::string csvCity1 = std::string(home) + "/git/MNXB11-Final-project/Preprocessed_data/Lund.csv";
    std::string csvCity2 = std::string(home) + "/git/MNXB11-Final-project/Preprocessed_data/Lulea.csv";
    std::string targetDay = "12-25";

    int startYear = 1500;

    auto dataCity1 = filterDate(csvCity1, targetDay);
    auto dataCity2 = filterDate(csvCity2, targetDay);

    auto avgCity1 = calc_average_temp(dataCity1);
    auto avgCity2 = calc_average_temp(dataCity2);

    std::vector<double> years1, temps1, years2, temps2;
    makeLists(avgCity1, years1, temps1);
    makeLists(avgCity2, years2, temps2);

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
        years = std::move(newYears);
        temps = std::move(newTemps);
    };

    filterByYear(years1, temps1);
    filterByYear(years2, temps2);

    std::vector<double> years, diffTemps;
    for (size_t i = 0; i < years1.size(); ++i)
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

    const int window = 5; // Calculate the moving average- in this case 5 years
    std::vector<double> movingAvg(diffTemps.size());
    for (size_t i = 0; i < diffTemps.size(); ++i)
    {
        int start = (i >= window - 1) ? i - (window - 1) : 0;
        double localSum = 0.0;
        int count = 0;
        for (size_t j = start; j <= i; ++j)
        {
            localSum += diffTemps[j];
            count++;
        }
        movingAvg[i] = localSum / count;
    }

    TCanvas *c = new TCanvas("c", "Temperature Difference on Christmas", 900, 600);

    TGraph *grDiff = new TGraph(years.size(), years.data(), diffTemps.data());
    grDiff->SetLineColor(kBlack);
    grDiff->SetMarkerColor(kRed);
    grDiff->SetMarkerStyle(20);
    grDiff->SetLineWidth(2);
    grDiff->SetTitle(Form("Temperature Difference (Lund - Lulea) on Christmas (from %d)", static_cast<int>(years.front()))); // This last part does so title has year that the data starts from
    grDiff->GetXaxis()->SetTitle("Year");
    grDiff->GetYaxis()->SetTitle("Temperature Difference (#circ C)");

    TGraph *grMoving = new TGraph(years.size(), years.data(), movingAvg.data());
    grMoving->SetLineColor(kBlue);
    grMoving->SetLineWidth(2);
    grMoving->SetLineStyle(2);

    grDiff->Draw("ALP");
    grMoving->Draw("L SAME");

    auto legend = new TLegend(0.15, 0.75, 0.45, 0.9);
    legend->AddEntry(grDiff, "Yearly temperature difference", "lp");
    legend->AddEntry(grMoving, "5-year moving average", "l");
    legend->Draw();

    c->SaveAs("temp_diff_christmas_with_averages.pdf");

    std::cout << "\nSaved plot to temp_diff_christmas_with_averages.pdf\n";

    return 0;
}
