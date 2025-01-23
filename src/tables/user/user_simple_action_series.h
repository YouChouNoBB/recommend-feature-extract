//
// Created by Bryan Zhou on 2022/11/28.
//

#ifndef EXTRACTOR_USER_SIMPLE_ACTION_SERIES_H
#define EXTRACTOR_USER_SIMPLE_ACTION_SERIES_H
#include "table_extractor.h"
#include "label.h"
#include "user.h"

class UserSimpleActionSeries:public TableExtractor{
public:
    UserSimpleActionSeries(string key,int hash_size,int item_hash_size):key(key),hash_size(hash_size),item_hash_size(item_hash_size){}
    ~UserSimpleActionSeries(){}
    /**
     * 用户行为序列特征，item_id粒度,只有item序列，没有其他信息,用于dssm
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features){
        vector<long> list;
        if(features->contains(key)){
            for(auto x:features->find(key)->second.str_list().value()){
                list.push_back(hash(x));
            }
        }
        sample->addLongList(key,list);
    }


private:
    string key; //click_item_series,cart_item_series,purchase_item_series,ignore_item_series
    int hash_size;
    int item_hash_size;
};
#endif //EXTRACTOR_USER_SIMPLE_ACTION_SERIES_H
