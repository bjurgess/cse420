/**
 * File: calculations.cpp
 * Authors: Cody Wilson
 *          Brian Jurgess
 */
#include <vector>
#include <math.h>
#include "algorithms.h"

void TwoPass(vector<float> &fdata, vector<double> &ddata, int num_rows, CalcValues *values)
{
    float xbarf = 0, s2f = 0;
    double xbard = 0, s2d = 0;
    for(int i = 0; i < num_rows; ++i)
    {
        xbarf += fdata[i];
    }
    xbarf = xbarf/num_rows;

    for(int i = 0; i < num_rows; ++i)
    {
        xbard += ddata[i];
    }
    xbard = xbard/num_rows;

    for(int i = 0; i < num_rows; ++i)
    {
        s2f += ((fdata[i] - xbarf)*(fdata[i] - xbarf));
    }
    s2f = s2f/(num_rows-1);

    for(int i = 0; i < num_rows; ++i)
    {
        s2d += ((ddata[i] - xbard)*(ddata[i] - xbard));
    }
    s2d = s2d/(num_rows-1);
    float sf = sqrt(s2f);
    double sd = sqrt(s2d);

    values->mean_d = xbard;
    values->var_d = s2d;
    values->stddev_d = sd;

    values->mean_f = xbarf;
    values->var_f = s2f;
    values->stddev_f = sf;
}

void West(vector<float> &fdata, vector<double> &ddata, int num_rows, CalcValues *values)
{
    float mf = fdata[0];
    float tf = 0;
    float xbarf, s2f, sf, qf, rf;
    double md = ddata[0];
    double td = 0;
    double xbard, s2d, sd, qd, rd;

    for (int i = 1; i < num_rows; ++i)
    {
        qf = fdata[i] - mf;
        rf = qf/i;
        mf += rf;
        tf += (i - 1) * qf * rf;

        qd = ddata[i] - md;
        rd = qd/i;
        md += rd;
        td += (i - 1) * qd * rd;
    }

    values->var_f = tf/(num_rows - 1);
    values->mean_f = mf;
    values->stddev_f = sqrt(values->var_f);

    values->var_d = td/(num_rows - 1);
    values->mean_d = md;
    values->stddev_d= sqrt(values->var_d);
}

void Textbook(vector<float> &fdata, vector<double> &ddata, int num_rows, CalcValues *values)
{
    float sumxf = 0, sumxf2 = 0;
    double sumxd = 0, sumxd2 = 0;

    for (int i = 0; i < num_rows; ++i)
    {
        sumxf += fdata[i];
        sumxf2 += (fdata[i] * fdata[i]);

        sumxd += ddata[i];
        sumxd2 += (ddata[i] * ddata[i]);
    }

    values->mean_f = sumxf/num_rows;
    values->var_f = (sumxf2 - values->mean_f * sumxf)/(num_rows - 1);
    values->stddev_f = sqrt(values->var_f);

    values->mean_d = sumxd/num_rows;
    values->var_d = (sumxd2 - values->mean_d * sumxd)/(num_rows - 1);
    values->stddev_d = sqrt(values->var_d);
}

void Cotton(vector<float> &fdata, vector<double> &ddata, int num_rows, CalcValues *values)
{
    float mf = fdata[0];
    float tf = 0;
    float qf;
    double md = ddata[0];
    double td = 0;
    double qd;

    for (int i = 1; i < num_rows; ++i)
    {
        qf = mf;
        mf = qf + (fdata[i] - qf)/i;
        tf += (fdata[i] * fdata[i]) + (i - 1) * (qf * qf) - i * (mf * mf);

        qd = md;
        md = qd + (ddata[i] - qd)/i;
        td += (ddata[i] * ddata[i]) + (i - 1) * (qd * qd) - i * (md * md);
    }

    values->mean_f = mf;
    values->var_f = tf/(num_rows - 1);
    values->stddev_f = sqrt(values->var_f);

    values->mean_d = md;
    values->var_d = td/(num_rows - 1);
    values->stddev_d = sqrt(values->var_d);
}
