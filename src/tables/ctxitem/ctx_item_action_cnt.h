//
// Created by Bryan Zhou on 2022/10/25.
//

#ifndef EXTRACTOR_ITEM_ACTION_CNT_H
#define EXTRACTOR_ITEM_ACTION_CNT_H
#include "ctxitem.h"
#include "table_extractor.h"
#include "five.h"
class CtxItemActionCnt:public TableExtractor{
public:
    CtxItemActionCnt(int hash_size):hash_size(hash_size){}
    ~CtxItemActionCnt(){}
    /**
     * event in (impressions_single,product_view,add_to_cart,purchase_product,product_search)
     */
     /**
      * ctx_item的统计特征
      * @param sample
      * @param features
      */
    void extract(Sample *sample, Map<string, Feature> *features);
    /**
     * 过滤周期内的行为 30天，14天，7天,3天,1天
     * @param list item action list
     * @param days days to keep
     * @param now now unix time
     * @return filtered list
     */
    vector<ItemAction> filter(vector<ItemAction> list, int days, long now);
    Map<string, Five<int, int, int, int, int>> statPv(vector<ItemAction> list, string flag);
    void extract(Sample *sample,vector<ItemAction> list, string flag,string prefix);
    Five<float, float, float, float, float> cxr(Five<int, int, int, int, int> clk,Five<int, int, int, int, int> exp);
    vector<pair<long,float>> padding(vector<pair<long,float>> stat_list,string flag);

private:
    int hash_size;
    const static int event_padding_size = 25;
    const static int ctx_padding_size = 50;
    const static int search_padding_size = 20;
};
#endif //EXTRACTOR_ITEM_ACTION_CNT_H
