//***************
//  Copyright: Kyle Chen
//  Author: Kyle Chen
//  Created: 2018-01-26
//  Description: Spike triggled average calculation program, between spikes and spikes;
//***************
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <stdexcept>
#include <chrono>
#include <boost/program_options.hpp>
#include "cnpy.h"
using namespace std;
namespace po = boost::program_options;

inline double STA(vector<int>& x, vector<double>& y) {
  return inner_product(x.begin(), x.end(), y.begin(), 0.0) / accumulate(x.begin(), x.end(), 0.0);
}

int main(int argc, const char* argv[]) {
  auto start = chrono::system_clock::now();
  // Config program options:
  bool verbose;
  po::options_description desc("All Options");
  desc.add_options()
    ("help,h", "show help message")
    ("verbose,v", po::bool_switch(&verbose), "enable verbose")
    ("prefix", po::value<string>()->default_value("./"), "prefix of output files")
    // input files
    ("ifile,i", po::value<vector<string> >()->multitoken(), "[positional] : input data files, as numpy *.npy format.")
    // output file
    ("ofile,o", po::value<string>(), "[positional] : output TDMI file")
    // delay range
    ("drange", po::value< vector<size_t> >()->multitoken(), "time delay time range")
    ;
  po::positional_options_description pos_desc;
  pos_desc.add("ifile", 2);
  pos_desc.add("ofile", 1);
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).positional(pos_desc).run(), vm);
  po::notify(vm);
  if (vm.count("help")) {
    printf("Usage: cal-sta [-c option_argument] ifile[0] ifile[1] ofile\n");
    cout << desc << '\n';
    return 1;
  }
  // Loading data;
  string dir = vm["prefix"].as<string>();
  vector<string> ifilename = vm["ifile"].as< vector<string> >();
  string ofilename  = vm["ofile"].as<string>();
  string ifilename1 = ifilename[0], ifilename2 = ifilename[1];
  vector<size_t> range = vm["drange"].as< vector<size_t> >();

  auto arr_spike = cnpy::npy_load(dir + ifilename1);
  auto arr_lfp = cnpy::npy_load(dir + ifilename2);
  int* p_spike = arr_spike.data<int>();
  double* p_lfp = arr_lfp.data<double>();
  vector<int> spike(p_spike, p_spike + arr_spike.shape.front());
  vector<double> lfp(p_lfp, p_lfp + arr_lfp.shape.front());
  // Set time range;
  int ntd = range[0];
  int ptd = range[1];
  // Calculate STA:
  vector<double> sta(ntd + ptd + 1);
  size_t res = ntd > ptd ? ntd : ptd;
  vector<int> spike_copy(spike.begin(), spike.end() - res);
  vector<double> lfp_copy(lfp.begin(), lfp.end() - res);
  // No shift;
  sta[ntd] = STA(spike_copy, lfp_copy);
  // Negative shift;
  for (size_t i = 0; i < ntd; i++) {
    spike_copy.erase(spike_copy.begin());
    spike_copy.insert(spike_copy.end(), *(spike.end() - res + i));
    sta[ntd - i - 1] = STA(spike_copy, lfp_copy);
  }
  // Positive shift;
  spike_copy.clear();
  spike_copy.insert(spike_copy.end(), spike.begin(), spike.end() - res);
  for (size_t i = 0; i < ptd; i++) {
    lfp_copy.erase(lfp_copy.begin());
    lfp_copy.insert(lfp_copy.end(), *(lfp.end() - res + i));
    sta[ntd + i + 1] = STA(spike_copy, lfp_copy);
  }
  // Output STA:
  string ofile = dir + ofilename;
  ofstream data_out;
  data_out.open(ofile.c_str());
  data_out << "#timelag,sta" << endl;
  for (int i = 0; i < ntd + ptd + 1; i++) {
    data_out << i - ntd << ',' << setprecision(15) << (double)sta[i] << '\n';
  }
  data_out.close();

  // Timer OFF
  auto finish = chrono::system_clock::now();
  chrono::duration<double> elapsed_seconds = finish-start;
  if (verbose) {
    printf(">> It takes %.2fs\n", elapsed_seconds.count());
  }
  return 0;
}
