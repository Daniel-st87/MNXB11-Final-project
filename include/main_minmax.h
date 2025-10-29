
#ifndef MAIN_MINMAX_H
#define MAIN_MINMAX_H

#include <fstream>
#include <string>
#include <vector>
#include <TH1.h>
#include "data_types.h"
#include "temp_analysis.h"

void FitGaussian(TH1 *hist, const std::string &label, int shiftBins = 100);

void minmax();

#endif