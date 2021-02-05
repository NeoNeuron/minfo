# Mutual Information Estimator

## Adaptive partitioning algorithm for mutual information estimation

Python implementation of mutual information estimation, where adaptive partitioning strategy is applied.

**ATTENTION:** This algorithm is designed for mutual information estimation between continuous variables. Applying it to discrete variables with few number of values might lead to large deviations. Modules for finite discrete cases are to be developed. 

## Reference
- Darbellay, G. A., & Vajda, I. (1999). Estimation of the information by an adaptive partitioning of the observation space. IEEE Transactions on Information Theory, 45(4), 1315-1321.
 
## Dependence:
- numpy
- cython

## Compile Cython library
```bash
python setup.py build_ext --inplace
```

## Example
```python
from mutual_info_cy import mutual_info, tdmi
n = 100
x = np.random.rand(n)
y = np.random.rand(n)

# compute mutual information
mutual_info(x,y)

# compute time-delayed mutual information
n_delay = 10
tdmi(x, y, n_delay)
```

## Compare performance with pure Python version

**Test OS info:**
- **Laptop:** *MacBook Pro (15-inch, 2018)*
- **System** *version: macOS Big Sur 11.2 (20D64)*
- **CPU:** *2.6 GHz 6-Core Intel Core i7*
- **RAM:** *32 GB 2400 MHz DDR4*

```bash
python mi_test.py
[INFO]: mi (cython) takes 0.025 s
[INFO]: tdmi (cython) takes 0.101 s
[INFO]: mi (python) takes 0.369 s
[INFO]: tdmi (python) takes 0.477 s
```

## TODO
- [x] Accelerate it with C/C++.
