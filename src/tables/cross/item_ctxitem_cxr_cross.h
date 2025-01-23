//
// Created by Bryan Zhou on 2022/11/7.
//

#ifndef EXTRACTOR_ITEM_CTXITEM_CXR_CROSS_H
#define EXTRACTOR_ITEM_CTXITEM_CXR_CROSS_H
#include "table_extractor.h"
#include "item.h"
#include "ctxitem.h"
#include "similarity.h"
#include "cross.h"
class ItemCtxItemCxrCross:public TableExtractor{
public:
    ItemCtxItemCxrCross(){}
    ~ItemCtxItemCxrCross(){}

    /**
     * 计算item和ctx_item的cxr相似度
     * @param sample
     * @param user
     * @param item
     */
    void extract(Sample *sample, Sample *user, Sample *item) {
        vector<float> list;
        for(auto x:days){
            vector<pair<long,float>> item_vec = item->getFeature(x);
            vector<pair<long,float>> ctx_item_vec = user->getFeature("ctx_"+x);
            float sim = Similarity::vecSim(ctx_item_vec,item_vec);
            list.push_back(sim);
        }

        sample->addFloatList(Cross::ii_cxr_sim,list);
    }

private:
    vector<string> days = {"cxr_1day","cxr_3day","cxr_7day","cxr_14day","cxr_30day"};
};
#endif //EXTRACTOR_ITEM_CTXITEM_CXR_CROSS_H
