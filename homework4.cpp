/**
 * homework4.cpp
 * Authors: Brian Jurgess
 *          Cody Wilson
 * CSE 420
 * Homework 4
 */

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "algorithms.h"

using std::vector;
using std::string;

void runAlgorithmsF(vector<vector<float> *> &datad, vector<vector<double> *> &ddata, std::ofstream &fs, std::ofstream &ds);

int main(int argc, char * argv[]) {

    vector<vector<float> *> data_f;
    for(unsigned i = 0; i < 5; i++) {
        data_f.push_back(new vector<float>());
    }

    // Vector data for double
    vector<vector<double> *> data_d;
    for(unsigned i = 0; i < 5; i++) {
        data_d.push_back(new vector<double>());
    }

    string filename = "Data.csv";            // Filename of the data file
    std::ifstream data_file;                 // Filestream

    data_file.open(filename.c_str());

    string line;
    while(!data_file.eof()) {
        // Get line from the file
        std::getline(data_file, line);

        // Make sure we aren't including the first line
        // or parse an empty line
        if (line.find("Data") != std::string::npos)
            continue;
        else if (line.empty())
            continue;
        // Split the line by comma and put the values into the appropriate
        // vectors
        std::stringstream ss(line);
        string value;
        int pos = 0;
        while(std::getline(ss,value, ',')) {
            double d = strtod(value.c_str(), NULL);
            float  f = strtof(value.c_str(), NULL);
            data_f[pos]->push_back(f);
            data_d[pos]->push_back(d);
            pos++;
        }
    }

    data_file.close();
    
    std::ofstream f_stream;
    std::ofstream d_stream;

    f_stream.open("float_values.txt");
    d_stream.open("double_values.txt");
    // Run the algorithms
    runAlgorithmsF(data_f, data_d, f_stream, d_stream);

    f_stream.close();
    d_stream.close();

    return 0;
}

void output(vector<CalcValues *> v, std::ofstream &fs, std::ofstream &ds) {
    fs << std::setw(8) << "Mean";
    ds << std::setw(8) << "Mean";
    for (unsigned i = 0; i < 5; i++) {
        fs << std::setw(15) << v[i]->mean_f;
        ds << std::setw(15) << v[i]->mean_d;
    }

    fs << std::endl;
    ds << std::endl;
    fs << std::setw(12) << " " << std::setw(8) << "Var";
    ds << std::setw(12) << " " << std::setw(8) << "Var";
    for (unsigned i=0; i < 5; i++) {
        fs << std::setw(15) << v[i]->var_f;
        ds << std::setw(15) << v[i]->var_d;
    }

    fs << std::endl;
    ds << std::endl;
    fs << std::setw(12) << " " << std::setw(8) << "StdDev";
    ds << std::setw(12) << " " << std::setw(8) << "StdDev";
    for (unsigned i=0; i < 5; i++) {
        fs << std::setw(15) << v[i]->stddev_f;
        ds << std::setw(15) << v[i]->stddev_d;
    }
    fs << std::endl;
    ds << std::endl;
}

void runAlgorithmsF(vector<vector<float> *> &data_f, vector<vector<double> *> &data_d, std::ofstream &fs, std::ofstream &ds) {
    int count = 10;

    vector<CalcValues *> values;
    for (unsigned i = 0; i < 5; i++) {
        values.push_back( new CalcValues() );
    }

    for (unsigned i = 0; i < 3; i++) {
        fs <<  std::setfill(' ') << std::setw(12) << std::setiosflags(std::ios::left) << "Algorithm"
            << std::setw(8)  << count
            << std::setw(15) << "Data 1"
            << std::setw(15) << "Data 2"
            << std::setw(15) << "Data 3"
            << std::setw(15) << "Data 4"
            << std::setw(15) << "Data 5" << std::endl;

        ds << std::setfill(' ') << std::setw(12) << std::setiosflags(std::ios::left) << "Algorithm"
            << std::setw(8)  << count
            << std::setw(15) << "Data 1"
            << std::setw(15) << "Data 2"
            << std::setw(15) << "Data 3"
            << std::setw(15) << "Data 4"
            << std::setw(15) << "Data 5" << std::endl;

        fs << std::resetiosflags(std::ios::left) << std::setw(95) << std::setfill('-') << " " << std::endl;
        ds << std::resetiosflags(std::ios::left) << std::setw(95) << std::setfill('-') << " " << std::endl;

        fs << std::setfill(' ') << std::setw(12);
        ds << std::setfill(' ') << std::setw(12);

        fs << std::setprecision(9) << std::setiosflags(std::ios::left) << "Two Pass";
        ds << std::setprecision(9) << std::setiosflags(std::ios::left) << "Two Pass";
        for (unsigned j=0; j < 5; j++) {
            TwoPass(*data_f[j], *data_d[j], count, values[j]);
        }
        output(values, fs, ds);
        fs << std::endl;
        ds << std::endl;

        fs << std::setfill(' ') << std::setw(12);
        ds << std::setfill(' ') << std::setw(12);

        fs << std::setiosflags(std::ios::left) << "West's";
        ds << std::setiosflags(std::ios::left) << "West's";
        for (unsigned j=0; j < 5; j++) {
            West(*data_f[j], *data_d[j], count, values[j]);
        }
        output(values, fs, ds);
        fs << std::endl;
        ds << std::endl;

        fs << std::setfill(' ') << std::setw(12);
        ds << std::setfill(' ') << std::setw(12);

        fs << std::setiosflags(std::ios::left) << "Textbook";
        ds << std::setiosflags(std::ios::left) << "Textbook";
        for (unsigned j=0; j < 5; j++) {
            Textbook(*data_f[j], *data_d[j], count, values[j]);
        }
        output(values, fs, ds);
        fs << std::endl;
        ds << std::endl;

        fs << std::setfill(' ') << std::setw(12);
        ds << std::setfill(' ') << std::setw(12);

        fs << std::setiosflags(std::ios::left) << "Cotton's";
        ds << std::setiosflags(std::ios::left) << "Cotton's";
        for (unsigned j=0; j < 5; j++) {
            Cotton(*data_f[j], *data_d[j], count, values[j]);
        }
        output(values, fs, ds);


        count = count * 10;
        fs << std::endl;
        ds << std::endl;
        
        fs << std::endl;
        ds << std::endl;
    }
}


