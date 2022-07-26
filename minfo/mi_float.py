import numpy as np
from numba import njit, prange

from . import mi_c

def mutual_info(x:np.ndarray, y:np.ndarray, bins:int=None, algorithm:str='uniform')->np.ndarray:
    """_summary_

    Args:
        x (np.ndarray): 2D numpy array of random variable. By default(rowvar),
            each row is a variable, each column is an observation.
        y (np.ndarray): 2D array of random variable with the same
            format with x. 
        bins (int, optional): number of bins of histogram2d with uniform binning strategy.
            Defaults to None.
        algorithm (str, optional): algorithm to estimate PDFs, either 'uniform' or 'adaptive'.
            Defaults to 'uniform'.

    Raises:
        ValueError: x and y must have the same size of observations, same shape.

    Returns:
        np.ndarray: value of mutual information.
    """
    if x.shape[0] != y.shape[0]:
        raise ValueError(f"x and y must have same first dimension, but "
                             f"have shapes {x.shape} and {y.shape}")
    if bins is None:
        bins = int(np.sqrt(x.shape[0]))
    if algorithm == 'uniform':
        bins2 = np.array([bins, bins])
        mi = mutual_info_uni_nb(x, y, bins2)
    elif algorithm == 'adaptive':
        mi = mi_c.mutual_info(x, y)
    else:
        raise ValueError(f"{algorithm:s} is not acceptable. Please use either 'uniform' or 'adaptive'.")
    return mi

def TDMI(x:np.ndarray, y:np.ndarray, delay:int, bins:int=None, algorithm:str='uniform')->np.ndarray:
    """_summary_

    Args:
        x (np.ndarray): 2D numpy array of random variable. By default(rowvar),
            each row is a variable, each column is an observation.
        y (np.ndarray): 2D array of random variable with the same
            format with x. 
        delay (int | np.ndarray): number of delays, or explicit sequence of delay indices.
            Defaults to None.
        bins (int, optional): number of bins of histogram2d with uniform binning strategy.
            Defaults to None.
        algorithm (str, optional): algorithm to estimate PDFs, either 'uniform' or 'adaptive'.
            Defaults to 'uniform'.

    Raises:
        ValueError: x and y must have the same size of observations, same shape.

    Returns:
        np.ndarray: value of mutual information.
    """
    if x.shape[0] != y.shape[0]:
        raise ValueError(f"x and y must have same first dimension, but "
                             f"have shapes {x.shape} and {y.shape}")
    if bins is None:
        bins = int(np.sqrt(x.shape[0]))
    if isinstance(delay, int):
        delays = np.arange(delay)
    elif isinstance(delay, np.ndarray):
        delays = delay
    if algorithm == 'uniform':
        tdmi = TDMI_uni(x, y, delays, bins)
    elif algorithm == 'adaptive':
        tdmi = mi_c.tdmi_omp(x, y, delays)
    else:
        raise ValueError(f"{algorithm:s} is not acceptable. Please use either 'uniform' or 'adaptive'.")
    return tdmi

@njit(nogil=True, parallel=False)
def hist2d_numba_seq(data:np.ndarray, bins:np.ndarray, ranges:np.ndarray)->np.ndarray:
    """2D histogram estimation accelerated by Numba
        https://iscinumpy.gitlab.io/post/histogram-speeds-in-python/

    Args:
        data (np.ndarray): 2D array of data. Each row is a variable and each column is an observation.
        bins (np.ndarray): array of bins with shape (2,).
        ranges (np.ndarray): array of ranges with shape (2,2). ranges[i,:] is the range of i-th variable.

    Returns:
        np.ndarray: 2D array of histogram data.
    """
    H = np.zeros((bins[0], bins[1]), dtype=np.uint64)
    delta = 1 / ((ranges[:, 1] - ranges[:, 0]) / bins)

    for t in range(data.shape[1]):
        i = (data[0, t] - ranges[0, 0]) * delta[0]
        j = (data[1, t] - ranges[1, 0]) * delta[1]
        if 0 <= i < bins[0] and 0 <= j < bins[1]:
            H[int(i), int(j)] += 1

    return H

@njit(nogil=True, parallel=False)
def mutual_info_uni_nb(x:np.ndarray, y:np.ndarray, bins:np.ndarray)->float:
    """mutual information estimator. Joint PDF is estimated using uniform binning.

    Args:
        x (np.ndarray): 1D data
        y (np.ndarray): 1D data
        bins (np.ndarray | tuple | list): number of bins for histogram2d estimation.

    Returns:
        float: value of mutual information
    """
    N = x.shape[0]
    ranges = np.array([[x.min(), x.max()], [y.min(), y.max()]])
    dat = np.vstack((x,y))
    counts_xy = hist2d_numba_seq(dat, bins=bins, ranges=ranges)
    x_mar = counts_xy.sum(1)
    y_mar = counts_xy.sum(0)
    mi_val = 0.0
    for i in range(bins[0]):
        for j in range(bins[1]):
            if counts_xy[i,j] > 0:
                mi_val += counts_xy[i,j]*np.log(counts_xy[i,j]/(x_mar[i]*y_mar[j]))
    return mi_val / N + np.log(N)

# Python+Numpy based mutual information estimator with uniform binning.
def mutual_info_uni(x, y, bins):
    N = x.shape[0]
    counts_xy,_,_ = np.histogram2d(x, y, bins=bins,)
    x_mar = np.tile(counts_xy.sum(1, keepdims=True), (1, bins))
    y_mar = np.tile(counts_xy.sum(0, keepdims=True), (bins, 1))
    mi = np.nansum(counts_xy*np.log(counts_xy/(x_mar*y_mar)))
    return mi/N + np.log(N)

@njit(nogil=True, parallel=False)
def DMI_uni(x:np.ndarray, y:np.ndarray, delay:int, bins:int):
    bins2 = np.array([bins, bins])
    if delay == 0:
        return mutual_info_uni_nb(x, y, bins2)
    elif delay < 0:
        return mutual_info_uni_nb(x[-int(delay):],y[:int(delay)], bins2)
    elif delay > 0:
        return mutual_info_uni_nb(x[:-int(delay)],y[int(delay):], bins2)

@njit(nogil=True, parallel=True)
def TDMI_uni(x:np.ndarray, y:np.ndarray, time_range:np.ndarray, bins:np.ndarray=None)->np.ndarray:
    tdmi = np.zeros(len(time_range))
    for i in prange(len(time_range)):
        tdmi[i] = DMI_uni(x,y,time_range[i],bins)
    return tdmi
