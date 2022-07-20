//***************
//  Copyright: Kyle Chen
//  Author: Kyle Chen
//  Created: 2018-01-26
//  Description: Mutual information analysis program, between spikes and spikes;
//***************
#include "mi_uniform.h"
#include "io.h"
#include <chrono>
#include <boost/program_options.hpp>
#include <cnpy.h>
//
//struct NpyArray {
//    std::vector<size_t> shape;
//    size_t word_size;
//    template<typename T> T* data();
//};
//
using namespace std;
namespace po = boost::program_options;
// compact function to calculate mutual information between multi-type signal
int main(int argc, const char* argv[]) {
  auto start = chrono::system_clock::now();
  chrono::system_clock::time_point finish;
  chrono::duration<double> escaped_seconds;
  // Config program options:
  bool verbose;
  po::options_description desc("All Options");
  desc.add_options()
    ("help,h", "show help message")
    ("verbose,v", po::bool_switch(&verbose), "enable verbose")
    ("prefix", po::value<string>()->default_value("./"), "prefix of output files")
    // TDMI types
    ("type,t", po::value<string>(), "type of MI, including : (with order)\nBB : bool & bool\nBD : bool & double\nDD : double & double")
    // input files
    ("ifile,i", po::value<vector<string> >()->multitoken(), "[positional] : input data files, as numpy *.npy format.")
    // output file
    ("ofile,o", po::value<string>(), "[positional] : output TDMI file")
    // delay range
    ("drange", po::value< vector<size_t> >()->multitoken(), "time delay time range")
    // bin size of probability distribution function
    ("dx", po::value<double>(), "bin size of probability distribution function")
    ;
  po::positional_options_description pos_desc;
  pos_desc.add("ifile", 2);
  pos_desc.add("ofile", 1);
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).positional(pos_desc).run(), vm);
  po::notify(vm);
  if (vm.count("help")) {
    printf("Usage: cal-mi [-c option_argument] ifile[0] ifile[1] ofile\n");
    cout << desc << '\n';
    return 1;
  }
  // processing input args;
  string dir = vm["prefix"].as<string>();
  vector<string> ifilename = vm["ifile"].as< vector<string> >();
  string ofilename  = vm["ofile"].as<string>();
  string ifilename1 = ifilename[0], ifilename2 = ifilename[1];
  vector<size_t> range = vm["drange"].as< vector<size_t> >();
  string type = vm["type"].as<string>();

  // start main loop
  vector<double> tdmi;
  try {
    if (type == "BB") {
      // INPUT NEURONAL DATA:
      auto arr_x = cnpy::npy_load(dir + ifilename1);
      auto arr_y = cnpy::npy_load(dir + ifilename2);
      int* px = arr_x.data<int>();
      int* py = arr_y.data<int>();
      vector<int> x(px, px + arr_x.shape.front());
      vector<int> y(py, py + arr_y.shape.front());
      finish = chrono::system_clock::now();
      if (verbose) {
        escaped_seconds = finish - start;
        printf(">> Reading data takes %5.3e s\n", escaped_seconds.count());
      }
      start = chrono::system_clock::now();
      TDMI(x, y, tdmi, range);
      finish = chrono::system_clock::now();
      if (verbose) {
        escaped_seconds = finish - start;
        printf(">> Calculation takes %5.3e s\n", escaped_seconds.count());
      }
    } else if (type == "BD") {
      auto arr_x = cnpy::npy_load(dir + ifilename1);
      auto arr_y = cnpy::npy_load(dir + ifilename2);
      int* px = arr_x.data<int>();
      double* py = arr_y.data<double>();
      vector<int> binary_series(px, px + arr_x.shape.front());
      vector<double> double_series(py, py + arr_y.shape.front());
      double binsize = vm["dx"].as<double>();
      finish = chrono::system_clock::now();
      if (verbose) {
        escaped_seconds = finish - start;
        printf(">> Reading data takes %5.3e s\n", escaped_seconds.count());
      }
      start = chrono::system_clock::now();
      TDMI(binary_series, double_series, tdmi, range, binsize);
      finish = chrono::system_clock::now();
      if (verbose) {
        escaped_seconds = finish - start;
        printf(">> Calculation takes %5.3e s\n", escaped_seconds.count());
      }
    } else if (type == "BD2") {
      start = chrono::system_clock::now();
      auto arr_x = cnpy::npy_load(dir + ifilename1);
      auto arr_y = cnpy::npy_load(dir + ifilename2);
      int* px = arr_x.data<int>();
      double* py = arr_y.data<double>();
      vector<int> binary_series(px, px + arr_x.shape.front());
      vector<double> double_series(py, py + arr_y.shape.front());
      double threshold = vm["dx"].as<double>();
      finish = chrono::system_clock::now();
      if (verbose) {
        escaped_seconds = finish - start;
        printf(">> Reading data takes %5.3e s\n", escaped_seconds.count());
      }
      start = chrono::system_clock::now();
      TDMI2bins(binary_series, double_series, tdmi, range, threshold);
      finish = chrono::system_clock::now();
      if (verbose) {
        escaped_seconds = finish - start;
        printf(">> Calculation takes %5.3e s\n", escaped_seconds.count());
      }
    } else if (type == "DD") {
      auto arr_x = cnpy::npy_load(dir + ifilename1);
      auto arr_y = cnpy::npy_load(dir + ifilename2);
      double* px = arr_x.data<double>();
      double* py = arr_y.data<double>();
      vector<double> double_series_1(px, px + arr_x.shape.front());
      vector<double> double_series_2(py, py + arr_y.shape.front());
      double binsize = vm["dx"].as<double>();
      finish = chrono::system_clock::now();
      if (verbose) {
        escaped_seconds = finish - start;
        printf(">> Reading data takes %5.3e s\n", escaped_seconds.count());
      }
      start = chrono::system_clock::now();
      TDMI(double_series_1, double_series_2, tdmi, range, binsize);
      finish = chrono::system_clock::now();
      if (verbose) {
        escaped_seconds = finish - start;
        printf(">> Calculation takes %5.3e s\n", escaped_seconds.count());
      }
    } else {
      throw invalid_argument("invalid TDMI type");
    }
  } catch(const exception& e) {
    cerr << e.what() << '\n';
  }
  //  Output data:
  start = chrono::system_clock::now();
  ofstream data_out;
  ofilename = dir + ofilename;
  data_out.open(ofilename.c_str());
  data_out << "#timelag,mi" << endl;
  for (size_t i = 0; i < range[0] + range[1] + 1; i++) {
    data_out << i - (int)range[0] << ',' << setprecision(15) << (double)tdmi[i] << '\n';
  }
  data_out.close();
  finish = chrono::system_clock::now();
  // Time counting:
  if (verbose) {
    escaped_seconds = finish - start;
    printf(">> Saving data takes %5.3e s\n", escaped_seconds.count());
  }
  return 0;
}
