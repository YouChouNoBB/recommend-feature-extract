//
// Created by Bryan Zhou on 2022/10/25.
//

#ifndef EXTRACTOR_USER_LDA_H
#define EXTRACTOR_USER_LDA_H

#include "table_extractor.h"
#include "user.h"
class UserLDA :public TableExtractor{
public:
    UserLDA(){}
    ~UserLDA(){}

    /**
     * user LDA特征
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features){
        vector<float> list;
        if(features->contains(User::user_lda_v1)){
            for(auto x:features->find(User::user_lda_v1)->second.float_list().value()){
                list.push_back(x);
            }
        }
        sample->addFloatList(User::user_lda_v1,list);
    }
};
#endif //EXTRACTOR_USER_LDA_H
