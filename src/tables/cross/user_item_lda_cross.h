//
// Created by Bryan Zhou on 2022/10/26.
//

#ifndef EXTRACTOR_USER_ITEM_LDA_CROSS_H
#define EXTRACTOR_USER_ITEM_LDA_CROSS_H
#include "table_extractor.h"
#include "user.h"
#include "item.h"
#include "cross.h"
#include "similarity.h"
using namespace extractor;
class UserItemLDACross:public TableExtractor{
public:
    UserItemLDACross(){}
    ~UserItemLDACross(){}

    /**
     * 对user和item的lda特征计算相似度
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features){
        float sim=MIN_FLOAT;
        if(features->contains(User::user_lda_v1) && features->contains(extractor::Item::item_lda_v1)){
            vector<float> item_lda_v1;
            vector<float> user_lda_v1;
            pbVector2Vector(features->find(extractor::Item::item_lda_v1)->second.float_list().value(),item_lda_v1);
            pbVector2Vector(features->find(User::user_lda_v1)->second.float_list().value(),user_lda_v1);
            sim = Similarity::vecSim(item_lda_v1,user_lda_v1);
        }
        sample->add(Cross::ui_lda_sim,sim);
    }

};
#endif //EXTRACTOR_USER_ITEM_LDA_CROSS_H
