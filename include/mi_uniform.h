//***************
//	Copyright: Kyle Chen
//	Author: Kyle Chen
//	Date: 2018-02-11
//	Description: Define algorithm to estimate Mutual Information(MI) and Time-Delayed Mutual Information(TDMI);
//***************
#ifndef _IFNET_MI_UNIFORM_H_
#define _IFNET_MI_UNIFORM_H_

#include <string>
#include <vector>
using namespace std;

// Joint probability function of binary variable x and y;
// VECTOR<BOOL> binary_spikes y: binary variable y;
// VECTOR<BOOL> binary_spikes x: binary variable x;
// VECTOR<VECTOR<DOUBLE> jointpdf: joint probability distribution function of x and y;
// Return: none;
void JointPDF(vector<bool>& x, vector<bool>& y, vector<vector<double> >& jointpdf);

// Joint probability function with uniform partition of double variable x and y;
// VECTOR<DOUBLE> x: continuous double variable x;
// VECTOR<DOUBLE> y: continuous double variable y;
// SIZE_T x_bin_num: number of uniform partitions of x;
// SIZE_T y_bin_num: number of uniform partitions of y;
// VECTOR<VECTOR<DOUBLE> jointpdf: joint probability distribution function of x and y;
// Return: none;
void JointPDF(vector<double>& x, vector<double>& y, vector<vector<double> >& jointpdf, double x_binsize, double y_binsize);

// Joint probability function with uniform partition of x and y;
// VECTOR<BOOL> binary_spikes: binary spike trains;
// VECTOR<DOUBLE> lfp: continuous local field potential;
// VECTOR<VECTOR<DOUBLE> jointpdf: joint probability distribution function of spike train and lfp;
// SIZE_T bin_num: number of uniform partitions of x;
// DOUBLE threshold: threshold of pdf of lfp with uniform partitions;
// Return: none;
void JointPDF(vector<bool>& binary_spikes, vector<double>& lfp, vector<vector<double> >& jointpdf, double binsize);
void JointPDF2bins(vector<bool>& binary_spikes, vector<double>& lfp, vector<vector<double> >& jointpdf, double threshold);

// Marginal probability density function of bivariable joint distribution density function;
// Return: none;
void MarginalPDF(vector<vector<double> >& jointpdf, vector<double>& p1, vector<double>& p2);

// Mutual information:
// VECTOR<VECTOR<DOUBLE> > jointpdf: joint probability distribution of bivariate date;
// Return: value of mutual information;
double MI(vector<vector<double> >& jointpdf);

// Mutual information of two binary spike trains;
// VECTOR<BOOL> x, y: tow original bool sequence;
// Return: value of mutual information;
double MIBB(vector<bool>& x, vector<bool>& y);

// Mutual information of two double sequences;
// VECTOR<DOUBLE> x, y: two original double sequences;
// DOUBLE* x_max_and_min, y_max_and_min: set of maximum and minimum value; *_max_and_min[0] = *_max; *_max_and_min[1] = *_min;
// DOUBLE x_bin_width, y_bin_width: bin width of histograms of x and y;
// Return: value of mutual information;
double MIDD(vector<double>& x, vector<double>& y, double x_binsize, double y_binsize, bool pdf_output_flag);

// Mutual information between binary spiking train and LFP with uniform binning size;
// VECTOR<BOOL> binary_spikes: original spike train with binary version;
// VECTOR<DOUBLE> LFP: local field potential;
// INT bin_number: number of bins of historgram of LFP;
// Return: valude of Mutual information;
double MIBD(vector<bool>& bool_series, vector<double>& double_series, double binsize, bool pdf_output_flag);
// Threshold: the seperation line between two bins in the pdf of continuous double variable;
// Return: valude of Mutual information;
double MIBD2bins(vector<bool>& bool_series, vector<double>& double_series, double threshold);

// Time-delayed mutual information between two spike trains;
void TDMI(vector<bool>& x, vector<bool>& y, vector<double> & tdmi, vector<size_t> &range);

// Delayed mutual information of two double series;
void TDMI(vector<double>& x, vector<vector<double> >& y, vector<double>& tdmi, double x_binsize, double y_binsize);

// Delayed mutual information of two double variables;
void TDMI(vector<double>& x, vector<double>& y, vector<double>& tdmi, vector<size_t> &range, double binsize);
// Delayed mutual information of spike train and LFP with direct scheme;
void TDMI(vector<bool>& x, vector<double>& y, vector<double>& tdmi, vector<size_t> &range, double binsize);
void TDMI2bins(vector<bool>& x, vector<double>& y, vector<double>& tdmi, vector<size_t> &range, double threshold);

#endif // _IFNET_MI_UNIFORM_H_
