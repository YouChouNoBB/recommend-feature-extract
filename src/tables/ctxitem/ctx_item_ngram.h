//
// Created by Bryan Zhou on 2022/10/26.
//

#ifndef EXTRACTOR_ITEM_NGRAM_H
#define EXTRACTOR_ITEM_NGRAM_H
#include "sample.h"
#include "features.pb.h"
#include "ctxitem.h"
#include "table_extractor.h"

class CtxItemNGram :public TableExtractor{
public:
    CtxItemNGram(int n,int hash_size,int padding_size):n(n),hash_size(hash_size),padding_size(padding_size){}
    CtxItemNGram(int n,int hash_size):n(n),hash_size(hash_size){}

    /**
     * ctx_item的N-gram特征
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features){
        vector<pair<long,float>> list;
        if(features->contains(CtxItem::ctx_title_tags)){
            for(auto query:features->find(CtxItem::ctx_title_tags)->second.str_list().value()){
                getNGram(query,n,hash_size,list);
            }
        }
        Map <long,float> map;
        map.insert(list.begin(),list.end());
        list.clear();
        for(auto it=map.begin();it!=map.end();++it){
            list.emplace_back(it->first,it->second);
        }
        // padding
//        padding_weight(list,padding_size);

        sample->addLongListWeight(CtxItem::ctx_item_ngram,list);
    }

private:
    int n;
    int hash_size;
    int padding_size;
};
#endif //EXTRACTOR_ITEM_NGRAM_H
