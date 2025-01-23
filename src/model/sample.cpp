
#include<iostream>
#include<string>
#include<vector>
#include "tfrecord.h"
#include "sample.h"

using namespace std;

Sample::Sample() {
    featureMap = example.mutable_features()->mutable_feature();
    if (featureMap == nullptr) {
        cout << "featureMap is null" << endl;
    }
}

Sample::~Sample() {
    featureMap->clear();
}

google::protobuf::Map<string, TFFeature> * Sample::getFeatureMap() {
    return featureMap;
}

TFFeature Sample::getIndexFeature(string key) {
    key = key + INDEX;
    auto it = featureMap->find(key);
    if (it == featureMap->end()) {
        return DEFAULT_INDEX;
    }

    return it->second;
}

TFFeature Sample::getValueFeature(string key) {
    key = key + VALUE;
    auto it = featureMap->find(key);
    if (it == featureMap->end()) {
        return  DEFAULT_VALUE;
    }
    return it->second;
}

vector<pair<long, float>> Sample::getFeature(string key) {
    TFFeature index = getIndexFeature(key);
    TFFeature value = getValueFeature(key);
    int size = index.int64_list().value_size();
    vector<pair<long, float>> list;
    if (size == 0)return list;
    for (int i = 0; i < size; i++) {
        list.emplace_back(make_pair<long, float>(
                index.int64_list().value(i),
                value.float_list().value(i)
        ));
    }
    return list;
}

void Sample::add(string name, float value) {
    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(name + INDEX, TfRecord::long2Feature(1L)));
    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(name + VALUE, TfRecord::float2Feature(value)));
}

void Sample::add(string name, long value) {
    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(name + INDEX, TfRecord::long2Feature(value)));
    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(name + VALUE, DEFAULT_VALUE));
}

void Sample::add(string name, string value) {
    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(name, TfRecord::string2Feature(value)));
}

void Sample::add(string name, long index, float value) {
    if (value == 0.0)value = DEFAULT_ZERO;
    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(name + INDEX, TfRecord::long2Feature(index)));
    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(name + VALUE, TfRecord::float2Feature(value)));
}

void Sample::addLabel(vector<float> label) {
    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(LABEL, TfRecord::floatlist2Feature(label)));
}

void Sample::addLabel(float label) {
    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(LABEL, TfRecord::float2Feature(label)));
}

void Sample::addLongList(string name, vector<long> value) {
    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(name + INDEX, TfRecord::longlist2Feature(value)));
    featureMap->insert(
            google::protobuf::MapPair<string, TFFeature>(name + VALUE, TfRecord::defaultWeightFeature(value.size())));
}

void Sample::addFloatList(string name, vector<float> value) {
    vector<long> idx;
    vector<float> val;
    for(int i=0;i<value.size();i++){
        if(value[i]!=0){
            idx.push_back(i+1L);
            val.push_back(value[i]);
        }else{
            idx.push_back(i+1L);
            val.push_back(1e-6f);
        }
    }

    featureMap->insert(
            google::protobuf::MapPair<string, TFFeature>(name + INDEX, TfRecord::longlist2Feature(idx)));
    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(name + VALUE, TfRecord::floatlist2Feature(val)));
}

void Sample::addLongListWeight(string name, vector<long> index, vector<float> value) {
    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(name + INDEX, TfRecord::longlist2Feature(index)));
    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(name + VALUE, TfRecord::floatlist2Feature(value)));
}

void Sample::addLongListWeight(string name, vector<pair<long, float>> list) {
    vector<long> index;
    vector<float> value;
    for (auto x: list) {
        index.emplace_back(x.first);
        value.emplace_back(x.second);
    }

    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(name + INDEX, TfRecord::longlist2Feature(index)));
    featureMap->insert(google::protobuf::MapPair<string, TFFeature>(name + VALUE, TfRecord::floatlist2Feature(value)));
}

Example Sample::toExample() {
    return example;
}

Example Sample::merge(vector<Sample*> samples) {
    Example example;
    google::protobuf::Map<string, TFFeature> *map_feature = example.mutable_features()->mutable_feature();
    for (auto sample: samples) {
        google::protobuf::Map<string, TFFeature> *tmp = sample->getFeatureMap();
        map_feature->insert(tmp->begin(), tmp->end());
    }
    return example;
}


string Sample::INDEX = "_index";
string Sample::VALUE = "_value";
string Sample::LABEL = "label";
float Sample::DEFAULT_FLOAT = 1.0f;
float Sample::DEFAULT_ZERO = 1e-6f;
TFFeature Sample::DEFAULT_INDEX = TfRecord::long2Feature(0l);
TFFeature Sample::DEFAULT_VALUE = TfRecord::float2Feature(Sample::DEFAULT_FLOAT);

