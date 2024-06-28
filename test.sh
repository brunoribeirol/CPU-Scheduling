#!/bin/bash

# Function to run tests
run_tests() {
  command=$1
  echo "---------------------------------"
  echo "Running tests for $command"
  echo "---------------------------------"

  # Loop through the input files
  for inputfile in ./tests/$command/input/*.txt
  do
    # Extract the file number from the input file
    filenumber=$(basename $inputfile .txt)

    comparefile="./tests/$command/out/$filenumber.out"

    # Run the command on the input file
    ./$command $inputfile

    # Compare the output with the comparefile
    total_lines=$(wc -l < "$comparefile")
    diff_lines=$(diff -y --suppress-common-lines $command.out "$comparefile" | wc -l)
    same_lines=$((total_lines - diff_lines))
    
    # Calculate the similarity percentage
    if [ $total_lines -gt 0 ]
    then
      similarity=$((100 * same_lines / total_lines))
    else
      similarity=0
    fi

    if diff -a $command.out "$comparefile" &> /dev/null
    then
      echo "Test $filenumber: Passed"
      echo "---------------------------------"
    else
      echo "Test $filenumber: Failed"
      echo
      # Print the diff
      diff -a $command.out "$comparefile"
      echo
      echo "Similarity: $similarity%"
      echo "---------------------------------"
    fi
  done
}

# Run tests for rate and edf sequentially
run_tests rate
run_tests edf
