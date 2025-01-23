//
// Created by Bryan Zhou on 2023/1/30.
//

#ifndef EXTRACTOR_RANK_GAUSS_H
#define EXTRACTOR_RANK_GAUSS_H

#include <vector>
#include <map>
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <cmath>
using namespace std;

double RationalApproximation(double t);
double NormalCDFInverse(double p);
double vdErfInvSingle01(double x);
void buildRankGaussTrafo(vector<float> dataIn, map<float,float> &trafoMap);
void applyRankTrafo(vector<float> dataIn, map<float,float> &trafoMap, vector<float> &dataOut);

#endif //EXTRACTOR_RANK_GAUSS_H
