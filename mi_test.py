import numpy as np
from mutual_info_cy import mutual_info as mi_cy
from mutual_info import mutual_info as mi_py
import time

n = 24000
dat = np.zeros((n,2))
dat[:,0] = np.random.rand(n)
dat[:,1] = dat[:,0] + np.random.rand(n)
# np.random.shuffle(dat[:,1])
t0 = time.time()
print( mi_cy(dat[:,0], dat[:,1]) )
print(f'cpp version takes {time.time() - t0:3.3f} s')

t0 = time.time()
print( mi_py(dat) )
print(f'python version takes {time.time() - t0:3.3f} s')