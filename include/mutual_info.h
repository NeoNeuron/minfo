/**
    Adaptive partitioning algorithm for mutual information estimation

    C++ implementation of mutual information estimation, where adaptive partitioning strategy is applied.

    Reference: Darbellay, G. A., & Vajda, I. (1999). Estimation of the information by an adaptive partitioning of the observation space. IEEE Transactions on Information Theory, 45(4), 1315-1321.
**/
#ifndef __MUTUAL_INFO_
#define __MUTUAL_INFO_
#include <vector>
#include <algorithm>
#include <numeric>

// Reference: 
//  https://stackoverflow.com/questions/1577475/c-sorting-and-keeping-track-of-indexes
template <typename T>
std::vector<size_t> argsort(const std::vector<T> &v) {
    // initialize original index locations
    std::vector<size_t> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    // using std::stable_sort instead of std::sort
    // to avoid unnecessary index re-orderings
    // when v contains elements of equal values 
    std::stable_sort(idx.begin(), idx.end(),
        [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});

  return idx;
}

double MutualInfo(std::vector<double> &x, std::vector<double> &y);

// Compute the delayed MutualInfo from x to y
// i.e. I(tau) = I(x(t),y(t+tau))
std::vector<double> TDMI(std::vector<double>& x, std::vector<double>& y, int n_delay);

// OpenMP accelerated version
std::vector<double> TDMI_omp(std::vector<double>& x, std::vector<double>& y, int n_delay);

#endif //__MUTUAL_INFO_