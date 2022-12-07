# Mutual Information Estimator

*Python-based mutual information estimator, designed for multiple data types.*

## Prerequisite

- [Numpy](https://numpy.org)
- [Cython](https://cython.readthedocs.io/en/stable/index.html)
- [Numba](https://numba.pydata.org)

```bash
# Make sure install numpy using conda. 
# Installation with pip may lead to unexpected errors.
conda install numpy cython numba
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

## Get Started

### MI with uniform bins 

```python
from minfo.mi_float import mutual_info, TDMI
# mutual_info : mutual information estimator
#        tdmi : time-delayed mutual information estimator
n = 100
x = np.random.rand(n)
y = np.random.rand(n)

# compute mutual information
mutual_info(x,y, bins=50) # default algorithm: 'uniform'

# compute time-delayed mutual information (Parallel accelerated)
n_delay = 10
TDMI(x, y, n_delay, bins=50) # default algorithm: 'uniform'
```

### MI with adaptive bins

**ATTENTION:** This algorithm is designed for mutual information estimation between continuous variables. Applying it to discrete variables with few number of values might lead to large deviations. Modules for finite discrete cases are to be developed. 

**Reference**

- Darbellay, G. A., & Vajda, I. (1999). [Estimation of the information by an adaptive partitioning of the observation space. IEEE Transactions on Information Theory](https://ieeexplore.ieee.org/document/761290), 45(4), 1315-1321.

```python
from minfo.mi_float import mutual_info, TDMI
# mutual_info : mutual information estimator
#        tdmi : time-delayed mutual information estimator
n = 100
x = np.random.rand(n)
y = np.random.rand(n)

# compute mutual information
mutual_info(x,y, algorithm='adaptive')

# compute time-delayed mutual information (Parallel accelerated)
n_delay = 10
TDMI(x, y, n_delay, algorithm='adaptive')
```

## Compare performance with pure Python version

**Test OS info:**
- **Laptop:** *MacBook Pro (15-inch, 2018)*
- **System** *version: macOS Big Sur 12.4 (21F79)*
- **CPU:** *2.6 GHz 6-Core Intel Core i7*
- **RAM:** *32 GB 2400 MHz DDR4*

```bash
$ cd example
$ python example.py
[INFO]:   mi_adaptive (python)  takes 0.025 s
[INFO]:   mi_uniform  (numba)   takes 0.000 s
[INFO]:   mi_adaptive (cython)  takes 0.009 s
[INFO]: tdmi_adaptive (python)  takes 0.560 s
[INFO]: tdmi_uniform  (numba)   takes 0.005 s
[INFO]: tdmi_adaptive (cython)  takes 0.076 s
```
