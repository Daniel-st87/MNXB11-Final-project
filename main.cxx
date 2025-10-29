#include "average_temp.h"
#include "filter_date.h"
#include "events_per_year.h"
#include "decade_average.h"
#include "compare_temps.h"
#include "make_lists.h"
#include "max_temp_per_year.h"
#include "waves_uppsala_lund.h"
#include "temp_analysis.h"
#include "main_minmax.h"
#include "main_max_temp.h"

#include <iostream>

int main()
{
    std::cout << "Generating graphs..." << std::endl;

    compare_temps();     // For temps between two cities
    //minmax();            // For min and max temp
    waves();             // for the heat waves figures
    main_max_Soderarm(); // For creating the max temps in Soderarm

    std::cout << "All graphs completed!" << std::endl;
    return 0;
}
