//
// Created by Bryan Zhou on 2022/10/24.
//
#include "labelcontext.h"
void LabelContext::extractTime(Sample *sample, Map<string, Feature> *features){
    long now_time;
    if(!features->contains(Label::time)){
        now_time=now();
    }else{
        now_time=features->find(Label::time)->second.long_value();
    }
    vector<long> list = getTimeFeature(now_time);
    sample->add(Label::day_of_week,list[0]);
    sample->add(Label::day_of_month,list[1]);
    sample->add(Label::hour_of_day,list[2]);
    sample->add(Label::hour_slice,list[3]);
    sample->add(Label::is_weekend,list[4]);
}


void LabelContext::addAdditionInfo(Sample *sample, Map<string, Feature> *features){
    addString(sample,Label::user_id,features,Label::user_id);
    addString(sample,Label::store_id,features,Label::store_id);
    addString(sample,Label::item_id,features,Label::item_id);
    addString(sample,Label::ctx_item_id,features,Label::ctx_item_id);
    addString(sample,Label::query,features,Label::query);
    addString(sample,Label::scm,features,Label::scm);
    addString(sample,Label::collection_id,features,Label::collection_id);
}


void LabelContext::extract(Sample *sample, Map<string, Feature> *features){
    addLong(sample,Label::store_id,features,Label::store_id,hash_size);
    addLong(sample,Label::item_id,features,Label::item_id,hash_size);
    addLong(sample,Label::query,features,Label::query,hash_size);
    addLong(sample,Label::collection_id,features,Label::collection_id,hash_size);
    addLong(sample,Label::ctx_item_id,features,Label::ctx_item_id,hash_size);
    addLong(sample,Label::city,features,Label::city,hash_size);
    addLong(sample,Label::province,features,Label::province,hash_size);
    addLong(sample,Label::country,features,Label::country,hash_size);
    addLong(sample,Label::os,features,Label::os,hash_size);
    addLabel(sample,features,Label::label);
    extractTime(sample,features);
    addAdditionInfo(sample,features);
}

LabelContext::LabelContext() {}
