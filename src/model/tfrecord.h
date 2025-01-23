#ifndef TFRECORD_H
#define TFRECORD_H
#include "tfexample.pb.h"
#include <string>
#include <vector>
using namespace std;

/**
 * 基础类型数据转换成TfRecord类型数据
 */
class TfRecord{
    public:
    static TFFeature float2Feature(float x);

    static TFFeature long2Feature(long x);

    static TFFeature string2Feature(string x);

    static TFFeature longlist2Feature(vector<long> x);

    static TFFeature floatlist2Feature(vector<float> x);

    static TFFeature stringlist2Feature(vector<string> x);

    static TFFeature defaultIndexFeature(int size);

    static TFFeature defaultWeightFeature(int size);

};

#endif