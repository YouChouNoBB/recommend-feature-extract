//
// Created by Bryan Zhou on 2022/11/7.
//

#ifndef EXTRACTOR_QUERY_ITEM_LDA_CROSS_H
#define EXTRACTOR_QUERY_ITEM_LDA_CROSS_H
#include "table_extractor.h"
#include "cross.h"
#include "item.h"
#include "query.h"
#include "similarity.h"
class QueryItemLDACross:public TableExtractor{
public:
    QueryItemLDACross(){}
    ~QueryItemLDACross(){}

    /**
     * 计算query和item的LDA相似度
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features) {
        float sim=MIN_FLOAT;
        if(features->contains(Query::query_lda_v1) && features->contains(extractor::Item::item_lda_v1)){
            vector<float> item_lda_v1;
            vector<float> query_lda_v1;
            pbVector2Vector(features->find(extractor::Item::item_lda_v1)->second.float_list().value(),item_lda_v1);
            pbVector2Vector(features->find(Query::query_lda_v1)->second.float_list().value(),query_lda_v1);
            sim = Similarity::vecSim(item_lda_v1,query_lda_v1);
        }
        sample->add(Cross::qi_lda_sim,sim);
    }
};
#endif //EXTRACTOR_QUERY_ITEM_LDA_CROSS_H
