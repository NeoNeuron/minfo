# TDMI-Utilities
To calculate time-delayed mutual information (TDMI) for neurophysiology data, especially for spike trains and local field potentials (LFPs).
## Repository hierarchy
- doc: Necessary documents and configuration files;
- include: C/C++ header files;
- src: C/C++ source files;
- pys: python scripts for example and automating tests;
- test: test codes;

## Get Started
Currnetly, there are four distinct modules here, including

```bash
# basic modules
bin/cal-spike	# convert single spike train to binary series from raw neural data
bin/cal-lfp		# generate lfp series from raw neural data
bin/cal-mi		# calculate tdmi between the data with preprocessed by previous two modules
# extended module
bin/cal-lfp-simple	# generate simplified version of lfp data with given target neurons
```
### Compile

```bash
make 					# for three basic modules
make bin/cal-lfp-simple	# for extended module
```
### How to use

```bash
# convert binary spike train of No. 1 (-I index)
# from [raw_data] to [output_filename] with time range (5e2, 1e5)ms,
# sampling step 0.5 ms
bin/cal-spike --prefix [/path/of/data] [raw_data] [output_filename] -I 1 -t 5e2 1e5 --dt 0.5 [-f]

# calculate lfp series from raw data to [output_filename]
# with given config.ini settings
bin/cal-lfp --prefix [/path/of/data] [path/of/config.ini] [output_filename]
# default config.ini : doc/config_lfp_default.ini

# calculate tdmi between spike trains and LFP (-t BD),
# stored in [input_filename_1] and [input_filename2], 
# with range of time delay [-40,40] steps,
# 0.001 bin size for calculating PDF of LFP
bin/cal-mi --prefix [/path/of/data] -t BD --drange 40 40 --dx 0.001 [input_filename_1] [input_filename2] [output_filename] 
```

Use ```-h``` for more detailed help
