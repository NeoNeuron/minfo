//***************
//	Copyright: Kyle Chen
//	Author: Kyle Chen
//	Date: 2018-07-29
//	Description: Library for Data I/O functions in project;
//***************
#ifndef _IO_H_
#define _IO_H_

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstdio>
using namespace std;

//	Read 2 dimensional information;
	//	Read 2-D data from *.csv files; data type can be int or double;
  //	VECTOR<VECTOR<T> > data: container of data;
	//	STRING path: path of target file;
	//	Return: none;
template <class T> void Read2D(string path, vector<vector<T> >& data);
template <class T> void Read2DBin(string path, vector<vector<T> >& data);
//	Read 1 dimensional (row/column) from *.csv file;
  //	STRING path: file path of *.csv file;
  //	VECTOR<INT/DOUBLE> data: storage of output data;
  //	INT index: the index of chosen row/column, from 0 to maximum - 1;
  //  INT axis: 0 for row, 1 for column;
  //	Return: none;
template <class T> void Read1D(string path, vector<T>& data, int index, int axis);
template <class T> void Read1DBin(string path, vector<T>& data, int index, int axis);

//  Print 2 dimensional data;
  //  Print 2-D data to *.csv files; data type can be int, double or bool;
  //	VECTOR<VECTOR<T> > data: container of data;
  //  STRING mode: openmode for aiming file; "app" for append, "trunc" for any contents that existed in the file before it is open are discarded.
  //  Return: none;
template <class T> void Print2D(string path, vector<vector<T> >& data, string mode);
template <class T> void Print2DBin(string path, vector<vector<T> >& data, string mode);

//	Print 1 dimensional (row/column) to *.csv file;
  //	STRING path: file path of *.csv file;
  //	VECTOR<INT/DOUBLE> data: storage of outputing data;
  //  STRING mode: openmode for aiming file; "app" for append, "trunc" for any contents that existed in the file before it is open are discarded.
  //  INT axis: 0 for row, 1 for column;
  //	Return: none;
template <class T> void Print1D(string path, vector<T>& data, string mode, int axis);
template <class T> void Print1DBin(string path, vector<T>& data, string mode);

class FILEWRITE {
	private:
		ofstream ofile_;
		string file_path_;
	public:
		FILEWRITE(string file_path, string write_mode = "trunc") {
			file_path_ = file_path;
			if (write_mode == "trunc") {
				ofile_.open(file_path.c_str(), ios::binary);
			} else if (write_mode == "add") {
				ofile_.open(file_path.c_str(), ios::binary | ios::app);
			} else {
				throw runtime_error(file_path + " wrong write mode");
			}
		}
		~FILEWRITE() {
			if (ofile_.is_open()) ofile_.close();
		}
		
		// Initialize the size of array:
		void SetSize(size_t *size) {
			ofile_.write((char*)size, 2*sizeof(size_t));
		}
		
		// Write 1-D vector to file;
		template <class T> void Write(vector<T>& data) {
			T tmp;
			for (typename vector<T>::iterator it = data.begin(); it != data.end(); it ++) {
				tmp = *it;
				ofile_.write((char*)&tmp, sizeof(T));
			}
		}

		// Manually close the file and delete it;
		void Remove() {
			ofile_.close();
			remove(file_path_.c_str());
		}

};


#include "io.hpp"

#endif // _IO_H_
