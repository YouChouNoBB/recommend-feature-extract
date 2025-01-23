//
// Created by Bryan Zhou on 2022/10/25.
//
#include "item_lda.h"
#include "item.h"
using namespace extractor;
ItemLDA::ItemLDA(int embedding_size){
    if(default_list.empty()){
        for(int i=0;i<embedding_size;i++){
            default_list.push_back(MIN_FLOAT);
        }
    }
}

void ItemLDA::extract(Sample *sample, Map<string, Feature> *features){
    vector<float> list;
    if(features->contains(extractor::Item::item_lda_v1)){
        for(auto x:features->find(extractor::Item::item_lda_v1)->second.float_list().value()){
            list.push_back(x);
        }
    }else{
//        list = default_list;
    }
    sample->addFloatList(extractor::Item::item_lda_v1,list);
}
vector<float> ItemLDA::default_list;