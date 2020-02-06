# TDMI-Utilities
To calculate time-delayed mutual information (TDMI) for neurophysiology data, especially for spike trains and local field potentials (LFPs).
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
bin/cal-lfp			# generate lfp series from raw neural data
bin/cal-mi			# calculate tdmi between types of data
# extended module
bin/cal-lfp-simple	# generate current (LFP) data of single unit
bin/cal-cc			# cross-correlation between continuous variables
bin/cal-sta			# spike triggered average
```

Sample usage:

```bash
# calculate lfp series from raw data to [output_filename]
# with given config.ini settings
bin/cal-lfp --prefix [/path/of/data] -c [path/of/config.ini] [output_filename]
# default config.ini : doc/config_lfp_default.ini

# calculate tdmi between spike trains and LFP (-t BD),
# stored in [input_filename_1] and [input_filename2], 
# with range of time delay [-40,40] steps,
# 0.001 bin size for calculating PDF of LFP
bin/cal-mi --prefix [/path/of/data] -t BD --drange 40 40 --dx 0.001 [input_filename_1] [input_filename2] [output_filename] 
```

Use ```-h``` for more detailed help

#### Python Utilities
- Spike train generator

*Convert raw raster data to single spike train with binary data format.*

```bash
>>> python utils/spike.py -h
usage: spike.py [-h] [--id ID] [--dt DT] [-t TIME_RANGE TIME_RANGE] [-v]
                prefix raster_fname spike_fname

Single spike train generator.

positional arguments:
  prefix                working directory of source data and output data
  raster_fname          filename of raster data, *.csv
  spike_fname           filename of spike train data, *.npy

optional arguments:
  -h, --help            show this help message and exit
  --id ID               id of neuron (default: 0)
  --dt DT               time step of sampling (default: 0.5)
  -t TIME_RANGE TIME_RANGE, --time_range TIME_RANGE TIME_RANGE
                        time range of spike train (default: [0, None])
  -v, --verbose         enable verbose of running time
```

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

- LFP filter

*Filter original LFP data.*

*Optional filters: 'alpha', 'beta', 'theta', 'gamma', 'delta', 'ripple', 'lfp'*

```bash
>>> python utils/filter.py -h
usage: filter.py [-h] [-v]
                 prefix lfp_fname {alpha,beta,theta,gamma,delta,ripple,lfp}

Frequency filter for LFP signal.

positional arguments:
  prefix                working directory of source data and output data
  lfp_fname             filename of LFP data
  {alpha,beta,theta,gamma,delta,ripple,lfp}
                        frequency band

optional arguments:
  -h, --help            show this help message and exit
  -v, --verbose         enable verbose of running time

```
