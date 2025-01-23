//
// Created by Bryan Zhou on 2022/11/7.
//

#ifndef EXTRACTOR_ITEM_CTXITEM_LDA_CROSS_H
#define EXTRACTOR_ITEM_CTXITEM_LDA_CROSS_H
#include "table_extractor.h"
#include "item.h"
#include "ctxitem.h"
#include "cross.h"
#include "similarity.h"
class ItemCtxItemLDACross:public TableExtractor{
public:
    ItemCtxItemLDACross(){}
    ~ItemCtxItemLDACross(){}

    /**
     * 计算item和ctx_item的LDA相似度
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features) {
        float sim=MIN_FLOAT;
        if(features->contains(CtxItem::ctx_item_lda_v1) && features->contains(extractor::Item::item_lda_v1)){
            vector<float> item_lda_v1;
            vector<float> ctx_lda_v1;
            pbVector2Vector(features->find(extractor::Item::item_lda_v1)->second.float_list().value(),item_lda_v1);
            pbVector2Vector(features->find(CtxItem::ctx_item_lda_v1)->second.float_list().value(),ctx_lda_v1);
            sim = Similarity::vecSim(item_lda_v1,ctx_lda_v1);
        }
        sample->add(Cross::qi_lda_sim,sim);
    }
};
#endif //EXTRACTOR_ITEM_CTXITEM_LDA_CROSS_H
