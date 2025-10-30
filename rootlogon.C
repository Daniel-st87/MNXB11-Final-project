#include <TStyle.h>
#include <iostream>

void rootlogon()
{
    gStyle->SetOptStat(0); // No stat box
    gStyle->SetOptFit(1);  // Show fit results when fitting
    gStyle->SetTitleFontSize(0.045);

    gStyle->SetTitleSize(0.05, "x"); // Use bigger text on the axes
    gStyle->SetTitleSize(0.05, "y");
    gStyle->SetLabelSize(0.05, "x"); // Makes the labels larger
    gStyle->SetLabelSize(0.05, "y");
    gStyle->SetLineWidth(2);
    gStyle->SetPadTopMargin(0.05); // Change the margins to fit our new sizes
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadBottomMargin(0.16);
    gStyle->SetPadLeftMargin(0.16);

    std::cout << "[rootlogon.C] Custom ROOT style loaded.\n";
}
