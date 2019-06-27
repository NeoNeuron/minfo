//***************
//	Copyright: Kyle Chen
//	Author: Kyle Chen
//	Date: 2018-01-26
//	Description: Mutual information analysis program, between spikes and spikes;
//***************
#include "mi_uniform.h"
#include "io.h"
#include "common_header.h"
#include <stdexcept>
#include <boost/program_options.hpp>
using namespace std;
namespace po = boost::program_options;
// compact function to calculate mutual information between multi-type signal
int main(int argc, const char* argv[]) {
	clock_t start, finish;
	start = clock();
	// Config program options:
	po::options_description desc("All Options");
	desc.add_options()
		("help,h", "show help message")
		("prefix", po::value<string>()->default_value("./"), "prefix of output files")
		// TDMI types
		("type,t", po::value<string>(), "type of MI, including : (with order)\nBB : bool & bool\nBD : bool & double\nDD : double & double")
		// input files
		("ifile,i", po::value<vector<string> >()->multitoken(), "[positional] : input data files")
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
	if (type == "BB") {
		// INPUT NEURONAL DATA:
		vector<bool> x, y;
		Read1DBin(dir + ifilename1, x, 0, 0);
		Read1DBin(dir + ifilename2, y, 0, 0);
		TDMI(x, y, tdmi, range);
	} else if (type == "BD") {
		vector<bool> bool_series;
		vector<double> double_series;
		Read1DBin(dir + ifilename1, bool_series, 0, 0);
		Read1DBin(dir + ifilename2, double_series, 0, 0);
		double binsize = vm["dx"].as<double>();
		TDMI(bool_series, double_series, tdmi, range, binsize);
	} else if (type == "BD2") {
		vector<bool> bool_series;
		vector<double> double_series;
		Read1DBin(dir + ifilename1, bool_series, 0, 0);
		Read1DBin(dir + ifilename2, double_series, 0, 0);
		double threshold = vm["dx"].as<double>();
		TDMI2bins(bool_series, double_series, tdmi, range, threshold);
	} else if (type == "DD") {
		vector<double> double_series_1, double_series_2;
		Read1DBin(dir + ifilename1, double_series_1, 0, 0);
		Read1DBin(dir + ifilename2, double_series_2, 0, 0);
		double binsize = vm["dx"].as<double>();
		vector<double> tdmi;
		TDMI(double_series_1, double_series_2, tdmi, range, binsize);
	} else {
		cout << "ERROR: invalid TDMI type\n";
		return -1;
	}
	//	Output data:
	ofstream data_out;
	ofilename = dir + ofilename;
	data_out.open(ofilename.c_str());
	data_out << "#timelag,mi" << endl;
	for (int i = 0; i < range[0] + range[1] + 1; i++) {
		data_out << i - (int)range[0] << ',' << setprecision(15) << (double)tdmi[i] << '\n';
	}
	data_out.close();

	finish = clock();
	// Time counting:
	printf(">> It takes %3.3f s\n", (finish - start)*1.0 / CLOCKS_PER_SEC);
	return 0;
}
