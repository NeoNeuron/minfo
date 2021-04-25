#!/usr/bin/env python
# coding: utf-8
import numpy as np
from mutual_info import mutual_info

def main():
    print( mutual_info(dat) )

if __name__ == '__main__':
    dat = np.load('sample.npy')
    import cProfile
    cProfile.run('main()', "output.dat")

    import pstats
    from pstats import SortKey

    with open("output_time.txt", 'w') as f:
        p = pstats.Stats("output.dat", stream=f)
        p.sort_stats("time").print_stats()

    with open("output_calls.txt", 'w') as f:
        p = pstats.Stats("output.dat", stream=f)
        p.sort_stats("calls").print_stats()