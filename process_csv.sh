#!/bin/bash

# How to use: ./process_csv.sh inputfilename outputfilename startingrow col1 col2 col3
# Example: ./process_csv.sh data.csv processed.csv 25 2 5 7

# Arguments
INPUT_FILE="$1"
OUTPUT_FILE="$2"
START_LINE="$3"   # the number of the column where the important data starts
COL1="$4"         # first column to extract
COL2="$5"         # second column
COL3="$6"         # third column

#Check to make sure that all of the arguments are provided
if [ $# -ne 6 ]; then
    echo "Usage: $0 input.csv output.csv start_line col1 col2 col3"
    exit 1
fi

# Process the file
tail -n +"$START_LINE" "$INPUT_FILE" | awk -F';' -v c1="$COL1" -v c2="$COL2" -v c3="$COL3" '{print $c1 "," $c2 "," $c3}' > "$OUTPUT_FILE"

echo "Processed CSV saved to $OUTPUT_FILE"
