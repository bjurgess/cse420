/**
 * File: algorithms.h
 * Authors: Brian Jurgess
 *          Cody Wilson
 */
#include <vector>

using std::vector;

struct CalcValues {
    double mean_d;
    double var_d;
    double stddev_d;

    float mean_f;
    float var_f;
    float stddev_f;
};

void TwoPass(vector<float> &fdata, vector<double> &ddata, int num_rows, CalcValues *);
void West(vector<float> &fdata, vector<double> &ddata, int num_rows, CalcValues *);
void Textbook(vector<float> &fdata, vector<double> &ddata, int num_rows, CalcValues *);
void Cotton(vector<float> &fdata, vector<double> &ddata, int num_rows, CalcValues *);
