//
// Created by Bryan Zhou on 2022/10/25.
//

#include "item_action_cnt.h"
#include "label.h"
#include "item.h"

using namespace extractor;

void ItemActionCnt::extract(Sample *sample, Map<string, Feature> *features) {

    long now_time;
    if (!features->contains(Label::time)) {
        now_time = now();
    } else {
        now_time = features->find(Label::time)->second.long_value();
    }
    vector<ItemAction> list;
    if (features->contains(extractor::Item::item_stat)) {
        auto item_action_list = (*features)[extractor::Item::item_stat].item_action_list().value();
        for (auto x: item_action_list)
            list.emplace_back(x);
    }
    vector<ItemAction> list_2week;
    filter(list, list_2week, 14, now_time);

    vector<ItemAction> list_1week;
    filter(list, list_1week, 7, now_time);

    vector<ItemAction> list_3day;
    filter(list, list_3day, 3, now_time);

    vector<ItemAction> list_1day;
    filter(list, list_1day, 1, now_time);

    //event,ctx,search
    vector<ItemAction> finalList = list;
    vector<pair<vector<ItemAction>, string>> all{
            {finalList,  "30day"},
            {list_2week, "14day"},
            {list_1week, "7day"},
            {list_3day,  "3day"},
            {list_1day,  "1day"}
    };

    for (auto x: all) {
        extract(sample, x.first, "event", x.second);
        extract(sample, x.first, "ctx_event", x.second);
        extract(sample, x.first, "search", x.second);
    }
}

void ItemActionCnt::filter(vector<ItemAction> &list, vector<ItemAction> &dst, int days, long now_time) {
    if (list.empty())return;
    for (auto x: list) {
        if (diffDay(now_time, date2Unix(x.date())) <= days)
            dst.emplace_back(x);
    }
}

Map<string, Five<int, int, int, int, int>> ItemActionCnt::statPv(vector<ItemAction> list, string flag) {
    Map<string, Five<int, int, int, int, int>> map;
    for (ItemAction itemAction: list) {
        string tag;
        if (flag == "event") {     //处理行为事件
            tag = itemAction.event();
        } else if (flag == "ctx_event") { //处理带场景的行为事件
            if (itemAction.event() == "product_search") {
                continue;             //搜索的ctx是query，单独处理
            }
            tag = itemAction.event() + "#" + itemAction.context();
        } else {                        //处理搜索词
            if (itemAction.event() != "product_search") {
                continue;             //非搜索事件不处理
            }
            tag = itemAction.context();
        }

        int cnt = itemAction.cnt();
        int dis_cnt = itemAction.dist_cnt();
        int clk_cnt = itemAction.click_cnt();
        int buy_cnt = itemAction.buy_cnt();
        int occur_days = 1; //出现天数
        if (map.contains(tag)) {
            Five<int, int, int, int, int> old = map[tag];
            Five<int, int, int, int, int> five(
                    old.first + cnt,
                    old.second + dis_cnt,
                    old.third + clk_cnt,
                    old.fourth + buy_cnt,
                    old.fifth + occur_days
            );
            map[tag] = five;
        } else {
            Five<int, int, int, int, int> five(cnt, dis_cnt, clk_cnt, buy_cnt, occur_days);
            map[tag] = five;
        }
    }
    return map;
}

void ItemActionCnt::extract(Sample *sample, vector<ItemAction> list, string flag, string prefix) {
    Map<string, Five<int, int, int, int, int>> map = statPv(list, flag);
    //1.纯统计量
    vector<pair<long, float>> stat_list;
    for (auto it = map.begin(); it != map.end(); ++it) {
        string key = it->first;
        Five<int, int, int, int, int> five = it->second;
        string key_ = flag + key;
        stat_list.emplace_back(hash(key_ + "1") % hash_size + 1L, log(five.first));
        stat_list.emplace_back(hash(key_ + "2") % hash_size + 1L, log(five.second));
        stat_list.emplace_back(hash(key_ + "3") % hash_size + 1L, log(five.third));
        stat_list.emplace_back(hash(key_ + "4") % hash_size + 1L, log(five.fourth));
        stat_list.emplace_back(hash(key_ + "5") % hash_size + 1L, log(five.fifth));
    }
    if (!stat_list.empty()) {
        std::remove_if(stat_list.begin(), stat_list.end(), [](pair<long, float> i) {
            return i.second == 0.0;
        });
    }
    // padding
//    stat_list = padding(stat_list, flag); //对于item特征，都需要padding
    sample->addLongListWeight("stat_" + flag + "_" + prefix, stat_list);
    if (flag != "event") {  // template/search 不做ctr,cvr统计
        return;
    }

    Five<int, int, int, int, int> exp;
    if (map.contains("impressions_single"))
        exp = map.find("impressions_single")->second;

    Five<int, int, int, int, int> clk;
    if (map.contains("product_view"))
        clk = map.find("product_view")->second;

    Five<int, int, int, int, int> add;
    if (map.contains("add_to_cart"))
        add = map.find("add_to_cart")->second;

    Five<int, int, int, int, int> buy;
    if (map.contains("purchase_product"))
        buy = map.find("purchase_product")->second;

    //2.ctr
    Five<float, float, float, float, float> ctr = cxr(clk, exp);
    //3.cvr
    Five<float, float, float, float, float> cvr = cxr(buy, clk);
    //4.atr
    Five<float, float, float, float, float> atr = cxr(add, exp);
    //5.avr
    Five<float, float, float, float, float> avr = cxr(buy, add);

    vector<float> ctx;
    ctx.push_back(ctr.first);
    ctx.push_back(ctr.second);
    ctx.push_back(ctr.third);
    ctx.push_back(ctr.fourth);
    ctx.push_back(ctr.fifth);

    ctx.push_back(cvr.first);
    ctx.push_back(cvr.second);
    ctx.push_back(cvr.third);
    ctx.push_back(cvr.fourth);
    ctx.push_back(cvr.fifth);

    ctx.push_back(atr.first);
    ctx.push_back(atr.second);
    ctx.push_back(atr.third);
    ctx.push_back(atr.fourth);
    ctx.push_back(atr.fifth);

    ctx.push_back(avr.first);
    ctx.push_back(avr.second);
    ctx.push_back(avr.third);
    ctx.push_back(avr.fourth);
    ctx.push_back(avr.fifth);
    addFloatList(sample, "cxr_" + prefix, ctx);
}


Five<float, float, float, float, float>
ItemActionCnt::cxr(Five<int, int, int, int, int> clk, Five<int, int, int, int, int> exp) {
    Five<float, float, float, float, float> ctr;
    ctr.first = 0;
    ctr.second = 0;
    ctr.third = 0;
    ctr.fourth = 0;
    ctr.fifth = 0;
    if (clk.first * exp.first != 0)ctr.setFirst(((float) clk.first) / exp.first);
    if (clk.second * exp.second != 0)ctr.setSecond(((float) clk.second) / exp.second);
    if (clk.third * exp.third != 0)ctr.setThird(((float) clk.third) / exp.third);
    if (clk.fourth * exp.fourth != 0)ctr.setFourth(((float) clk.fourth) / exp.fourth);
    if (clk.fifth * exp.fifth != 0)ctr.setFifth(((float) clk.fifth) / exp.fifth);

    return ctr;
}


vector<pair<long, float>> ItemActionCnt::padding(vector<pair<long, float>> stat_list, string flag) {
    if (flag == "event")return padding_weight(stat_list, event_padding_size);
    if (flag == "ctx_event")return padding_weight(stat_list, ctx_padding_size);
    return padding_weight(stat_list, search_padding_size);
}