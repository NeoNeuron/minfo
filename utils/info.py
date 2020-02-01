"""
Module for information theory analysis

"""
import numpy as np

def mutual_info(x, y, xbins, ybins):
    jointpdf, _, _ = np.histogram2d(x, y, bins = [xbins, ybins])
    px = jointpdf.sum(1)
    py = jointpdf.sum(0)
    # rearange px and py
    px = np.repeat(np.array([px]), ybins, axis = 0).T
    py = np.repeat(np.array([py]), xbins, axis = 0)
    # remove zero
    idx_nonzero = jointpdf.nonzero()
    jointpdf = jointpdf[idx_nonzero]
    px = px[idx_nonzero]
    py = py[idx_nonzero]
    return (jointpdf*np.log(jointpdf/px/py)).sum()/len(x) + np.log(len(x))

def main(prefix, spike_fname, lfp_fname, bins, ndelays, verbose, tdmi_fname=None):
    '''
    Calculate TDMI between spike train and LFP

    Parameters
    ----------
    prefix : string
        prefix of input and output data
    spike_fname : string
        filename of spike train data
    lfp_fname : string
        filename of LFP data
    bins : int
        number of bins in generating PDF of LFP data
    ndelays : int
        number of delayed time window, symmetric for positive and negative time delay
    verbose : bool
        flag to print the runing time
    tdmi_fname : string, optional
        filname of output TDMI data

    Return
    ------
    tdmi : ndarray
        TDMI data

    '''
    import time
    t_start = time.time()
    spike = np.load(prefix + spike_fname)
    lfp = np.load(prefix + lfp_fname)
    t_finish_1 = time.time()
    a = [mutual_info(spike[:-i], lfp[i:], xbins=2, ybins = bins) for i in np.arange(1, ndelays+1)]
    b = [mutual_info(spike[i:], lfp[:-i], xbins=2, ybins = bins) for i in np.arange(1, ndelays+1)]
    c = mutual_info(spike, lfp, xbins=2, ybins = bins)
    t_finish_2 = time.time()
    tdmi = np.hstack((b,c,a))
    tdmi = np.vstack((np.arange(-ndelays, ndelays+1), tdmi)).T
    
    if tdmi_fname != None:
        np.savetxt(prefix + tdmi_fname, tdmi, delimiter = ',')

    t_finish_3 = time.time()
    if verbose:
        print('>> Reading date took %5.3e s'% (t_finish_1-t_start))
        print('>> Calculating TDMI took %5.3e s'% (t_finish_2-t_finish_1))
        print('>> Saving TDMI took %5.3e s'% (t_finish_3-t_finish_2))
    return tdmi

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description = "Calculate TDMI script.")
    parser.add_argument('prefix', type = str, help = 'working directory of source data and output data')
    parser.add_argument('spike_fname', type = str, help = 'filename of spike train data, *.npy')
    parser.add_argument('lfp_fname', type = str, help = 'filename of LFP data, *.npy')
    parser.add_argument('tdmi_fname', type = str, help = 'filename of output TDMI data, *.csv')
    parser.add_argument('bins', type = int, help = 'number of bins in LFP histogram')
    parser.add_argument('ndelays', type = int, help = 'number of delayed bins in TDMI calculation')
    parser.add_argument('-v', '--verbose', action='store_true', help = 'enable verbose of running time')
    args = parser.parse_args()
    main(**(vars(args)))
