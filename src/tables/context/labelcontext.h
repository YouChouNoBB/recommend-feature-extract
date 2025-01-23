//
// Created by Bryan Zhou on 2022/10/20.
//

#ifndef EXTRACTOR_LABELCONTEXT_H
#define EXTRACTOR_LABELCONTEXT_H
#include "table_extractor.h"
#include "label.h"
class LabelContext:public TableExtractor{
public:
    LabelContext(int hash_size):hash_size(hash_size){}

    LabelContext();

    ~LabelContext(){}
    /**
     * 时间特征抽取
     * @param sample
     * @param features
     */
    void extractTime(Sample *sample, Map<string, Feature> *features);

    /**
     * 添加context的一些信息，方便排查问题
     * @param sample
     * @param features
     */
    void addAdditionInfo(Sample *sample, Map<string, Feature> *features);

    /**
     * 特征抽取接口
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features);


private:
    int hash_size;
};
#endif //EXTRACTOR_LABELCONTEXT_H
