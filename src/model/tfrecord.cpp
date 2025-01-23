#include "tfexample.pb.h"
#include <string>
#include <vector>
#include <tfrecord.h>

using namespace std;

TFFeature TfRecord::float2Feature(float x) {
    TFFeature feature ;
    feature.mutable_float_list()->add_value(x);
    return feature;
}

TFFeature TfRecord::long2Feature(long x) {
    TFFeature feature ;
    feature.mutable_int64_list()->add_value(x);
    return feature;
}

TFFeature TfRecord::string2Feature(string x) {
    TFFeature feature ;
    feature.mutable_bytes_list()->add_value(x);
    return feature;
}

TFFeature TfRecord::longlist2Feature(vector<long> x) {
    TFFeature feature ;
    for (auto i: x) {
        feature.mutable_int64_list()->add_value(i);
    }
    return feature;
}

TFFeature TfRecord::floatlist2Feature(vector<float> x) {
    TFFeature feature ;
    for (auto i: x) {
        feature.mutable_float_list()->add_value(i);
    }
    return feature;
}

TFFeature TfRecord::stringlist2Feature(vector<string> x) {
    TFFeature feature ;
    for (auto i: x) {
        feature.mutable_bytes_list()->add_value(i);
    }
    return feature;
}

TFFeature TfRecord::defaultIndexFeature(int size) {
    TFFeature feature ;
    for (int i = 1; i <= size; i++) {
        feature.mutable_int64_list()->add_value(i);
    }
    return feature;
}

TFFeature TfRecord::defaultWeightFeature(int size) {
    TFFeature feature ;
    for (int i = 0; i < size; i++) {
        feature.mutable_float_list()->add_value(1.0f);
    }
    return feature;
}

