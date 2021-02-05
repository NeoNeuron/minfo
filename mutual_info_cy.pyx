from libcpp.vector cimport vector
import numpy as np

cdef extern from "mutual_info.h":
    double MutualInfo(vector[double] &, vector[double] &)
    vector[double] TDMI(vector[double] &, vector[double] &, int)

def mutual_info(vec1, vec2):
    return MutualInfo(<vector[double]&> vec1, <vector[double]&> vec2)

def tdmi(vec1, vec2, n):
    return np.array(TDMI(<vector[double]&> vec1, <vector[double]&> vec2, <int> n))