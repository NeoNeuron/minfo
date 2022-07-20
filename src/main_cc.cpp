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

/*
 * Cross-correlation
 * (f * g)(t') = \int f(t)*g(t+t')dt
*/
inline double CC(vector<double>& x, vector<double>& y) {
  return inner_product(x.begin(), x.end(), y.begin(), 0.0);
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
		("ofile,o", po::value<string>(), "[positional] : output Cross-Correlation file")
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

  auto arr_x = cnpy::npy_load(dir + ifilename1);
  auto arr_y = cnpy::npy_load(dir + ifilename2);
  double* px = arr_x.data<double>();
  double* py = arr_y.data<double>();
  vector<double> data_series_1(px, px + arr_x.shape.front());
  vector<double> data_series_2(py, py + arr_y.shape.front());
  // Set time range;
	int ntd = range[0];
	int ptd = range[1];
  // Calculate CC:
  vector<double> sta(ntd + ptd + 1);
  size_t res = ntd > ptd ? ntd : ptd;
	vector<double> data_series_1_copy(data_series_1.begin(), data_series_1.end() - res);
	vector<double> data_series_2_copy(data_series_2.begin(), data_series_2.end() - res);
  // No shift;
	sta[ntd] = CC(data_series_1_copy, data_series_2_copy);
	// Negative shift;
	for (size_t i = 0; i < ntd; i++) {
		data_series_1_copy.erase(data_series_1_copy.begin());
		data_series_1_copy.insert(data_series_1_copy.end(), *(data_series_1.end() - res + i));
		sta[ntd - i - 1] = CC(data_series_1_copy, data_series_2_copy);
	}
	// Positive shift;
	data_series_1_copy.clear();
	data_series_1_copy.insert(data_series_1_copy.end(), data_series_1.begin(), data_series_1.end() - res);
	for (size_t i = 0; i < ptd; i++) {
		data_series_2_copy.erase(data_series_2_copy.begin());
		data_series_2_copy.insert(data_series_2_copy.end(), *(data_series_2.end() - res + i));
		sta[ntd + i + 1] = CC(data_series_1_copy, data_series_2_copy);
	}
  // Output CC:
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
