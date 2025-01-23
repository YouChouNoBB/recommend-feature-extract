//
// Created by Bryan Zhou on 2022/10/25.
//

#ifndef EXTRACTOR_USER_ACTION_TAG_SERIES_H
#define EXTRACTOR_USER_ACTION_TAG_SERIES_H
#include "user.h"
#include "table_extractor.h"
#include "label.h"
class UserActionTagSeries :public TableExtractor{
public:
    UserActionTagSeries(string key,int hash_size):key(key),hash_size(hash_size){}
    ~UserActionTagSeries(){}

    /**
     * 用户点击tag特征，包含与实时特征合并
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features){
        long now_time;
        if(!features->contains(Label::time)){
            now_time= now();
        }else{
            now_time=features->find(Label::time)->second.long_value();
        }
        //实时特征合并到离线特征
        mergeRTAction(features,key);
        extractAction(sample,features,key,now_time,hash_size);
    }

private:
    string key;
    int hash_size;
};
#endif //EXTRACTOR_USER_ACTION_TAG_SERIES_H
