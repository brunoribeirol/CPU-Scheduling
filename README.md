<p align="center">
  <img
    src="https://img.shields.io/badge/Status-Finished%20-green?style=flat-square"
    alt="Status"
  />
</p>

<p align="center">
  <img
    src="https://img.shields.io/github/repo-size/brunoribeirol/CPU-Scheduling?style=flat"
    alt="Repository Size"
  />
  <img
    src="https://img.shields.io/github/languages/count/brunoribeirol/CPU-Scheduling?style=flat&logo=python"
    alt="Language Count"
  />
  <img
    src="https://img.shields.io/github/commit-activity/t/brunoribeirol/CPU-Scheduling?style=flat&logo=github"
    alt="Commit Activity"
  />
  <a href="LICENSE.md"
    ><img
      src="https://img.shields.io/github/license/brunoribeirol/CPU-Scheduling"
      alt="License"
  /></a>
</p>

# Real-time CPU Scheduling Simulation

## 👀 Overview

This project implements a real-time process scheduling simulator using the Rate-Monotonic (rate) and Earliest-Deadline-First (edf) algorithms for scheduling strategies. The program reads a set of tasks as input. Then it simulates the execution of these tasks in a real-time environment, following either the `rate` or `edf` scheduling algorithms.

## 📬 Input File Format

The input file should have the following format:

    [TOTAL TIME]
    [TASK NAME 1] [PERIOD 1] [CPU BURST 1]
    [TASK NAME 2] [PERIOD 2] [CPU BURST 2]
    ...
    [TASK NAME n] [PERIOD n] [CPU BURST n]


- `[TOTAL TIME]`: Total simulation time.
  - Should be an integer 
- `[TASK NAME i]`: Name of the i-th task.
- `[PERIOD i]`: Period of the i-th task.
  - Should be an integer
- `[CPU BURST i]`: CPU burst time for the i-th task.
  - Should be an integer               

## 🪛 Build Instructions

1. Ensure you are on a Linux, Unix, or macOS system.
2. Compile using `make` with the desired algorithm name as a parameter:
   - Rate-Monotonic:
     - `make rate`
   - Earliest-Deadline-First:
     - `make edf`
   
## 📎Execution

Run the compiled executable with the input file:
- For Rate-Monotonic:
  - `./rate [input_file]`
- For Earliest-Deadline-First:
  - `./edf [input_file]`

## 📜 Specifications and Notes

1. The program is written in C and compiled using a Makefile.
3. Input files must not have a blank line at the end, and each line must not have extra spaces between task parameters.
4. Tasks described in the input file arrive simultaneously at time 0 for execution.
6. Priorities are resolved using the FCFS algorithm in case of tie.
8. If a task finishes exactly when a higher-priority task arrives, the lower-priority task is considered finished.
9. Tasks arriving exactly at the simulation end are marked as KILLED.
11. Zero CPU bursts or missed tasks with zero execution units do not need to be displayed.
12. Priority calculation for rate and edf algorithms is based on the specified task period as the deadline.

## 📩 Example Input

For example, given an input file named `input.txt` with the following content:

    165
    T1 50 25
    T2 80 35

## 📦 Output File

The output file should be names as `algorithm.out`, where `algorithm` is the schedulling strategy addopted (`rate` or `edf`). The format follows strict guidelines as provided in the assignment.

    EXECUTION BY RATE
    [T1] for 25 units - F
    [T2] for 25 units - H
    [T1] for 25 units - F
    [T2] for 5 units - L
    [T2] for 20 units - H
    [T1] for 25 units - F
    [T2] for 15 units - F
    idle for 10 units
    [T1] for 15 units - K
    LOST DEADLINES
    [T1] 0
    [T2] 1
    COMPLETE EXECUTION
    [T1] 3
    [T2] 1
    KILLED
    [T1] 1
    [T2] 1
