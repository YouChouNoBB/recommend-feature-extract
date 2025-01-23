//
// Created by Bryan Zhou on 2022/10/26.
//

#ifndef EXTRACTOR_ITEM_NGRAM_H
#define EXTRACTOR_ITEM_NGRAM_H
#include "sample.h"
#include "features.pb.h"
#include "item.h"
#include "table_extractor.h"
using namespace extractor;

class ItemNGram :public TableExtractor{
public:
    ItemNGram(int n,int hash_size,int padding_size):n(n),hash_size(hash_size),padding_size(padding_size){}
    ItemNGram(int n,int hash_size):n(n),hash_size(hash_size){}
    ItemNGram(string key,string out_key,int n,int hash_size):key(key),out_key(out_key),n(n),hash_size(hash_size){}

    /**
     * item N-gram
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features){
        vector<pair<long,float>> list;
        if(features->contains(key)){
            for(auto query:features->find(key)->second.str_list().value()){
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

        sample->addLongListWeight(out_key,list);
    }

private:
    string key;
    string out_key;
    int n;
    int hash_size;
    int padding_size;
};
#endif //EXTRACTOR_ITEM_NGRAM_H
