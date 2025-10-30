#include "TCanvas.h"
#include "TH1I.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TFile.h"
#include "TF1.h"
#include "TMath.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "data_types.h"
#include "average_temp.h"
#include "temp_analysis.h"

// Fit a histogram with a Gaussian and print the parameters
void FitGaussian(TH1 *hist, const std::string &label, int shiftBins = 100)
{
    if (!hist || hist->GetNbinsX() <= 0 || hist->GetEntries() == 0)
    {
        std::cout << "Histogram empty or invalid for " << label << "\n";
        return;
    }

    // Empty clone to hold the rotated contents
    const int N = hist->GetNbinsX(); // N is a total number of bins (366)
    TH1 *hShift = (TH1 *)hist->Clone((label + "_shift").c_str());
    hShift->Reset("ICES"); // reset contents

    // Rotate by bin indices around bin 100 (between winter and summer)
    for (int i = 1; i <= N; ++i)
    {
        double y = hist->GetBinContent(i);
        if (y == 0.0)
            continue;
        int j = i + shiftBins;
        while (j > N)
            j -= N;
        // set shifted content
        hShift->SetBinContent(j, y);
    }

    // Fit the rotated histogram with sensible bounds near its peak
    int imax = hShift->GetMaximumBin();
    double x0 = hShift->GetBinCenter(imax);
    double halfWin = 80.0; // adjust if needed

    static int uid = 0;
    TF1 *gausFit = new TF1(Form("gausFit_rot_%d", ++uid), "gaus", x0 - halfWin, x0 + halfWin);
    gausFit->SetParameters(hShift->GetMaximum(), x0, 25.0); // A, mu, sigma
    gausFit->SetParLimits(1, x0 - 40.0, x0 + 40.0);         // clamp mean
    gausFit->SetParLimits(2, 1.0, 80.0);                    // clamp sigma

    hShift->Fit(gausFit, "RQ0"); // quiet, use range, don't draw rotated one

    // Extract parameters and map the mean back to original axis
    double A = gausFit->GetParameter(0);
    double muRot = gausFit->GetParameter(1);
    double sigma = gausFit->GetParameter(2);

    // Shift mean back to original bins: muOrig = muRot - shift
    double muOrig = muRot - shiftBins;
    while (muOrig <= 0.0)
        muOrig += N;
    while (muOrig > N)
        muOrig -= N;

    // Convert bin-space mean to x
    double xmin = hist->GetXaxis()->GetXmin();
    double xmax = hist->GetXaxis()->GetXmax();
    double meanX = muOrig;

    // Build a Gaussian on the ORIGINAL axis with same amp & sigma, shifted-back mean
    TF1 *gausBack = new TF1(Form("gausFit_back_%d", uid), "gaus", xmin, xmax);
    gausBack->SetParameters(A, meanX, sigma);
    gausBack->SetLineColor(kBlack);
    gausBack->SetLineWidth(2);
    gausBack->Draw("SAME");
    // Second Gaussian with same parameters in order to see the tail of the cold days distribution
    TF1 *gausBack2 = new TF1(Form("gausFit_back_%d", uid), "gaus", xmin, xmax);
    gausBack2->SetParameters(A, meanX + 360, sigma);
    gausBack2->SetLineColor(kBlack);
    gausBack2->SetLineWidth(2);
    gausBack2->Draw("SAME");

    // Print parameters in the ORIGINAL coordinate system
    std::cout << "\n=== Gaussian fit for " << label << " ===\n";
    std::cout << "Most probable DOY: " << meanX << "\n";
    std::cout << "Sigma:             " << sigma << "\n";
}

void minmax()
{
    std::ifstream csv("Preprocessed_data/Lund.csv");

    std::vector<DataRow> rows;
    std::string line;

    // If your CSV has a header, skip it:
    std::streampos start = csv.tellg();
    if (std::getline(csv, line))
    {
        if (!(line.size() >= 4 && std::isdigit(static_cast<unsigned char>(line[0]))))
        {
            // header consumed
        }
        else
        {
            // no header → rewind and process from first line
            csv.clear();
            csv.seekg(start);
        }
    }

    while (std::getline(csv, line))
    {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string date, time, tempStr;

        if (!std::getline(ss, date, ','))
            continue;
        if (!std::getline(ss, time, ','))
            continue;
        if (!std::getline(ss, tempStr, ','))
            continue;

        if (!tempStr.empty() && tempStr.back() == '\r')
            tempStr.pop_back();
        try
        {
            double t = std::stod(tempStr);
            rows.push_back({date, time, t, "Lund.csv"});
        }
        catch (...)
        {
            // skip malformed line
        }
    }

    auto daily = calc_average_temp(rows);
    auto byYear = find_hottest_coldest_by_year(daily);

    // Create two histograms for hottest/coldest day-of-year
    TH1I *hHot = new TH1I("hHot",
                          "Hottest day of each year;Day of year;Entries", 366, 0.5, 366.5);
    TH1I *hCold = new TH1I("hCold",
                           "Coldest day of each year;Day of year;Entries", 366, 0.5, 366.5);

    // Fill histograms using your per-year data
    for (const auto &kv : byYear)
    {
        const auto &ex = kv.second;
        if (ex.initialized)
        {
            hHot->Fill(ex.hottest.doy);
            hCold->Fill(ex.coldest.doy);
        }
    }

    // Styling and visualization
    gStyle->SetOptStat(0); // disable stats box
    TCanvas *c = new TCanvas("c", "Hottest vs Coldest DOY", 900, 600);

    hHot->SetFillColor(kRed + 1);
    hHot->SetLineColor(kRed + 2);
    hCold->SetFillColor(kBlue + 1);
    hCold->SetLineColor(kBlue + 2);

    // Draw histograms
    hHot->Draw();        // draw first
    hCold->Draw("SAME"); // overlay second

    FitGaussian(hHot, "Hottest Day of Year");
    FitGaussian(hCold, "Coldest Day of Year");

    // Add a legend
    TLegend *leg = new TLegend(0.70, 0.75, 0.90, 0.90);
    leg->AddEntry(hHot, "Hottest DOY", "f");
    leg->AddEntry(hCold, "Coldest DOY", "f");
    leg->Draw();

    // Save as PNG and ROOT file
    c->SaveAs("figures/hot_cold_doy.png");
}
