//***************
//  Copyright: Kyle Chen
//  Author: Kyle Chen
//  Created: 2017-02-27 21:47:53
//  Description: Test all four TDMI functions;
//***************
#include "mi_uniform.h"
#include "io.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
using namespace std;

void PrintTitle(string title_str) {
  for (int i = 0; i < 80; i ++) { cout << '='; }
  cout << '\n';
  cout << title_str << '\n';
  for (int i = 0; i < 80; i ++) { cout << '-'; }
  cout << '\n';
  return;
}

void PrintResult(vector<size_t>& time_range, vector<double>& result) {
  for (int i = -time_range[0]; i < 0; i++) {
    cout << setw(8) << i  << right << '|';
  }
  cout << endl;
  for (int i = 0; i < time_range[0]; i++) {
    cout << setw(8) << right << scientific << setprecision(1) << result[i] << '|';
  }
  cout << endl;
  for (int i=0; i < time_range[1] + 1; i++) {
    cout << setw(8) << i << internal << '|';
  }
  cout << endl;
  for (int i=0; i < time_range[1] + 1; i++) {
    cout << setw(8) << right << scientific << setprecision(1) << result[i + time_range[0]] << '|';
  }
  cout << endl;
  return;
}

int main(int argc, const char* argv[]) {
  size_t len_data = 1e4;
  mt19937 rand_gen(0);
  normal_distribution<> norm_dist;
  bernoulli_distribution ber_dist(0.01);
  // generate test data;
  vector<int> data_b1(len_data, 0);
  vector<int> data_b2(len_data, 0);
  for (size_t i; i < len_data; i++) {
    if (ber_dist(rand_gen)) { data_b1[i] = 1; }
  }
  for (size_t i; i < len_data; i++) {
    if (ber_dist(rand_gen)) { data_b2[i] = 1; }
  }
  vector<double> x(len_data, 0), y(len_data, 0), u(len_data, 0), v(len_data, 0); // x, y are two int random variables with Gaussion distribution; u = f(x), v = g(y);
  x[0] = norm_dist(rand_gen);
  y[0] = norm_dist(rand_gen);
  double correlation_factor = 0.1;
  for (size_t i = 1; i < len_data; i++) {
    x[i] = -correlation_factor*x[i-1] + norm_dist(rand_gen);
    y[i] = -correlation_factor*y[i-1] + 5*correlation_factor*x[i-1] + norm_dist(rand_gen);
  }
  for (size_t i = 0; i < len_data; i++) {
    u[i] = x[i];
    v[i] = abs(y[i]) * y[i];
  }
  // Test BB version
  vector<size_t> time_range = {10,10};
  vector<double> tdmi_bb;
  TDMI(data_b1, data_b2, tdmi_bb, time_range);
  PrintTitle("Result of TDMI_BB test:");
  PrintResult(time_range,tdmi_bb);
  
  // Test DD version
  vector<double> xy_tdmi, uv_tdmi;
  TDMI(x, y, xy_tdmi, time_range, 0.1);
  TDMI(u, v, uv_tdmi, time_range, 0.1);
  PrintTitle("Result of TDMI_DD test: I(x,y)");
  PrintResult(time_range,xy_tdmi);
  PrintTitle("Result of TDMI_DD test: I(f(x),f(y))");
  PrintResult(time_range,uv_tdmi);
  // Test BD version
  vector<double> tdmi_bd;
  TDMI(data_b1, x, tdmi_bd, time_range, 0.1);
  PrintTitle("Result of TDMI_BD test:");
  PrintResult(time_range,tdmi_bd);
  // Test BD2 version
  TDMI2bins(data_b1, x, tdmi_bd, time_range, 0.0);
  PrintTitle("Result of TDMI_BD2 test:");
  PrintResult(time_range,tdmi_bd);
  cout << "OK" << endl;

  return 0;
}
