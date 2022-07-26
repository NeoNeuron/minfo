import numpy as np
from minfo.mi_float import mutual_info as mi_cy
from minfo.mi_float import TDMI as TDMI_cy
from mutual_info import mutual_info as mi_py
import time

def TDMI_py(dat, n):
    tdmi = np.zeros(n)
    N = dat.shape[0]
    dat_buffer = np.zeros((N-n+1, 2))
    dat_buffer[:,0] = dat[:N-n+1, 0]
    for i in range(n):
        dat_buffer[:,1] = dat[i:i+N-n+1,1]
        tdmi[i] = mi_py(dat_buffer)
    return tdmi

np.random.seed(2022)
n = 24000
dat = np.random.rand(n,2)
dat[:,1] += dat[:,0]
# np.random.shuffle(dat[:,1])

t0 = time.time()
mi_py_result = mi_py(dat)
print(f'[INFO]:   mi_adaptive (python)  takes {time.time() - t0:3.3f} s')

# run first to excute JIT compling
mi_cy(dat[:,0], dat[:,1], bins=50,)
t0 = time.time()
mi_cy_uni_result = mi_cy(dat[:,0], dat[:,1], bins=50,)
print(f'[INFO]:   mi_uniform  (numba)   takes {time.time() - t0:3.3f} s')

t0 = time.time()
mi_cy_adp_result = mi_cy(dat[:,0], dat[:,1], algorithm='adaptive')
print(f'[INFO]:   mi_adaptive (cython)  takes {time.time() - t0:3.3f} s')

# ---

n_delay = 100

t0 = time.time()
tdmi_py_result = TDMI_py(dat, n_delay)
print(f'[INFO]: tdmi_adaptive (python)  takes {time.time() - t0:3.3f} s')

# run first to excute JIT compling
TDMI_cy(dat[:,0], dat[:,1], n_delay, bins=50)
t0 = time.time()
tdmi_cy_result = TDMI_cy(dat[:,0], dat[:,1], n_delay, bins=50)
print(f'[INFO]: tdmi_uniform  (numba)   takes {time.time() - t0:3.3f} s')

t0 = time.time()
tdmi_cy_omp_result = TDMI_cy(dat[:,0], dat[:,1], n_delay, algorithm='adaptive')
print(f'[INFO]: tdmi_adaptive (cython)  takes {time.time() - t0:3.3f} s')
