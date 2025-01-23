//
// Created by Bryan Zhou on 2022/10/25.
//

#ifndef EXTRACTOR_ITEM_CTX_ITEM_TAG_SIM_H
#define EXTRACTOR_ITEM_CTX_ITEM_TAG_SIM_H
#include "table_extractor.h"
#include "ctxitem.h"
#include "cross.h"
#include "item.h"
#include <algorithm>
#include <string>
#include "similarity.h"

using namespace extractor;
class ItemCtxItemTagSim:public TableExtractor{
public:
    ItemCtxItemTagSim(){}
    ~ItemCtxItemTagSim(){}

    /**
     * item和ctx_item的title_tags相似
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features) {
        if(!features->contains(CtxItem::ctx_title_tags) || !features->contains(extractor::Item::title_tags)){
            sample->addFloatList(Cross::ii_title_sim,vector<float>{MIN_FLOAT,MIN_FLOAT});
            return;
        }
        vector<string> ctx_tags ;
        for(auto x:features->find(CtxItem::ctx_title_tags)->second.str_list().value()){
            transform(x.begin(),x.end(),x.begin(),::tolower); //转小写
            x = trim(x);
            ctx_tags.emplace_back(x);
        }

        vector<string> tags;
        for(auto x:features->find(extractor::Item::title_tags)->second.str_list().value()){
            transform(x.begin(),x.end(),x.begin(),::tolower); //转小写
            x = trim(x);
            tags.emplace_back(x);
        }

        vector<float> res ;
        res.push_back(Similarity::matchSim(ctx_tags,tags));
        res.push_back(Similarity::matchEditSim(ctx_tags,tags));
        sample->addFloatList(Cross::ii_title_sim,res);
    }
};
#endif //EXTRACTOR_ITEM_CTX_ITEM_TAG_SIM_H
