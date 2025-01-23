//
// Created by Bryan Zhou on 2022/11/7.
//

#ifndef EXTRACTOR_QUERY_LDA_H
#define EXTRACTOR_QUERY_LDA_H
#include "table_extractor.h"
#include "query.h"
class QueryLDA :public TableExtractor{
public:
    QueryLDA(){}
    ~QueryLDA(){}

    /**
     * query LDA特征
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features) {
        vector<float> list;
        if(features->contains(Query::query_lda_v1)){
            for(auto x:(*features)[Query::query_lda_v1].float_list().value()){
                list.push_back(x);
            }
        }
        sample->addFloatList(Query::query_lda_v1,list);
    }
};
#endif //EXTRACTOR_QUERY_LDA_H
