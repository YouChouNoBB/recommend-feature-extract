//
// Created by Bryan Zhou on 2022/10/25.
//

#ifndef EXTRACTOR_ITEM_LDA_H
#define EXTRACTOR_ITEM_LDA_H

#include "table_extractor.h"
#include "ctxitem.h"
class CtxItemLDA : public TableExtractor{
public:
    CtxItemLDA();
    ~CtxItemLDA(){}

    /**
     * ctx_item的LDA特征
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features){
        vector<float> list;
        if(features->contains(CtxItem::ctx_item_lda_v1)){
            for(auto x:(*features)[CtxItem::ctx_item_lda_v1].float_list().value()){
                list.push_back(x);
            }
        }
        sample->addFloatList(CtxItem::ctx_item_lda_v1,list);
    }

};
#endif //EXTRACTOR_ITEM_LDA_H
