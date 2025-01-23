//
// Created by Bryan Zhou on 2023/1/30.
//

#include "rank_gauss.h"

double RationalApproximation(double t) //code from: http://www.johndcook.com/blog/cpp_phi_inverse/
{
    double c[] = {2.515517, 0.802853, 0.010328};
    double d[] = {1.432788, 0.189269, 0.001308};
    return t - ((c[2]*t + c[1])*t + c[0]) / (((d[2]*t + d[1])*t + d[0])*t + 1.0);
}

double NormalCDFInverse(double p)
{
    if (p <= 0.0 || p >= 1.0)
        assert(false);
    if (p < 0.5)
        return -RationalApproximation( sqrt(-2.0*log(p)) );
    return RationalApproximation( sqrt(-2.0*log(1-p)) );
}

double vdErfInvSingle01(double x)
{
    return (x==0.0?0.0:(x<0.0?-NormalCDFInverse(-x):NormalCDFInverse(x)))*0.7;
}

void buildRankGaussTrafo(vector<float> dataIn, map<float,float> &trafoMap)
{
    map<float,int> hist;
    for (int i=0; i < dataIn.size(); i++)
        hist[dataIn[i]]++;
    if (hist.size() == 0)
        return;
    else if (hist.size() == 1)  // unary column: trafo all to 0
        trafoMap[hist.begin()->first] = 0.0;
    else if (hist.size() == 2)  // binary column: trafo to 0 / 1
    {
        trafoMap[hist.begin()->first] = 0.0;
        trafoMap[hist.rbegin()->first] = 1.0;
    }
    else  // more than 2 unique values
    {
        int N = 0, cnt = 0;
        for (map<float,int>::iterator it = hist.begin(); it != hist.end(); it++)
            N += it->second;
        assert(N == dataIn.size());
        double mean = 0.0;
        for (map<float,int>::iterator it = hist.begin(); it != hist.end(); it++)
        {
            double rankV = (double)cnt/(double)N;
            rankV = rankV*0.998 + 1e-3;
            rankV = vdErfInvSingle01(rankV);
            assert(rankV >= -3.0 && rankV <= 3.0);
            mean += ((double)it->second)*rankV;
            trafoMap[it->first] = rankV;
            cnt += it->second;  // increase distance to next bin
        }
        mean /= (double)N;
        for (map<float,float>::iterator it = trafoMap.begin(); it != trafoMap.end(); it++)
            it->second -= mean;
    }
}

void applyRankTrafo(vector<float> dataIn, map<float,float> &trafoMap, vector<float> &dataOut)
{
    dataOut.resize(dataIn.size());
    for (int i=0; i < dataIn.size(); i++)
    {
        float val = dataIn[i], trafoVal = 0.0;
        pair<map<float,float>::iterator,map<float,float>::iterator> p = trafoMap.equal_range(val);  // find nearest values
        if (p.first == trafoMap.begin())
            trafoVal = trafoMap.begin()->second;
        else if (p.first == trafoMap.end())
            trafoVal = trafoMap.rbegin()->second;
        else
        {
            if (p.first != trafoMap.begin())
                p.first--;
            if (p.second != trafoMap.end())
                p.second++;
            trafoVal = p.first->second;
            float minDist = 1e20;
            int cc = 0;
            for (map<float,float>::iterator it = p.first; it != p.second; it++)
            {
                cc++;
                if (minDist > fabs(val - it->first))
                {
                    minDist = fabs(val - it->first);
                    trafoVal = it->second;
                }
            }
            if (minDist != 0.0)
            {
                if (cc != 2)
                    assert(false);
                else  // linear interpolation
                {
                    map<float,float>::iterator it = p.first;
                    double x0 = it->first, y0 = it->second;
                    it++;
                    double x1 = it->first, y1 = it->second;
                    double x = val;
                    double y = y0 + (x - x0) * (y1 - y0) / (x1 - x0);
                    assert(y >= y0);
                    assert(y <= y1);
                    trafoVal = y;
                }
            }
        }
        dataOut[i] = trafoVal;  // assigned transformed
    }
}

