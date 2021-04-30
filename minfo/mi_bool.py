import numpy as np

def mutual_info(x:np.ndarray, y:np.ndarray=None, rowvar=True)->np.ndarray:
    """Mutual Information Estimator for binary random variables.

    Args:
        x (np.ndarray): 2D numpy array of random variable. By default(rowvar),
            each row is a variable, each column is an observation.
        y (np.ndarray, optional): 2D array of random variable with the same
            format with x. If None, y is defined as a copy of x. 
            Defaults to None.
        rowvar (bool, optional): If True, each row of x(or y) is a variable,
            and each column is an observation. If False, each row of x(or y)
            is an observation, and each column is a variable. 
            Defaults to True.

    Raises:
        ValueError: x and y must have the same dimension of observations.

    Returns:
        np.ndarray: (nx, ny) mutual information matrix.
    """
    ref_flag = False
    if y is None:
        y = x   # shallow copy
        ref_flag = True
    else:
        if rowvar:
            if x.shape[1] != y.shape[1]:
                raise ValueError(f"x and y must have same second dimension, but "
                                 f"have shapes {x.shape} and {y.shape}")
        else:
            if x.shape[0] != y.shape[0]:
                raise ValueError(f"x and y must have same first dimension, but "
                                 f"have shapes {x.shape} and {y.shape}")
    if rowvar:
        N = x.shape[1]
        n00 = (~x).astype(np.float32) @ (~y.T).astype(np.float32)
        n01 = (~x).astype(np.float32) @ (y.T).astype(np.float32)
        if ref_flag:
            n10 =  n01.T
        else:
            n10 =  (x).astype(np.float32) @ (~y.T).astype(np.float32)
        # n11 =    x.astype(np.float32) @ (y.T).astype(np.float32)
        n11 =  np.ones_like(n00)*N - n00 - n01 - n10
        nx, ny = x.sum(1), y.sum(1)
    else:
        N = x.shape[0]
        n00 = (~x.T).astype(np.float32) @ (~y).astype(np.float32)
        n00 = (~x.T).astype(np.float32) @ (~y).astype(np.float32)
        n01 = (~x.T).astype(np.float32) @ y.astype(np.float32)
        if ref_flag:
            n10 =  n01.T
        else:
            n10 =  (x.T).astype(np.float32) @ (~y).astype(np.float32)
        # n11 =  (x.T).astype(np.float32) @ y.astype(np.float32)
        n11 =  np.ones_like(n00)*N - n00 - n01 - n10
        nx, ny = x.sum(0), y.sum(0)

    I = np.zeros_like(n00, dtype=float)
    for nxy_, nx_, ny_ in zip(
        (n00, n01, n10, n11),
        (N-nx, N-nx, nx, nx),
        (N-ny, ny, N-ny, ny)
    ):
        nonzero_mask = nxy_>0
        buffer = nxy_[nonzero_mask]*np.log(nxy_[nonzero_mask]/np.outer(nx_, ny_)[nonzero_mask])
        I[nonzero_mask] += buffer

    return I / N + np.log(N)
