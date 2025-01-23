//
// Created by Bryan Zhou on 2022/10/20.
//

#ifndef EXTRACTOR_LABELCONTEXT_ITEM_H
#define EXTRACTOR_LABELCONTEXT_ITEM_H
#include "table_extractor.h"
#include "label.h"
#include "labelcontext.h"
class LabelContextItemHash:public LabelContext{
public:
    LabelContextItemHash(int hash_size,int item_hash_size):hash_size(hash_size),item_hash_size(item_hash_size){}
    LabelContextItemHash(){}
    ~LabelContextItemHash(){}

    void extract(Sample *sample, Map<string, Feature> *features){
        addLong(sample,Label::store_id,features,Label::store_id,hash_size);
        addLong(sample,Label::item_id,features,Label::item_id,item_hash_size);
        addLong(sample,Label::query,features,Label::query,hash_size);
        addLong(sample,Label::collection_id,features,Label::collection_id,hash_size);
        addLong(sample,Label::ctx_item_id,features,Label::ctx_item_id,item_hash_size);
        addLong(sample,Label::city,features,Label::city,hash_size);
        addLong(sample,Label::province,features,Label::province,hash_size);
        addLong(sample,Label::country,features,Label::country,hash_size);
        addLong(sample,Label::os,features,Label::os,hash_size);
        addLabel(sample,features,Label::label);
        extractTime(sample,features);
        addAdditionInfo(sample,features);
    }


private:
    int hash_size;
    int item_hash_size;
};
#endif //EXTRACTOR_LABELCONTEXT_ITEM_H
