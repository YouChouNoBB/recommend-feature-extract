//
// Created by Bryan Zhou on 2022/11/28.
//

#ifndef EXTRACTOR_DSSM_SHORT_CLICK_H
#define EXTRACTOR_DSSM_SHORT_CLICK_H
#include "labelcontext.h"
class DssmShortClickContext :public LabelContext{
public:
    /**
     * dssm短序列context
     * 通用hash和对item的hash size需要区分开
     * @param hash_size
     * @param label_hash_size
     */
    DssmShortClickContext(int hash_size,int label_hash_size):hash_size(hash_size),label_hash_size(label_hash_size){}
    ~DssmShortClickContext(){}
    void extract(Sample *sample, Map<string, Feature> *features){
        addLong(sample,Label::store_id,features,Label::store_id,hash_size);
        addLong(sample,Label::item_id,features,Label::item_id,label_hash_size);
        addLong(sample,Label::city,features,Label::city,hash_size);
        addLong(sample,Label::province,features,Label::province,hash_size);
        addLong(sample,Label::country,features,Label::country,hash_size);
        addLabel(sample,features,Label::label);
        addAdditionInfo(sample,features);
    }

private:
    int hash_size;
    int label_hash_size;
};

#endif //EXTRACTOR_DSSM_SHORT_CLICK_H
