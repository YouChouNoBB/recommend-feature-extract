//
// Created by Bryan Zhou on 2022/11/28.
//
#include "user_action_series.h"
#include "triple.h"
#include "jsonutil.h"

void UserActionSeries::extract(Sample *sample, Map<string, Feature> *features){
    long now_time;
    if(!features->contains(Label::time)){
        now_time= now();
    }else{
        now_time=features->find(Label::time)->second.long_value();
    }
    //实时特征合并到离线特征
    mergeActionSeries(features,key);
    extractActionSeries(sample,features,key,now_time);
}

//合并离线和在线数据
void UserActionSeries::mergeActionSeries(Map<string, Feature> *features, string key){
    string rt_key = "rt_" + key;
    if (features->contains(key)) {
        if (features->contains(rt_key)) { //离线有，实时也有，需要合并
            Feature offline = features->find(key)->second;
            Feature online = features->find(rt_key)->second;
            mergeSeries(&offline, &online);
            (*features)[key] = offline;
        }//离线有，实时没有，不需要处理

    } else if (features->contains(rt_key)) { //离线没有，只有实时有,把实时的写到离线key中
        (*features)[key]= features->find(rt_key)->second;
    }
}

//实时和离线都有,单测ok
void UserActionSeries::mergeSeries(Feature *offline,Feature *online){
    //这里在线离线协议是一样的，处理会方便很多
    auto online_map = online->rt_user_action_list().items();
    auto offline_map = offline->mutable_rt_user_action_list()->mutable_items();
    //遍历在线特征，离线有就合并，没有就塞进去，返回离线特征map
    for (auto it = online_map.begin(); it != online_map.end(); ++it) {
        string key = it->first;
        Item value = it->second;
        Item off_value = (*offline_map)[key];

        if (offline_map->contains(key)) { //离线有，需要合并
            mergeItem(&off_value,&value);
            (*offline_map)[key] = off_value;
        } else {
            (*offline_map)[key] = value; //离线没有，直接塞进去
        }
    }
}

// 单测ok
//这里改成void* 是为了防止头文件Item和extractor::Item冲突
void UserActionSeries::mergeItem(void *off,void *on){
    Item *offline = reinterpret_cast<Item*>(off);
    Item *online = reinterpret_cast<Item*>(on);

    //collection
    if(offline->collection_id().empty() && !online->collection_id().empty())
        offline->set_collection_id(online->collection_id());

    //first_time
    if(online->first_time()<offline->first_time())
        offline->set_first_time(online->first_time());

    //last_second_time
    long min_last_time = min(offline->last_time(),online->last_time());
    long max_second_last_time = max(offline->last_second_time(),online->last_second_time());
    offline->set_last_second_time(min(min_last_time,max_second_last_time));

    //last_time
    if(online->last_time()>offline->last_time())
        offline->set_last_time(online->last_time());
    //cnt
    offline->set_cnt(offline->cnt()+online->cnt());
}

void UserActionSeries::extractActionSeries(Sample *sample, Map<string, Feature> *features, string key, long now){
    vector<Triple<long, long, int>> triples;
    //cnt/rate feature
    vector<pair<long, float>> itemCnt;
    vector<pair<long, float>> itemRate;

    //ordered feature
    vector<pair<long, float>> itemOrderdByTime;
    vector<pair<long, float>> itemOrderdByCnt;

    //time to now diff
    vector<pair<long, float>> itemFirstTimeDiff;
    vector<pair<long, float>> itemLSecondTimeDiff;
    vector<pair<long, float>> itemLastTimeDiff;

    //var time diff
    vector<pair<long, float>> firstLastTimeDiff;
    vector<pair<long, float>> secondLastTimeDiff;
    vector<pair<long, float>> firstSecondTimeDiff;

    //avg time diff
    vector<pair<long, float>> itemAvgTimeDiff;

    //collection_id cnt
    unordered_map<long,int> collection_cnt_map;
    vector<pair<long, float>> collectionCnt;

    int cntSum = 0;
    if (features->contains(key)) {
        auto list = features->find(key)->second.rt_user_action_list().items();

        for (auto pair: list) {
            Item userAction = pair.second;
            long item = hash(userAction.item_id()) % item_hash_size + 1;
            long collection_id = hash(userAction.collection_id())% hash_size + 1;
            long firstTime = userAction.first_time();
            long lSecondTime = userAction.last_second_time();
            long lastTime = userAction.last_time();
            int cnt = userAction.cnt();

            if(collection_cnt_map.contains(collection_id)){
                collection_cnt_map[collection_id] += cnt;
            }else{
                collection_cnt_map[collection_id] = cnt;
            }

            triples.emplace_back(item, lastTime, cnt);

            // log(1) ==0
            float cntValue = log(cnt);
            itemCnt.emplace_back(item, cntValue);
            itemRate.emplace_back(item, (float) cnt);

            //to calc rate
            cntSum += cnt;

            float diff = log(diffDay(now, firstTime));
            if(!isnan(diff)){
                itemFirstTimeDiff.emplace_back(item, diff);
            }

            diff = log(diffDay(now, lSecondTime));
            if(!isnan(diff)){
                itemLSecondTimeDiff.emplace_back(item, diff);
            }

            diff = log(diffDay(now, lastTime));
            if(!isnan(diff)){
                itemLastTimeDiff.emplace_back(item, diff);
            }

            diff = log(diffDay(lastTime, firstTime));
            float avg_diff = diff / cnt;
            if(!isnan(diff)){
                firstLastTimeDiff.emplace_back(item, diff);
            }
            if(!isnan(avg_diff)){
                itemAvgTimeDiff.emplace_back(item, avg_diff);
            }

            diff = log(diffDay(lastTime, lSecondTime));
            if(!isnan(diff)){
                secondLastTimeDiff.emplace_back(item, diff);
            }

            diff = log(diffDay(lSecondTime, firstTime));
            if(!isnan(diff)){
                firstSecondTimeDiff.emplace_back(item, diff);
            }

        }
        // collection cnt
        map2vec(collection_cnt_map,&collectionCnt);

        int sum = cntSum; //should be final sum
        for (auto x = itemRate.begin(); x != itemRate.end(); ++x) {
            x->second = x->second / sum;
        }

        // sort by cnt
        auto cmp_by_cnt = [](Triple<long, long, int> &x, Triple<long, long, int> &y) { return x.third > y.third; };
        std::sort(triples.begin(), triples.end(), cmp_by_cnt);

        int start = 1;
        for (auto triple: triples) {
            itemOrderdByCnt.emplace_back(triple.first, 1.0F / start++);
        }

        // sort by last time
        auto cmp_by_last_time = [](Triple<long, long, int> &x, Triple<long, long, int> &y) {
            return x.second > y.second;
        };
        sort(triples.begin(), triples.end(), cmp_by_last_time);
        start = 1;
        for (auto triple: triples) {
            itemOrderdByTime.emplace_back(triple.first, 1.0F / start++);
        }


        // add to sample
        sample->addLongListWeight(key + "_cnt", itemCnt);
        sample->addLongListWeight(key + "_rate", itemRate);
        sample->addLongListWeight(key + "_orderd_by_time", itemOrderdByTime);
        sample->addLongListWeight(key + "_orderd_by_cnt", itemOrderdByCnt);
        sample->addLongListWeight(key + "_first_diff", itemFirstTimeDiff);
        sample->addLongListWeight(key + "_last_second_diff", itemLSecondTimeDiff);
        sample->addLongListWeight(key + "_last_diff", itemLastTimeDiff);
        sample->addLongListWeight(key + "_first_last_diff", firstLastTimeDiff);
        sample->addLongListWeight(key + "_second_last_diff", secondLastTimeDiff);
        sample->addLongListWeight(key + "_first_second_diff", firstSecondTimeDiff);
        sample->addLongListWeight(key + "_avg_diff", itemAvgTimeDiff);
        sample->addLongListWeight(key + "_clct_cnt", collectionCnt);
    }


}