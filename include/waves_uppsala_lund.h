#ifndef WAVES_UPPSALA_LUND_H
#define WAVES_UPPSALA_LUND_H

#include "average_temp.h"
#include "data_types.h"
#include "CSV_reader.h"
#include <string>
#include "decade_average.h"
#include "TGraph.h"
#include "events_per_year.h"
#include <vector>
#include <map>

double percentile(std::vector<double> v, double p);

TGraph *create_decade_graph(const std::map<int, double> &decade_averages);

void create_plot(const std::map<int, double> &lund_decades, const std::map<int, double> &uppsala_decades, const std::string &title, const std::string &filename);

int waves();

#endif // WAVES_UPPSALA_LUND_H
