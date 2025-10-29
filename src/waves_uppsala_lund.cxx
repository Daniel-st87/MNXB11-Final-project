#include "average_temp.h"
#include "data_types.h"
#include "CSV_reader.h"
#include "decade_average.h"
#include "events_per_year.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <cstdlib>

#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TF1.h"

// calculate the percentiles
double percentile(std::vector<double> v, double p)
{
    if (v.size() == 0)
    {
        return 0.0;
    }

    std::sort(v.begin(), v.end());
    double x = p * (v.size() - 1);
    int i = (int)x;
    int j = i + 1;

    if (j >= (int)v.size())
    {
        j = (int)v.size() - 1;
    }

    double frac = x - i;
    double val = v[i] + frac * (v[j] - v[i]);
    return val;
}

// Create graph from decade averages
TGraph *create_decade_graph(const std::map<int, double> &decade_averages)
{
    int n_points = decade_averages.size();
    TGraph *g = new TGraph(n_points);
    int point_index = 0;

    for (std::map<int, double>::const_iterator it = decade_averages.begin(); it != decade_averages.end(); ++it)
    {
        int decade_start = it->first;
        double average_events = it->second;
        double decade_midpoint = decade_start + 5.0; // Middle of decade

        g->SetPoint(point_index, decade_midpoint, average_events);
        point_index++;
    }

    return g;
}

// Create the plot
void create_plot(const std::map<int, double> &lund_decades, const std::map<int, double> &uppsala_decades, const std::string &title, const std::string &filename)
{

    TCanvas *c = new TCanvas("c", "c", 1000, 600);
    TGraph *gL = create_decade_graph(lund_decades);
    TGraph *gU = create_decade_graph(uppsala_decades);

    gL->SetTitle((title + ";Decade;Mean count per year").c_str());
    gL->SetMarkerStyle(20);
    gL->SetMarkerColor(kBlue);
    gL->SetLineColor(kBlue);
    gU->SetMarkerStyle(21);
    gU->SetMarkerColor(kRed);
    gU->SetLineColor(kRed);

    gL->Draw("AP");
    gU->Draw("P SAME");

    TF1 fL("fL", "pol1");
    TF1 fU("fU", "pol1");
    fL.SetLineColor(kBlue);
    fL.SetLineWidth(2);
    fU.SetLineColor(kRed);
    fU.SetLineWidth(2);

    if (gL->GetN() > 1)
    {
        gL->Fit(&fL, "Q");
        fL.Draw("SAME");
    }

    if (gU->GetN() > 1)
    {
        gU->Fit(&fU, "Q");
        fU.Draw("SAME");
    }

    TLegend *leg = new TLegend(0.12, 0.75, 0.70, 0.90);
    leg->AddEntry(gL, "Lund", "p");
    leg->AddEntry(gU, "Uppsala", "p");

    if (gL->GetN() > 1)
    {
        leg->AddEntry(&fL, "Lund trend", "l");
    }

    if (gU->GetN() > 1)
    {
        leg->AddEntry(&fU, "Uppsala trend", "l");
    }

    leg->Draw();

    c->SaveAs(filename.c_str());

    delete c;
    delete gL;
    delete gU;
}

int waves() // changed this as main() should only be in the main.cxx file
{
    std::string lundPath = "Preprocessed_data/Lund.csv";
    std::string uppPath = "Preprocessed_data/Uppsala.csv";

    // Read data
    std::vector<DataRow> lrows = readCSV(lundPath);
    std::vector<DataRow> urows = readCSV(uppPath);

    // Convert to DataRow
    std::vector<DataRow> ldata, udata;
    for (size_t i = 0; i < lrows.size(); i++)
    {
        DataRow r;
        r.date = lrows[i].date;
        r.time = lrows[i].time;
        r.temperature = lrows[i].temperature;
        r.sourceFile = lundPath;
        ldata.push_back(r);
    }
    for (size_t i = 0; i < urows.size(); i++)
    {
        DataRow r;
        r.date = urows[i].date;
        r.time = urows[i].time;
        r.temperature = urows[i].temperature;
        r.sourceFile = uppPath;
        udata.push_back(r);
    }

    // Daily averages
    std::map<std::string, double> lDaily = calc_average_temp(ldata);
    std::map<std::string, double> uDaily = calc_average_temp(udata);

    // Calculate thresholds
    std::vector<double> lv, uv;
    for (std::map<std::string, double>::const_iterator it = lDaily.begin(); it != lDaily.end(); ++it)
    {
        lv.push_back(it->second);
    }

    for (std::map<std::string, double>::const_iterator it = uDaily.begin(); it != uDaily.end(); ++it)
    {
        uv.push_back(it->second);
    }

    double heatThrL = percentile(lv, 0.90);
    double heatThrU = percentile(uv, 0.90);
    double coldThrL = percentile(lv, 0.10);
    double coldThrU = percentile(uv, 0.10);
    int minDays = 3;

    // Count events and calculate decades
    auto lund_heat_counts = count_events_per_year(lDaily, heatThrL, minDays, true);
    auto uppsala_heat_counts = count_events_per_year(uDaily, heatThrU, minDays, true);
    auto lund_cold_counts = count_events_per_year(lDaily, coldThrL, minDays, false);
    auto uppsala_cold_counts = count_events_per_year(uDaily, coldThrU, minDays, false);

    auto lund_heat_decades = calculate_decade_averages(lund_heat_counts);
    auto uppsala_heat_decades = calculate_decade_averages(uppsala_heat_counts);
    auto lund_cold_decades = calculate_decade_averages(lund_cold_counts);
    auto uppsala_cold_decades = calculate_decade_averages(uppsala_cold_counts);

    // Create plots
    create_plot(lund_heat_decades, uppsala_heat_decades, "Heatwaves per Decade", "figures/heatwaves_decadal.png");
    create_plot(lund_cold_decades, uppsala_cold_decades, "Coldwaves per Decade", "figures/coldwaves_decadal.png");

    return 0;
}
