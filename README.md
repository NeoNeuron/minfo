# Mutual Information Estimator

## Adaptive partitioning algorithm for mutual information estimation

Python(Cython)-based mutual information estimator with adaptive partitioning strategy.

**ATTENTION:** This algorithm is designed for mutual information estimation between continuous variables. Applying it to discrete variables with few number of values might lead to large deviations. Modules for finite discrete cases are to be developed. 

## Reference

- Darbellay, G. A., & Vajda, I. (1999). [Estimation of the information by an adaptive partitioning of the observation space. IEEE Transactions on Information Theory](https://ieeexplore.ieee.org/document/761290), 45(4), 1315-1321.

## Prerequisite

- numpy
- cython

```bash
# Make sure install numpy using conda. 
# Installation with pip may lead to unexpected errors.
conda install numpy cython 
```


> **Install OpenMP Library** \
> *Install OpenMP Library to enable CPU parallel acceleration for Time-delayed Mutual Information calculation.*
> > Linux:
> > ```
> > sudo apt-get install libomp-dev
> > ```
> > MacOS:
> > ```
> > brew install libomp
> >```
> 

## Installation

### To install via pip:

```bash
pip install minfo
```

### To install via repository:

```bash
git clone https://github.com/NeoNeuron/minfo
cd minfo
pip install -e .
```

## Example

```python
from minfo.mi_float import mutual_info, tdmi, tdmi_omp
# mutual_info := mutual information estimator
# tdmi := time-delayed mutual information estimator
# tdmi_omp := TDMI estimator with openMP accelerated
n = 100
x = np.random.rand(n)
y = np.random.rand(n)

# compute mutual information
mutual_info(x,y)

# compute time-delayed mutual information
n_delay = 10
tdmi(x, y, n_delay)

# OpenMP accelerated version
tdmi_omp(x, y, n_delay)
```

## Compare performance with pure Python version

**Test OS info:**
- **Laptop:** *MacBook Pro (15-inch, 2018)*
- **System** *version: macOS Big Sur 11.2 (20D64)*
- **CPU:** *2.6 GHz 6-Core Intel Core i7*
- **RAM:** *32 GB 2400 MHz DDR4*

```bash
$ cd example
$ python example.py
[INFO]:   mi (cython)        takes 0.024 s
[INFO]: tdmi (cython)        takes 0.435 s
[INFO]: tdmi (cython/OpenMP) takes 0.089 s
[INFO]:   mi (python)        takes 0.434 s
[INFO]: tdmi (python)        takes 0.865 s
```

---
---

# MI Uniform

To calculate time-delayed mutual information (TDMI) for neurophysiology data.
## Repository hierarchy
- doc: Necessary documents and configuration files;
- include: C/C++ header files;
- src: C/C++ source files;
- external: third-party modules;
- test: test codes;
- utils: utilities for data processing, developed in Python;

## Get Started
### Installation

1. Build external `cnpy` library
	
	```bash
	mkdir build
	cd build
	cmake ../external/cnpy -DCMAKE_INSTALL_PREFIX=../
	make
	make install
	```

2. Build main modules

	```bash
	cd ..
	make
	```
	
### How to use
Currnetly, there are five distinct modules here, including

```bash
# basic modules
bin/cal-mi			# calculate tdmi between types of data
# extended module
bin/cal-cc			# cross-correlation between continuous variables
bin/cal-sta			# spike triggered average
```

Sample usage:

```bash
# calculate tdmi between spike trains and LFP (-t BD),
# stored in [input_filename_1] and [input_filename2], 
# with range of time delay [-40,40] steps,
# 0.001 bin size for calculating PDF of LFP
bin/cal-mi --prefix [/path/of/data] -t BD --drange 40 40 --dx 0.001 [input_filename_1] [input_filename2] [output_filename] 
```

Use ```-h``` for more detailed help

#### Python Utilities
- Python TDMI calculator

*For quick test and small-sized calculation.*

*10 times slower than c++ version*

```bash
>>> python utils/info.py -h
usage: info.py [-h] [-v] prefix spike_fname lfp_fname tdmi_fname bins ndelays

Calculate TDMI script.

positional arguments:
  prefix         working directory of source data and output data
  spike_fname    filename of spike train data, *.npy
  lfp_fname      filename of LFP data, *.npy
  tdmi_fname     filename of output TDMI data, *.csv
  bins           number of bins in LFP histogram
  ndelays        number of delayed bins in TDMI calculation

optional arguments:
  -h, --help     show this help message and exit
  -v, --verbose  enable verbose of running time

```
