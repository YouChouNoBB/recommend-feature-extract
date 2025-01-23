#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include "table_extractor.h"
#include "tfexample.pb.h"
using namespace std;

class Extractor  {
public:
    Extractor(){
        genDateMap();
    }
    virtual ~Extractor(){}
    vector<TableExtractor*> userFeatureTables;
    vector<TableExtractor*> itemFeatureTables;
    vector<TableExtractor*> crossFeatureTables;
    void genDateMap();
    /**
     * user/context/query 特征抽取
     * @param map features
     * @return Sample
     */

    Sample* extractUser(Map<string, Feature> *map);
    /**
     * item侧 特征抽取
     * @param map features
     * @return Sample
     */
    Sample* extractItem(Map<string, Feature> *map);

    /**
     * 交叉特征抽取
     * @param user user/context/query sample特征
     * @param item item sample特征
     * @return Sample
     */
    Sample* extractCross(Sample *user, Sample *item);

    /**
     * 内部抽取接口，离线和在线接口都会调用
     * @param features
     * @return tf格式的Example
     */
    Example extractor(Features *features);

    /**
     * go在线调用接口
     * @param features BatchFeatures格式的指针
     * @param size 数据长度
     * @return 指针
     */
    byte* extractBatch(void *features,int size);

    /**
     * 离线调用接口
     * @param features Features格式的指针
     * @param size 数据长度
     * @return 指针
     */
    byte* extract(void *features,int size);
    virtual void print();
private:
    static int num_threads;
};
#endif