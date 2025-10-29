#include <corupt_data.h>

#include <iostream>
#include <fstream>
#include <string>

void corupt_data(datafile){
  // Was thinkt to be used as controllist but it is perhaps not necesarry. 
 // was thinkt to in case the temperature not is missed the year, date and temp should be putted in seperate lists but I don't know if the idea is so god as it sounds. 
     
   
    year=[];
    date =[];
    temperature = []; 
    
    for (int line : datafile);
        control =[];
        if line.empty(){continue};
        else
            {std::strtok(line, ",");
            for (int d : line)
            contrl.append(d)}; // Thinckt to split upp line to a list with elements.
        
        lisst_len= control.size()
        if lisst_len==3 {return true};  // If the line not is corupted it should have three elements.
        else if
            return false;
            
        
}

//if datafile[2] != "-" ore "0" ore "1" ore "2" ore "3" ore "5" ore "6" ore "7" ore "8" ore "9"; // check if the element start with a minus sigh ore a number
//            return false;
//        else
//            return true;    // The tinking is if it is not an temperature on the row it returns false and only if the code return true the date and temperature will be putted in the lists for plotting.



#endif //Corupt_data