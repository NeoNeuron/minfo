#include <random>
#include <chrono>
#include <iomanip>
#include <iostream>
#include "mutual_info.h"

using namespace std;

mt19937 rand_gen(1);
uniform_real_distribution<> uni_dis(0.0,1.0);

int main() {
    int n = 24000;
    vector<double> x_d(n), y_d(n);
    for (int i = 0; i < n; i++) {
        x_d[i] = uni_dis(rand_gen);
        y_d[i] = x_d[i]+uni_dis(rand_gen);
    }
    // shuffle(y_d.begin(), y_d.end(), rand_gen);
    auto start = chrono::system_clock::now();

    double mi = MutualInfo(x_d, y_d);
    
    auto finish = chrono::system_clock::now();
    cout << setprecision(16) << mi << endl;
	chrono::duration<double> elapsed_seconds = finish-start;
    printf(">> it takes : \t%3.3f s\n", elapsed_seconds.count());
    return 0;
}