#ifndef SAMPLE_H
#define SAMPLE_H

#include "tfexample.pb.h"
#include <string>
#include <vector>
#include<iostream>
using namespace std;
class Sample{
    public:
    ~Sample();
    Sample();

    /**
     * 返回features数据，protobuf协议
     * @return Map<key,TFFeature>
     */
    google::protobuf::Map<string,TFFeature> * getFeatureMap();

    /**
     * 返回index，TF-Record协议
     * @param key 特征名
     * @return
     */
    TFFeature getIndexFeature(string key);

    /**
     * 返回value，TF-Record协议
     * @param key 特征名
     * @return
     */
    TFFeature getValueFeature(string key);

    /**
     * 同时返回key和value
     * @param key 特征名
     * @return vector<pair<index,value>>
     */
    vector<pair<long,float>> getFeature(string key);

    /**
     * 添加float类型特征
     * @param name 特征名
     * @param value 特征值
     */
    void add(string name,float value);

    /**
     * 添加long类型特征，value直接作为index,权重默认为1.0
     * @param name 特征名
     * @param value index值
     */
    void add(string name,long value);

    /**
     * 添加string类型的特征，一般用的少，string直接专程bytes
     * @param name 特征名
     * @param value 特征值
     */
    void add(string name,string value);

    /**
     * 添加一个特征，直接传入index和value
     * @param name 特征名
     * @param index 特征index
     * @param value 特征value
     */
    void add(string name,long index,float value);

    /**
     * 添加标签
     * @param label 数组，多目标场景使用
     */
    void addLabel(vector<float> label);

    /**
     * 添加标签
     * @param label 单值，单目标场景使用
     */
    void addLabel(float label);

    /**
     * 添加数组特征，只传index，weight默认1.0
     * @param name 特征名
     * @param value vector<long>
     */
    void addLongList(string name,vector<long> value);

    /**
     * 添加数组特征，只传weights,index默认1-N
     * @param name 特征名
     * @param value vector<float>
     */
    void addFloatList(string name,vector<float> value);

    /**
     * 添加带权重的数组特征，传入index和weights
     * @param name 特征名
     * @param index vector<long>
     * @param value vector<float>
     */
    void addLongListWeight(string name,vector<long> index,vector<float> value);

    /**
     * 添加带权重的数组特征，传入vector<pair<index,weight>>
     * @param name 特征名
     * @param list vector<pair<index,weight>>
     */
    void addLongListWeight(string name,vector<pair<long,float>> list);

    /**
     * protobuf的Map<string,TFFeature>转成TFRecord协议的Example
     * @return Example
     */
    Example toExample();

    /**
     * 合并user/query/context/item特征
     * @param samples
     * @return
     */
    static Example merge(vector<Sample*>samples);

    private:
    google::protobuf::Map<string,TFFeature> *featureMap; //直接从example获取到的指针
    Example example;
    //一些默认值
    static string INDEX ;
    static string VALUE ;
    static string LABEL ;
    static float DEFAULT_FLOAT;
    static float DEFAULT_ZERO;
    static TFFeature DEFAULT_INDEX;
    static TFFeature DEFAULT_VALUE;
};

#endif