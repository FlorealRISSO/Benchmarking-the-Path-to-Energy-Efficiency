# Project README

Welcome to the project repository! This repository contains scientific article
sources, program source code, and scripts for data analysis related to a
specific scientific study. Below is a guide on how to navigate and use the
various components of this project.

## Table of Contents
- [Article](#article)
- [Programs](#programs)
- [Scripts](#scripts)
- [Usage](#usage)
  - [Configuring Parameters](#configuring-parameters)
  - [Building Executables](#building-executables)
  - [Setting CPU Frequency](#setting-cpu-frequency)
  - [Running Benchmarks](#running-benchmarks)
  - [Plotting Data](#plotting-data)
  - [Single Execution Measurement](#single-execution-measurement)
  - [Frequency-based Benchmark](#frequency-based-benchmark)
- [Dependencies](#dependencies)


## 1. Article
The `article` directory contains the sources for the scientific article. To generate the article PDF, navigate to the `article` directory and run:
```bash
cd article
make
```
This will generate the `article.pdf` in the `build` directory.

## 2. Programs
The `prog` directory contains source code for different programs. To build the binaries, edit the `cons.h` file to configure parameters, then run:
```bash
cd prog
make
```
This will compile and generate binaries in the `build` directory.

## 3. Scripts
The `scripts` directory contains various scripts for data analysis. To perform specific tasks, follow the instructions below:
- To set CPU frequency, run `set-frequency.sh`.
- To run benchmarks on all executables, use `benchmark-exe.sh`.
- To plot data, run `preprocess.sh` and then execute `plot.py`.
- For a single execution measurement, use `audit.sh`.
- To test one binary with varying frequencies, use `benchmark-freq.sh`.

## 4. Usage

### 4.1 Configuring Parameters
To configure the parameters of the program, edit the `cons.h` file in the `prog` directory before building the binaries.

### 4.2 Building Executables
Compile the program binaries by navigating to the `prog` directory and running:
```bash
cd prog
make
```

### 4.3 Setting CPU Frequency
Adjust the CPU frequency by executing the following command:
```bash
cd scripts
./set-freq.sh <frequency>
```

### 4.4 Running Benchmarks
To run benchmarks on all built executables from a folder, use the following command:
```bash
cd scripts
./benchmark-exe.sh <folder> <iterations>
```

### 4.5 Plotting Data
Generate plots from benchmark data by executing the following commands:
(The timestamp in `juliaM25620231220_105438.log` is `20231220_105438`)
```bash
cd scripts
./preprocess.sh <timestamps>
./plot.py <timestamps>
```

### 4.6 Single Execution Measurement
Measure the execution time of one binary with the following command:
```bash
./audit.sh <path-to-binary>
```

### 4.7 Frequency-based Benchmark
Test one binary with varying CPU frequencies using the following command:
```bash
./scripts/benchmark-freq.sh <path-to-binary>
```
## 5. Dependencies

To ensure the proper functioning of the project and its associated scripts, it is important to have the following dependencies installed on your system.

### 5.1 Python Dependencies
- **sys:** Standard Python library. No additional installation required.

- **pandas:**
  ```bash
  pip install pandas
  ```

- **matplotlib:**
  ```bash
  pip install matplotlib
  ```

### 5.2 C Dependencies
- **OpenMP:**
Ensure your C compiler supports OpenMP. For GCC, OpenMP is typically included.
For other compilers, please refer to the respective documentation.

### 5.3 Benchmark Dependencies
- **mojitos:**
The project relies on the Mojitos benchmark tool for performance testing.
Follow the installation instructions provided by Mojitos: 
[MojitosGitHubRepository](https://gitlab.irit.fr/sepia-pub/mojitos)

Make sure to install these dependencies before running any scripts or compiling
the C programs. If you encounter any issues related to dependencies, consult
the documentation or community forums of the respective tools for assistance.

Feel free to explore and contribute to this project! If you encounter any
issues or have suggestions, please open an issue or submit a pull request.

