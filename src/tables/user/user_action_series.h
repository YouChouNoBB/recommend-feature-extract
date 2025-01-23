//
// Created by Bryan Zhou on 2022/11/28.
//

#ifndef EXTRACTOR_USER_ACTION_SERIES_H
#define EXTRACTOR_USER_ACTION_SERIES_H
#include "table_extractor.h"
#include "label.h"
#include "user.h"

class UserActionSeries:public TableExtractor{
public:
    UserActionSeries(string key,int hash_size,int item_hash_size):key(key),hash_size(hash_size),item_hash_size(item_hash_size){}
    ~UserActionSeries(){}
    /**
     * 用户行为序列特征，item_id粒度
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features);
    void mergeActionSeries(Map<string, Feature> *features, string key);
    void extractActionSeries(Sample *sample, Map<string, Feature> *features, string key, long now);
    void mergeSeries(Feature *offline,Feature *online);
    void mergeItem(void *a,void *b);

private:
    string key; //click_item_series,cart_item_series,purchase_item_series,ignore_item_series
    int hash_size;
    int item_hash_size;
};
#endif //EXTRACTOR_USER_ACTION_SERIES_H
