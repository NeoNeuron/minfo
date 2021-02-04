from libcpp.vector cimport vector

cdef extern from "mutual_info.h":
    double MutualInfo(vector[double] &, vector[double] &)

def mutual_info(vec1, vec2):
    return MutualInfo(<vector[double]&> vec1, <vector[double]&> vec2)