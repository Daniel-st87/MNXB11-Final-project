# MNXB11 Final Project

Final project for the MNXB11 course.

## Requirements

To run this code, it is required to have an environment with **C++** and **ROOT** installed.

To start the provided development container, run:

```bash
startmnxb11container.sh bash
```

The bash argument starts an interactive shell inside the container.
This assumes that the `.bash_profile` has already been edited to include the path to the script.

## Creating the Preprocessed CSV files

First, the .CSV files in the `Preprocessed_data` directory have to be created. The files in this directory have the same data as the ones from SMHI, but do not include the metadata. These are created using the bash script called `process_csv.sh` as well as the original data in the `datasets` directory. To use the bash script, enter this command into the console:

```bash
./process_csv.sh inputfilename outputfilename startingrow col1 col2 col3
```

- `inputfilename`: the name of the CSV file that you want to remove the metadata from.
- `outputfilename`: the path and name of the new CSV file without the metadata.
- `startingrow`: the number of the first row after the metadata.
- `col1 col2 col3`: the indices of the columns that you want to keep.

An example of this for Lund would be:

```bash
./process_csv.sh datasets/smhi-opendata_1_53430_20231007_155558_Lund.csv Preprocessed_data/Lund.csv 14 1 2 3
```

## Compiling and Running the Project
After you have the preprocessed data, compile and run the project using:

```bash
make
./main
```
This will create the 4 figures, which will be in the figures directory.


