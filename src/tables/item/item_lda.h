//
// Created by Bryan Zhou on 2022/10/25.
//

#ifndef EXTRACTOR_ITEM_LDA_H
#define EXTRACTOR_ITEM_LDA_H

#include "table_extractor.h"

class ItemLDA : public TableExtractor{
public:
    ItemLDA(int embedding_size);
    ~ItemLDA(){}
    /**
     * item LDA特征
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features);

private:
    static vector<float> default_list;

};
#endif //EXTRACTOR_ITEM_LDA_H
