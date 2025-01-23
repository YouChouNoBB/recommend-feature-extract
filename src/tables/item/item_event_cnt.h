//
// Created by Bryan Zhou on 2022/10/25.
//

#ifndef EXTRACTOR_ITEM_EVENT_CNT_H
#define EXTRACTOR_ITEM_EVENT_CNT_H
#include "item.h"
#include "table_extractor.h"
#include "five.h"
class ItemEventCnt:public TableExtractor{
public:
    ItemEventCnt(string key):key(key){}
    ~ItemEventCnt(){}
     /**
      * item 统计特征,size=12
      * event in (exp,clk,sea,add,buy)
      * @param sample
      * @param features
      */
    void extract(Sample *sample, Map<string, Feature> *features){
        if(features->contains(key)){
            auto list = (*features)[key].float_list();
            float exp = list.value(0);
            float clk = list.value(1);
            float sea = list.value(2);
            float add = list.value(3);
            float buy = list.value(4);

            float ctr = clk / (clk+exp); //简单平滑
            float str = sea / (sea+exp);
            float atr = add / (add+exp);
            float btr = buy / (buy+exp);

            float car = add / (add+clk);
            float cbr = buy / (buy+clk);
            float abr = buy / (buy+add);

            vector<float> res = {log(exp),log(clk),log(sea),log(add),log(buy),
                                 ctr,str,atr,btr,
                                 car,cbr,abr
                                 };
            addFloatList(sample,key,res);
        }
    }


private:
    string key;
};
#endif //EXTRACTOR_ITEM_EVENT_CNT_H
