#include <iostream>
#include <map>
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TF1.h"
#include "TStyle.h"


// Function to plot max temperatures per year
void plotMaxTempPerYear(const std::map<int, double> &yearlyMax)
{
    // Create a TGraph (x = year, y = max temp)
    TGraph *g = new TGraph(yearlyMax.size());

    int i = 0;
    for (const auto &entry : yearlyMax)
    {
        g->SetPoint(i, entry.first, entry.second); // (year, temp)
        i++;
    }

    // Create a window that shows the plot, a canvas
    TCanvas *c = new TCanvas("c", "Max Average Temperature per Year", 900, 600);

    // Making the graph's title and labelling the axes
    g->SetTitle("Halmstad: Hottest Average Temperature per Year;Year;Temperature [#circC]");

    // Customize point style
    g->SetMarkerStyle(20);
    g->SetMarkerColor(kBlue + 2);
    g->SetLineColor(kBlue);

    // Draw the plot
    g->Draw("APL"); // A = axes, P = points, L = line

    // Add a linear trend line (to show warming trend)
    TF1 *fit = new TF1("fit", "pol1"); // y = p0 + p1*x
    g->Fit(fit, "Q");                  // "Q" = quiet fit
    fit->SetLineColor(kRed);

    // Show slope in terminal
    std::cout << "Linear fit slope (°C/year): " << fit->GetParameter(1) << std::endl;

    // Save the plot as an image
    c->SaveAs("max_temp_per_year_plot.png");

    std::cout << "Plot saved as max_temp_per_year_plot.png" << std::endl;
}
