#include <array>
#include <cmath>
#include "mutual_info.h"
#include "omp.h"
using namespace std;

double MI_core(int Np, array<int, 4> &edges) {
    int Nx = edges[1] - edges[0] + 1;
    int Ny = edges[3] - edges[2] + 1;
    return Np * log(1.0 * Np / Nx / Ny);
}

double PartialSum(vector<int>& x, vector<int>& y, array<int, 4> &edges, bool force_split) {
    vector<vector<int> > p(4, vector<int>());
    int Np = x.size();
    int x_mid = floor((edges[0] + edges[1])/2);
    int y_mid = floor((edges[2] + edges[3])/2);
    for (int i = 0; i < Np; i++) {
        if (x[i] <= x_mid && y[i] <= y_mid) {
            p[0].push_back(i);
        } else if (x[i] <= x_mid && y[i] > y_mid) {
            p[1].push_back(i);
        } else if (x[i] > x_mid && y[i] <= y_mid) {
            p[2].push_back(i);
        } else {
            p[3].push_back(i);
        }
    }
    double tst = 0.0;
    for (int i=0; i<4; i++) {
        tst += (p[i].size()-Np/4.0) * (p[i].size()-Np/4.0);
    }
    tst *= 4.0 / Np;
    if (tst <= 7.8 && !force_split) {
        return MI_core(Np, edges);
    } else {
        array<array<int, 4>, 4> edges4 = {
                {
                    { edges[0],  x_mid,  edges[2],  y_mid},
                    { edges[0],  x_mid, y_mid+1, edges[3]},
                    {x_mid+1, edges[1],  edges[2],  y_mid},
                    {x_mid+1, edges[1], y_mid+1, edges[3]}
                }
            };
        double mi_buffer = 0.0;
        for (size_t i=0; i < 4; i++) {
            if (p[i].size() > 2) {
                vector<int> x_p, y_p;
                for (size_t j = 0; j < p[i].size(); j++) {
                    x_p.push_back(x[p[i][j]]);
                    y_p.push_back(y[p[i][j]]);
                }
                mi_buffer += PartialSum(x_p, y_p, edges4[i], false);
            } else if (p[i].size() > 0 && p[i].size() <=2) {
                mi_buffer += MI_core(p[i].size(), edges4[i]);
            } else {
                continue;
            }
        }
        return mi_buffer;
    }
}

double MutualInfo(vector<double> &x, vector<double> &y) {
    // Normalize data series to [0, x.size()-1]
    int n = x.size();
    vector<int> x_int(n), y_int(n);
    int counter = 0;
    for (auto i: argsort(x)) {
        x_int[i] = counter;
        counter ++;
    }
    counter = 0;
    for (auto i: argsort(y)) {
        y_int[i] = counter;
        counter ++;
    }
    array<int,4> edges = {0, n-1, 0, n-1};

    return PartialSum(x_int, y_int, edges, true)/n + log(n);
}

vector<double> TDMI(vector<double>& x, vector<double>& y, int n_delay) {
    vector<double> tdmi(n_delay);
    vector<double> x_buffer(x.begin(), x.end()-n_delay+1);
    vector<double> y_buffer(y.size()-n_delay+1);
    for (int i = 0; i < n_delay; i ++) {
        y_buffer.assign(y.begin()+i, y.end()-n_delay+1+i);
        tdmi[i] = MutualInfo(x_buffer, y_buffer);
        y_buffer.clear();
    }
    return tdmi;
}

vector<double> TDMI_omp(vector<double>& x, vector<double>& y, int n_delay) {
    vector<double> tdmi(n_delay);
    vector<double> x_buffer(x.begin(), x.end()-n_delay+1);
#pragma omp parallel for
    for (int i = 0; i < n_delay; i ++) {
        vector<double> y_buffer(y.size()-n_delay+1);
        y_buffer.assign(y.begin()+i, y.end()-n_delay+1+i);
        tdmi[i] = MutualInfo(x_buffer, y_buffer);
    }
    return tdmi;
}