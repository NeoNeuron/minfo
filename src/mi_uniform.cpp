//***************
//  Copyright: Kyle Chen
//  Author: Kyle Chen
//  Date: 2018-03-31
//  Description: source file of mi_uniform.h
//***************
#include "../include/mi_uniform.h"
#include "../include/io.h"
#include <iostream>
#include <cmath>
#include <numeric>
#include <algorithm>
using namespace std;

double Max(vector<double>& data) {
  return *(max_element(data.begin(), data.end()));
}

double Min(vector<double>& data) {
  return *(min_element(data.begin(), data.end()));
}

void JointPDF(vector<bool>& x, vector<bool>& y, vector<vector<double> >& jointpdf) {
  size_t num_pairs = x.size();
  vector<vector<int> > count_xy(2, vector<int>(2, 0));
  for (size_t i = 0; i < num_pairs; i++) {
    if (x[i]) {
      if (y[i]) count_xy[1][1] ++;
      else count_xy[1][0] ++;
    } else {
      if (y[i]) count_xy[0][1] ++;
      else count_xy[0][0] ++;
    }
  }
  jointpdf.resize(2, vector<double>(2, 0.0));
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      jointpdf[i][j] += count_xy[i][j] * 1.0 / num_pairs;
    }
  }
}

void JointPDF(vector<double>& x, vector<double>& y, vector<vector<double> >& jointpdf, double x_binsize, double y_binsize) {
  size_t num_pairs = x.size();
  double x_max = Max(x);
  double x_min = Min(x);
  double y_max = Max(y);
  double y_min = Min(y);
  size_t x_bin_num = ceil((x_max - x_min) / x_binsize);
  size_t y_bin_num = ceil((y_max - y_min) / y_binsize);
  jointpdf.resize(x_bin_num, vector<double>(y_bin_num, 0.0));
  size_t indx, indy;
  for (size_t i = 0; i < num_pairs; i++) {
    indx = floor((x[i] - x_min) / x_binsize);
    indy = floor((y[i] - y_min) / y_binsize);
    if (indx == x_bin_num) indx = x_bin_num - 1;
    if (indy == y_bin_num) indy = y_bin_num - 1;
    jointpdf[indx][indy] += 1.0 / num_pairs;
  }
}

void JointPDF(vector<bool>& binary_spikes, vector<double>& lfp, vector<vector<double> >& jointpdf, double binsize) {
  size_t num_pairs = binary_spikes.size();
  double lfp_min = Min(lfp);
  size_t bin_num = ceil((Max(lfp) - lfp_min) / binsize);
  jointpdf.resize(2, vector<double>(bin_num, 0.0));
  size_t ind;
  for (size_t i = 0; i < num_pairs; i++) {
    // determine lfp's coordination;
    ind = floor((lfp[i] - lfp_min) / binsize);
    if (ind == bin_num) ind = bin_num - 1;
    // determine spike's coordination;
    if (binary_spikes[i]) jointpdf[1][ind] += 1.0 / num_pairs;
    else jointpdf[0][ind] += 1.0 / num_pairs;
  }
}

void JointPDF2bins(vector<bool>& binary_spikes, vector<double>& lfp, vector<vector<double> >& jointpdf, double threshold) {
  size_t num_pairs = binary_spikes.size();
  vector<vector<int> > joint_count(2, vector<int>(2, 0));
  for (size_t i = 0; i < num_pairs; i++) {
    if (binary_spikes[i]) {
      if (lfp[i] >= threshold) joint_count[1][1] ++;
      else joint_count[1][0] ++;
    } else {
      if (lfp[i] >= threshold) joint_count[0][1] ++;
      else joint_count[0][0] ++;
    }
  }
  jointpdf.resize(2, vector<double>(2, 0.0));
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      jointpdf[i][j] += joint_count[i][j] * 1.0 / num_pairs;
    }
  }
  return;
}

void MarginalPDF(vector<vector<double> >& jointpdf, vector<double>& p1, vector<double>& p2) {
  p1.clear();
  p1.resize(jointpdf.size());
  p2.clear();
  p2.resize(jointpdf.begin() -> size(), 0.0);
  for (size_t i = 0; i < p1.size(); i++) {
    p1[i] = accumulate(jointpdf[i].begin(), jointpdf[i].end(), 0.0);
    for (size_t j = 0; j < p2.size(); j++) {
      p2[j] += jointpdf[i][j];
    }
  }
}

double MI(vector<vector<double> >& jointpdf) {
  // Marginal probability distribution function;
  vector<double> px, py;
  MarginalPDF(jointpdf, px, py);
  // Calculate mutual information;
  double mi = 0.0;
  for (size_t i = 0; i < px.size(); i++) {
    for (size_t j = 0; j < py.size(); j++) {
      if (jointpdf[i][j] != 0) {
        mi += jointpdf[i][j] * log(jointpdf[i][j] / (px[i] * py[j]));
      }
    }
  }
  return mi;
}

double MIBB(vector<bool>& x, vector<bool>& y) {
  if (x.size() != y.size()) {
    cout << "ERROR: x and y don't have the same length." << endl;
    return 0;
  } else {
    // Joint Probability Histograms;
    vector<vector<double> > joint_xy;
    JointPDF(x, y, joint_xy);
    return MI(joint_xy);
  }
}

double MIDD(vector<double>& x, vector<double>& y, double x_binsize, double y_binsize, bool pdf_output_flag) {
  //  Compare the length of x & y;
  if (x.size() != y.size()) {
    cout << "ERROR: x and y don't have the same length." << endl;
    return 0;
  } else {
    // Calculate joint probability distribution function;
    vector<vector<double> > jointpdf;
    JointPDF(x, y, jointpdf, x_binsize, y_binsize);
    if (pdf_output_flag) Print2D("./data/mi/joint_pdf.csv", jointpdf, "trunc");
    return MI(jointpdf);
  }
}

double MIBD(vector<bool>& bool_series, vector<double>& double_series, double binsize, bool pdf_output_flag) {
  if (bool_series.size() != double_series.size()) {
    cout << "ERROR: x and y don't have the same length." << endl;
    return 0;
  } else {
    // Calculate joint probability distribution function;
    vector<vector<double> > jointpdf;
    JointPDF(bool_series, double_series, jointpdf, binsize);
    if (pdf_output_flag) Print2D("./data/mi/joint_pdf.csv", jointpdf, "trunc");
    return MI(jointpdf);
  }
}

double MIBD2bins(vector<bool>& bool_series, vector<double>& double_series, double threshold) {
  if (bool_series.size() != double_series.size()) {
    cout << "ERROR: x and y don't have the same length." << endl;
    return 0;
  } else {
    // Calculate joint probability distribution function;
    vector<vector<double> > jointpdf;
    JointPDF2bins(bool_series, double_series, jointpdf, threshold);
    return MI(jointpdf);
  }
}

void TDMI(vector<bool>& x, vector<bool>& y, vector<double> & tdmi, vector<size_t> &range) {
  tdmi.clear();
  tdmi.resize(range[0] + range[1] + 1, 0);
  // No shift;
  tdmi[range[0]] = MIBB(x, y);
  // Negative shift;
  vector<bool> x_copy = x;
  vector<bool> y_copy = y;
  for (size_t i = 0; i < range[0]; i++) {
    x_copy.erase(x_copy.begin());
    y_copy.erase(y_copy.end() - 1);
    tdmi[range[0] - i - 1] = MIBB(x_copy, y_copy);
  }
  // Positive shift;
  x_copy = x;
  y_copy = y;
  for (size_t i = 0; i < range[1]; i++) {
    x_copy.erase(x_copy.end() - 1);
    y_copy.erase(y_copy.begin());
    tdmi[range[0] + i + 1] = MIBB(x_copy, y_copy);
  }
}

void TDMI(vector<double>& x, vector<vector<double> >& y, vector<double> & tdmi, double x_binsize, double y_binsize) {
  tdmi.resize(y.size(), 0);
  for (size_t i = 0; i < y.size(); i++) {
    tdmi[i] = MIDD(x, y[i], x_binsize, y_binsize, false);
  }
}

void TDMI(vector<double>& x, vector<double>& y, vector<double>& tdmi, vector<size_t> &range, double binsize) {
  // initialize container of tdmi;
  tdmi.clear();
  tdmi.resize(range[0] + range[1] + 1, 0);
  // prepare series;
  size_t res = range[0];
  if (res < range[1]) res = range[1];
  vector<double> x_copy(x.begin(), x.end() - res);
  vector<double> y_copy(y.begin(), y.end() - res);
  // No shift;
  tdmi[range[0]] = MIDD(x_copy, y_copy, binsize, binsize, false);
  // Negative shift;
  for (size_t i = 0; i < range[0]; i++) {
    x_copy.erase(x_copy.begin());
    x_copy.insert(x_copy.end(), *(x.end() - res + i));
    tdmi[range[0] - i - 1] = MIDD(x_copy, y_copy, binsize, binsize, false);
  }
  // Positive shift;
  x_copy.clear();
  x_copy.insert(x_copy.end(), x.begin(), x.end() - res);
  for (size_t i = 0; i < range[1]; i++) {
    y_copy.erase(y_copy.begin());
    y_copy.insert(y_copy.end(), *(y.end() - res + i));
    tdmi[range[0] + i + 1] = MIDD(x_copy, y_copy, binsize, binsize, false);
  }
}

void TDMI(vector<bool>& x, vector<double>& y, vector<double>& tdmi, vector<size_t> &range, double binsize) {
  // initialize container of tdmi;
  tdmi.clear();
  tdmi.resize(range[0] + range[1] + 1, 0);
  // prepare series;
  size_t res = range[0];
  if (res < range[1]) res = range[1];
  vector<bool> x_copy(x.begin(), x.end() - res);
  vector<double> y_copy(y.begin(), y.end() - res);
  // No shift;
  tdmi[range[0]] = MIBD(x_copy, y_copy, binsize, false);
  // Negative shift;
  for (size_t i = 0; i < range[0]; i++) {
    x_copy.erase(x_copy.begin());
    x_copy.insert(x_copy.end(), *(x.end() - res + i));
    tdmi[range[0] - i - 1] = MIBD(x_copy, y_copy, binsize, false);
  }
  // Positive shift;
  x_copy.clear();
  x_copy.insert(x_copy.end(), x.begin(), x.end() - res);
  for (size_t i = 0; i < range[1]; i++) {
    y_copy.erase(y_copy.begin());
    y_copy.insert(y_copy.end(), *(y.end() - res + i));
    tdmi[range[0] + i + 1] = MIBD(x_copy, y_copy, binsize, false);
  }
  return;
}

void TDMI2bins(vector<bool>& x, vector<double>& y, vector<double>& tdmi, vector<size_t> &range, double threshold) {
  // initialize container of tdmi;
  tdmi.clear();
  tdmi.resize(range[0] + range[1] + 1, 0);
  // prepare series;
  size_t res = range[0];
  if (res < range[1]) res = range[1];
  vector<bool> x_copy(x.begin(), x.end() - res);
  vector<double> y_copy(y.begin(), y.end() - res);
  // No shift;
  tdmi[range[0]] = MIBD2bins(x_copy, y_copy, threshold);
  // Negative shift;
  for (size_t i = 0; i < range[0]; i++) {
    x_copy.erase(x_copy.begin());
    x_copy.insert(x_copy.end(), *(x.end() - res + i));
    tdmi[range[0] - i - 1] = MIBD2bins(x_copy, y_copy, threshold);
  }
  // Positive shift;
  x_copy.clear();
  x_copy.insert(x_copy.end(), x.begin(), x.end() - res);
  for (size_t i = 0; i < range[1]; i++) {
    y_copy.erase(y_copy.begin());
    y_copy.insert(y_copy.end(), *(y.end() - res + i));
    tdmi[range[0] + i + 1] = MIBD2bins(x_copy, y_copy, threshold);
  }
  return;
}
