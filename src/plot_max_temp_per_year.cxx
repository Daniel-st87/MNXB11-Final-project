#include <iostream>
#include <map>
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TF1.h"
#include "TStyle.h"

// Makes a plot of the hottest average temperature each year
void plotMaxTempPerYear(const std::map<int, double> &yearlyMax)
{
    // Make a graph (x = year, y = temp)
    TGraph *g = new TGraph(yearlyMax.size());

    int i = 0;
    for (const auto &entry : yearlyMax)
    {
        g->SetPoint(i, entry.first, entry.second); // add a point
        i++;
    }

    // Create a window for the plot
    TCanvas *c = new TCanvas("c", "Max Average Temperature per Year", 900, 600);

    // Title and axis labels
    g->SetTitle("Soderarm: Hottest Average Temperature per Year;Year;Temperature [#circC]");

    // Style for points and line
    g->SetMarkerStyle(20);
    g->SetMarkerColor(kBlue + 2);
    g->SetLineColor(kBlue);

    // Draw graph with axes, points, and line
    g->Draw("APL");

    // Fit a straight line (trend)
    TF1 *fit = new TF1("fit", "pol1"); // y = a + b*x
    g->Fit(fit, "Q");                  // quiet fit
    fit->SetLineColor(kRed);

    // Print slope value
    std::cout << "Linear fit slope (°C/year): " << fit->GetParameter(1) << std::endl;

    // Save image of plot
    c->SaveAs("figures/Soderarm_hottest_yearly_day.png");

    // Prints that the photo is saved
    std::cout << "Plot saved as Soderarm_hottest_yearly_day.png in figures directory" << std::endl;
}
