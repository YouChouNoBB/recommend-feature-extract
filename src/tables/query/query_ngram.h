//
// Created by Bryan Zhou on 2022/11/7.
//

#ifndef EXTRACTOR_QUERY_NGRAM_H
#define EXTRACTOR_QUERY_NGRAM_H
#include "table_extractor.h"
#include "query.h"
#include "label.h"
class QueryNGram:public TableExtractor{
public:
    QueryNGram(int n,int hash_size,string out_key):n(n),hash_size(hash_size),out_key(out_key){}
    QueryNGram(int n,int hash_size):n(n),hash_size(hash_size){out_key=Query::query_ngram;}
    ~QueryNGram(){}

    /**
     * query N-gram特征
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features) {
        vector<pair<long,float>> list ;
        if(features->contains(Label::query)){
            string query = (*features)[Label::query].str_value();
            tolower(query);
            trim(query);
            getNGram(query,n,hash_size,list);
        }
        sample->addLongListWeight(out_key,list);
    }

private:
    int n;
    string out_key;
    int hash_size;
};

#endif //EXTRACTOR_QUERY_NGRAM_H
