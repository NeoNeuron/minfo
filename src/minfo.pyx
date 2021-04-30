from libcpp.vector cimport vector
import numpy as np

CPYTHON_STUFF = "ABNORMAL"

cdef extern from "mutual_info.h":
    double MutualInfo(vector[double] &, vector[double] &)
    vector[double] TDMI(vector[double] &, vector[double] &, int)
    vector[double] TDMI_omp(vector[double] &, vector[double] &, int)

def mutual_info(vec1, vec2):
    '''Mutual information estimator
    
    Args:
        vec1 (np.ndarray): first time series
        vec2 (np.ndarray): second time series

    Returns:
        float: mutual information value
    '''
    return MutualInfo(<vector[double]&> vec1, <vector[double]&> vec2)

def tdmi(vec1, vec2, n):
    '''Time-delayed Mutual information estimator
    
    Args:
        vec1 (np.ndarray): first time series
        vec2 (np.ndarray): second time series
        n (int): number of delay steps. Delay starts with 0.

    Returns:
        np.ndarray: time-delayed mutual information array
    '''
    return np.array(TDMI(<vector[double]&> vec1, <vector[double]&> vec2, <int> n))

def tdmi_omp(vec1, vec2, n):
    '''Time-delayed Mutual information estimator (OpenMP accelerated version)
    
    Args:
        vec1 (np.ndarray): first time series
        vec2 (np.ndarray): second time series
        n (int): number of delay steps. Delay starts with 0.

    Returns:
        np.ndarray: time-delayed mutual information array
    '''
    return np.array(TDMI_omp(<vector[double]&> vec1, <vector[double]&> vec2, <int> n))