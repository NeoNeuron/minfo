#!/usr/bin/env python
# coding: utf-8

# # Adaptive partitioning algorithm for mutual information estimation
# 
# Python implementation of mutual information estimation, where adaptive partitioning strategy is applied.
# 
# ## Reference
# - Darbellay, G. A., & Vajda, I. (1999). Estimation of the information by an adaptive partitioning of the observation space. IEEE Transactions on Information Theory, 45(4), 1315-1321.

import numpy as np

def mutual_info(data):
    """
    Compute mutual information between data[:,0] and data[:,1]
    
    Parameter
    ---------
    data : 2D array
        N by 2 matrix, each column represents a random variable.
        
    Return
    ------
    mutual_info : float
        mutual information value.
    
    """
    
    def _partial_sum(_partial_data, range2d, force_split=False):
        """
        Parameters
        ----------
        _partial_data : 2D array of floats
            data represented as [x,y], x is column vector of first variable,
            y is the second one.
            
        range2d: 1D array
            The range of two variables in the form of 
            [xmin, xmax, ymin, ymax]
        
        force_split : bool
            Toggle to force the spliting operation in current recursion step.
            Mainly used for first recursion step in mutual information cauculation.
            
        Return
        ------
        _partial_sum : float 
            partial weighted sum of the log-frequency value
        
        """
   
        Np = _partial_data.shape[0]
        x_average = np.floor((range2d[0]+range2d[1])/2)
        y_average = np.floor((range2d[2]+range2d[3])/2)
        partition_mask = np.empty((Np, 4), dtype=bool)
        mask0 = (_partial_data[:,0]<=x_average)
        mask1 = (_partial_data[:,1]<=y_average)
        partition_mask[:,0] =  mask0 *  mask1
        partition_mask[:,1] =  mask0 * ~mask1
        partition_mask[:,2] = ~mask0 *  mask1
        partition_mask[:,3] = ~mask0 * ~mask1
        N_events = partition_mask.sum(axis=0)   # number of events in each masks
        
        # criteria for spliting into finer cells
        tst= 4*np.sum((N_events-Np/4*np.ones(4))**2)/Np
        
        if tst <= 7.8 and not force_split:
            # bottom of the recursion
            Nx = range2d[1] - range2d[0] + 1
            Ny = range2d[3] - range2d[2] + 1
            return Np * np.log( Np / (Nx * Ny) )
        else:
            # split cell into 4 smaller cells
            range2d_fine = np.array([[ range2d[0],  x_average,  range2d[2],  y_average],
                                     [ range2d[0],  x_average, y_average+1, range2d[3]],
                                     [x_average+1, range2d[1],  range2d[2],  y_average],
                                     [x_average+1, range2d[1], y_average+1, range2d[3]]])
            minfo_buffer = 0
            for i in range(4):
                if N_events[i]>2:
                    minfo_buffer += _partial_sum(_partial_data[partition_mask[:,i],:],
                                                 range2d_fine[i])
                else:
                    if N_events[i]>0:
                        Nx = range2d_fine[i,1] - range2d_fine[i,0] + 1
                        Ny = range2d_fine[i,3] - range2d_fine[i,2] + 1
                        minfo_buffer += N_events[i] * np.log( N_events[i] / (Nx*Ny) )
            return minfo_buffer
        
    
    # extract the order of raw sequence.
    N = data.shape[0]
    data_index = np.argsort(data, axis=0)
    data_normal = np.zeros_like(data, dtype=int)
    for i in range(2):
        data_normal[data_index[:,i],i] = np.arange(N)
    
    return _partial_sum(data_normal, [0,N-1,0,N-1], True)/N + np.log(N)



if __name__ == '__main__':
    import numpy as np 
    dat = np.load('sample.npy')
    print( mutual_info(dat) )



