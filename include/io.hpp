//***************
//	Copyright: Kyle Chen
//	Author: Kyle Chen
//	Date: 2017-08-29
//	Description: Library for Data I/O functions in project;
//***************
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

//	Read 2 dimensional information;
	//	Read 2-D data from *.csv files; data type can be int or double;
	//	STRING path: path of target file;
	//	VECTOR<VECTOR<T> > data: container of data;
	//	Return: none;
template <class T> void Read2D(string path, vector<vector<T> >& data) {
  data.clear();
  ifstream ifile(path.c_str());
  string s, buffer;
  vector<T> add_T;
  istringstream s_input;
  while (getline(ifile, s)) {
    add_T.clear();
    s_input.clear();
    s_input.str("");
    s_input.str(s);
    while (getline(s_input, buffer, ',')) {
      if (buffer.find('.') == string::npos) {
        add_T.push_back(atoi(buffer.c_str()));
      } else add_T.push_back(atof(buffer.c_str()));
    }
    data.push_back(add_T);
  }
  ifile.close();
}

template <class T> void Read2DBin(string path, vector<vector<T> >& data) {
  // open data file;
  ifstream ifile(path.c_str(), ios::binary);
  // Read the shape of 2D data array:
  size_t shape[2];
  ifile.read((char*)&shape, 2*sizeof(size_t));
  // Clear the inputing vector;
  data.clear();
  data.resize(shape[0], vector<T>(shape[1]));
  T buffer;
  size_t size = sizeof(buffer);
  for (size_t i = 0; i < shape[0]; i ++) {
    for (size_t j = 0; j < shape[1]; j ++) {
      ifile.read((char*)&buffer, size);
      data[i][j] = buffer;
    }
  }
  ifile.close();
}

template <class T> void Read1D(string path, vector<T>& data, int index, int axis) {
  data.clear();
  // open data file;
  ifstream ifile(path.c_str());
  if (axis == 0) {
    string s, buffer;
    size_t row_counter = 0;
    istringstream s_input;
    while (getline(ifile, s)) {
      if (row_counter == index) {
        s_input.clear();
        s_input.str(s);
        while (getline(s_input, buffer, ',')) {
          if (buffer.find('.') == string::npos) {
            data.push_back(atoi(buffer.c_str()));
          } else data.push_back(atof(buffer.c_str()));
        }
				break;
      }
      row_counter ++;
    }
  } else {
    string s, buffer;
    size_t column_counter;
    istringstream s_input;
    while (getline(ifile, s)) {
      column_counter = 0;
      s_input.clear();
      s_input.str(s);
      while (getline(s_input, buffer, ',')) {
        if (column_counter == index) {
          if (buffer.find('.') == string::npos) {
            data.push_back(atoi(buffer.c_str()));
          } else data.push_back(atof(buffer.c_str()));
          break;
        }
        column_counter ++;
      }
    }
  }
  ifile.close();
}

template <class T> void Read1DBin(string path, vector<T>& data, int index, int axis) {
  // open data file;
  ifstream ifile(path.c_str(), ios::binary);
  // Read the shape of 2D data array:
  size_t shape[2];
  ifile.read((char*)&shape, 2*sizeof(size_t));
  // clear the data vector;
  data.clear();
	vector<double> buffer_vec(shape[0]*shape[1]);
  T buffer;
  if (axis == 0) {
    data.resize(shape[1]);
    size_t prefix = index * shape[1] * sizeof(T);
    ifile.read((char*)buffer_vec.data(), prefix);
		//for (int i = 0; i < index; i ++) ifile.read((char*)buffer_vec.data(), shape[1]*sizeof(T));
    for (int i = 0; i < shape[1]; i ++) {
      ifile.read((char*)&buffer, sizeof(T));
      data[i] = buffer;
    }
  } else {
    ifile.read((char*)buffer_vec.data(), index * sizeof(T));
    data.resize(shape[0]);
    for (size_t i = 0; i < shape[0]; i ++) {
      ifile.read((char*)&buffer, sizeof(T));
      data[i] = buffer;
      ifile.read((char*)buffer_vec.data(), (shape[1] - 1) * sizeof(T));
    }
  }
  ifile.close();
}

template <class T> void Print2D(string path, vector<vector<T> >& data, string mode) {
  ofstream ofile;
  if (mode == "app") ofile.open(path.c_str(), ios::app);
  else if (mode == "trunc") ofile.open(path.c_str());
  ostringstream s_out;
  for (typename vector<vector<T> >::iterator it = data.begin(); it != data.end(); it++) {
    s_out.str("");
    for (typename vector<T>::iterator itt = it->begin(); itt != it->end(); itt++) {
      s_out << setprecision(15) << *itt << ',';
    }
    s_out << '\n';
    ofile << s_out.str();
  }
  ofile.close();
}

template <class T> void Print2DBin(string path, vector<vector<T> >& data, string mode) {
  ofstream ofile;
  if (mode == "app") ofile.open(path.c_str(), ios::binary|ios::app);
  else if (mode == "trunc") {
    ofile.open(path.c_str(), ios::binary);
    size_t buffer[2] = {data.size(), data.begin()->size()};
    ofile.write((char*)&buffer, 2*sizeof(size_t));
  }
	T tmp;
  for (typename vector<vector<T> >::iterator it = data.begin(); it != data.end(); it++) {
		for (typename vector<T>:: iterator itt = it -> begin(); itt != it -> end(); itt ++) {
			tmp = *itt;
			ofile.write((char*)&tmp, sizeof(T));
		}
  }
  ofile.close();
}

template <class T> void Print1D(string path, vector<T>& data, string mode, int axis) {
  ofstream ofile;
  if (mode == "app") ofile.open(path.c_str(), ios::app);
  else if (mode == "trunc") ofile.open(path.c_str());
  ostringstream s_out;
  if (axis == 0) {
    for (typename vector<T>::iterator it = data.begin(); it != data.end(); it++) {
      s_out << setprecision(15) << *it << ',';
    }
    s_out << '\n';
    ofile << s_out.str();
  } else {
    for (typename vector<T>::iterator it = data.begin(); it != data.end(); it++) {
      s_out << setprecision(15) << *it << '\n';
    }
    ofile << s_out.str();
  }
  ofile.close();
}

template <class T> void Print1DBin(string path, vector<T>& data, string mode) {
  ofstream ofile;
  if (mode == "app") ofile.open(path.c_str(), ios::binary|ios::app);
  else if (mode == "trunc") {
    ofile.open(path.c_str(), ios::binary);
    size_t buffer[2] = {1, data.size()};
    ofile.write((char*)&buffer, 2*sizeof(size_t));
  }
  T tmp;
  for (typename vector<T>::iterator it = data.begin(); it != data.end(); it ++) {
    tmp = *it;
    ofile.write((char*)&tmp, sizeof(T));
  }
  //ofile.write((char*)data.data(), data.size() * sizeof(T));
  ofile.close();
}
