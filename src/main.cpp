//***************
//  Copyright: Kyle Chen
//  Author: Kyle Chen
//  Date: 2018-01-26
//  Description: Mutual information analysis program, between spikes and spikes;
//***************
#include "mi_uniform.h"
#include "io.h"
#include "common_header.h"
#include <stdexcept>
#include <boost/program_options.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xnpy.hpp>
using namespace std;
namespace po = boost::program_options;
// compact function to calculate mutual information between multi-type signal
int main(int argc, const char* argv[]) {
  clock_t start, finish;
  start = clock();
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
      vector<int> x, y;
      auto x_x = xt::load_npy<int>(dir + ifilename1);
      auto x_y = xt::load_npy<int>(dir + ifilename2);
      //Read1DBin(dir + ifilename1, x, 0, 0);
      //Read1DBin(dir + ifilename2, y, 0, 0);
      x.insert(x.end(), &x_x(0), &x_x(x_x.shape()[0]-1));
      y.insert(y.end(), &x_y(0), &x_y(x_y.shape()[0]-1));
      finish = clock();
      if (verbose) {
        printf(">> Reading data takes %3.3f s\n", (finish - start)*1.0 / CLOCKS_PER_SEC);
      }
      start = clock();
      TDMI(x, y, tdmi, range);
      finish = clock();
      if (verbose) {
        printf(">> Calculation takes %5.3e s\n", (finish - start)*1.0 / CLOCKS_PER_SEC);
      }
    } else if (type == "BD") {
      vector<int> binary_series;
      vector<double> double_series;
      auto x_binary_series = xt::load_npy<int>(dir + ifilename1);
      auto x_double_series = xt::load_npy<double>(dir + ifilename2);
      //Read1DBin(dir + ifilename1, binary_series, 0, 0);
      //Read1DBin(dir + ifilename2, double_series, 0, 0);
      binary_series.insert(binary_series.end(), &x_binary_series(0), &x_binary_series(x_binary_series.shape()[0]-1));
      double_series.insert(double_series.end(), &x_double_series(0), &x_double_series(x_double_series.shape()[0]-1));
      double binsize = vm["dx"].as<double>();
      finish = clock();
      if (verbose) {
        printf(">> Reading data takes %5.3e s\n", (finish - start)*1.0 / CLOCKS_PER_SEC);
      }
      start = clock();
      TDMI(binary_series, double_series, tdmi, range, binsize);
      finish = clock();
      if (verbose) {
        printf(">> Calculation takes %5.3e s\n", (finish - start)*1.0 / CLOCKS_PER_SEC);  
      }
    } else if (type == "BD2") {
      vector<int> binary_series;
      vector<double> double_series;
      auto x_binary_series = xt::load_npy<int>(dir + ifilename1);
      auto x_double_series = xt::load_npy<double>(dir + ifilename2);
      //Read1DBin(dir + ifilename1, binary_series, 0, 0);
      //Read1DBin(dir + ifilename2, double_series, 0, 0);
      binary_series.insert(binary_series.end(), &x_binary_series(0), &x_binary_series(x_binary_series.shape()[0]-1));
      double_series.insert(double_series.end(), &x_double_series(0), &x_double_series(x_double_series.shape()[0]-1));
      double threshold = vm["dx"].as<double>();
      TDMI2bins(binary_series, double_series, tdmi, range, threshold);
    } else if (type == "DD") {
      vector<double> double_series_1, double_series_2;
      auto x_double_series_1 = xt::load_npy<double>(dir + ifilename1);
      auto x_double_series_2 = xt::load_npy<double>(dir + ifilename2);
      //Read1DBin(dir + ifilename1, double_series_1, 0, 0);
      //Read1DBin(dir + ifilename2, double_series_2, 0, 0);
      double binsize = vm["dx"].as<double>();
      double_series_1.insert(double_series_1.end(), &x_double_series_1(0), &x_double_series_1(x_double_series_1.shape()[0]-1));
      double_series_2.insert(double_series_2.end(), &x_double_series_2(0), &x_double_series_2(x_double_series_2.shape()[0]-1));
      TDMI(double_series_1, double_series_2, tdmi, range, binsize);
    } else {
      throw invalid_argument("invalid TDMI type");
    }
  } catch(const exception& e) {
    cerr << e.what() << '\n';
  }
  //  Output data:
  start = clock();
  ofstream data_out;
  ofilename = dir + ofilename;
  data_out.open(ofilename.c_str());
  data_out << "#timelag,mi" << endl;
  for (size_t i = 0; i < range[0] + range[1] + 1; i++) {
    data_out << i - (int)range[0] << ',' << setprecision(15) << (double)tdmi[i] << '\n';
  }
  data_out.close();

  finish = clock();
  // Time counting:
  if (verbose) {
    printf(">> Saving data takes %5.3e s\n", (finish - start)*1.0 / CLOCKS_PER_SEC);
  }
  return 0;
}
