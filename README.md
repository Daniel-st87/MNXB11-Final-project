# MNXB11-Final-project
Final Project for the MNXB11 Course

How to reproduce the figures:

To remove the metadata from the datasets, the process_csv.sh bash script is used. To use it, enter this command into the console: 
./process_csv.sh inputfilename outputfilename startingrow col1 col2 col3
Where the inputfilename is the name of the CSV file that you want to remove the metadata from, startingrow is the number of the first row that does not have any metadata, and col1 is the first column that contains data that you want to keep in the new processed CSV file.

After you have the preprocessed data, run:

make
./main

This will create the 4 figures, which will be in the figures directory.

