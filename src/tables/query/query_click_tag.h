//
// Created by Bryan Zhou on 2022/11/7.
//

#ifndef EXTRACTOR_QUERY_CLICK_TAG_H
#define EXTRACTOR_QUERY_CLICK_TAG_H
#include "table_extractor.h"
#include "query.h"
class QueryClickTag :public TableExtractor{
public:
    QueryClickTag(int hash_size):hash_size(hash_size){}
    ~QueryClickTag(){}

    /**
     * query点击过的tag特征
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features) {
        vector<pair<long,float>> list ;
        if(features->contains(Query::query_click_tag)){
            for(auto x:(*features)[Query::query_click_tag].str_int_list().value()){
                long key = hash(x.key()) %hash_size +1;
                float value = log(x.value());
                list.emplace_back(key,value);
            }
        }
        sample->addLongListWeight(Query::query_click_tag,list);
    }

private:
    int hash_size;
};

#endif //EXTRACTOR_QUERY_CLICK_TAG_H
