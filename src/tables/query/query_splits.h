//
// Created by Bryan Zhou on 2022/11/7.
//

#ifndef EXTRACTOR_QUERY_SPLITS_H
#define EXTRACTOR_QUERY_SPLITS_H
#include "table_extractor.h"
#include "query.h"
#include "label.h"
class QuerySplits:public TableExtractor{
public:
    QuerySplits(int hash_size):hash_size(hash_size){}
    ~QuerySplits(){}

    /**
     * query分词特征
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features) {
        vector<long> list ;
        if(features->contains(Label::query)){
            vector<string> splits;
            string query=(*features)[Label::query].str_value();
            tolower(query);
            trim(query);
            if(query.empty()){
                sample->addLongList(Query::query_seg,list);
                return;
            }
            split(query,' ',',',splits);
            for(auto x:splits){
                list.push_back(hash(x)%hash_size+1);
            }
        }

        sample->addLongList(Query::query_seg,list);
    }

private:
    int hash_size;
};

#endif //EXTRACTOR_QUERY_SPLITS_H
