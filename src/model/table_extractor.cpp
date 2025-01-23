//
// Created by Bryan Zhou on 2022/10/20.
//
#include "table_extractor.h"
#include <string>
#include <iostream>
#include "triple.h"
#include <algorithm>
#include "jsonutil.h"
#include "outer.h"

using namespace std;

void TableExtractor::addLabel(Sample *sample, const Feature *feature) {
    if (feature == nullptr) {
        sample->addLabel(vector<float>{0.0});
        return;
    }
    vector<float> res;
    for (auto x: feature->float_list().value()) {
        res.emplace_back(x);
    }
    sample->addLabel(res);
}

void TableExtractor::addLabel(Sample *sample, Map<string, Feature> *features, string key) {
    if (features->contains(key))
        addLabel(sample, &(*features)[key]);
    else
        addLabel(sample, nullptr);
}

void TableExtractor::addLong(Sample *sample, string name, const Feature *feature, int hash_size) {
    if (feature == nullptr) {
        sample->add(name, 0L);
        return;
    }
    if (feature->has_long_value()) {
        long value = feature->long_value() % hash_size + 1;
        sample->add(name, value);
        return;
    }
    if (feature->has_str_value()) {
        long value = hash(feature->str_value()) % hash_size + 1;
        sample->add(name, value);
        return;
    }
    if (feature->has_int_value()) {
        long value = feature->int_value() % hash_size + 1L;
        sample->add(name, value);
        return;
    }
    sample->add(name, 0L);
}

void TableExtractor::addLong(Sample *sample, string name, Map<string, Feature> *features, string key, int hash_size) {
    if (features->contains(key))
        addLong(sample, name, &(*features)[key], hash_size);
    else
        addLong(sample, name, nullptr, hash_size);
}


void TableExtractor::addString(Sample *sample, string name, const Feature *feature) {
    if (feature == nullptr) {
        sample->add(name, "");
        return;
    }
    if (feature->has_str_value()) {
        sample->add(name, feature->str_value());
        return;
    }
    if (feature->has_long_value()) {
        sample->add(name, std::to_string(feature->long_value()));
        return;
    }
    if (feature->has_int_value()) {
        sample->add(name, std::to_string(feature->int_value()));
        return;
    }
    sample->add(name, "");
}

void TableExtractor::addString(Sample *sample, string name, Map<string, Feature> *features, string key) {
    if (features->contains(key))
        addString(sample, name, &(*features)[key]);
    else
        addString(sample, name, nullptr);
}

void TableExtractor::addFloat(Sample *sample, string name, const Feature *feature) {
    if (feature == nullptr) {
        sample->add(name, MIN_FLOAT);
    } else {
        float value = feature->float_value();
        if (value == 0.0) value = MIN_FLOAT;
        sample->add(name, value);
    }
}

void TableExtractor::addFloat(Sample *sample, string name, Map<string, Feature> *features, string key) {
    if (features->contains(key))
        addFloat(sample, name, &features->find(key)->second);
    else
        addFloat(sample, name, nullptr);
}

/**
 * item特征需要长度对齐，特别是title_tags
 * @param sample
 * @param name
 * @param feature
 * @param max_hash
 * @param padding_size
 */
void TableExtractor::addLongList(Sample *sample, string name, const Feature *feature, int max_hash, int padding_size) {
    vector<long> list;
    if (feature != nullptr && feature->has_str_list()) {
        for (auto x: feature->str_list().value()) {
            list.emplace_back(hash(x) % max_hash + 1);
        }
    }
    if (feature != nullptr && feature->has_long_list()) {
        for (auto x: feature->long_list().value()) {
            list.emplace_back(x);
        }
    }

    if (padding_size < list.size()) {
        vector<long> res(list.begin(), list.begin() + padding_size);
        sample->addLongList(name, res);
        return;
    }

    if (padding_size == list.size()) {
        sample->addLongList(name, list);
        return;
    }

    vector<float> value;
    for (int i = 0; i < list.size(); i++) {
        value.push_back(1.0f);
    }
    while (value.size() < padding_size) {
        list.push_back(0L);
        value.push_back(MIN_FLOAT);
    }
    sample->addLongListWeight(name, list, value);
}

void TableExtractor::addLongList(Sample *sample, string name, Map<string, Feature> *features, string key, int max_hash,
                                 int padding_size) {
    if (features->contains(key))
        addLongList(sample, name, &features->find(key)->second, max_hash, padding_size);
    else {
        addLongList(sample, name, nullptr, max_hash, padding_size);
    }
}

void TableExtractor::addLongList(Sample *sample, string name, const Feature *feature, int max_hash) {
    vector<long> list;
    if (feature == nullptr) {
        list.push_back(0L);
        sample->addLongList(name, list);
        return;
    }
    if (feature->has_str_list()) {
        for (auto x: feature->str_list().value()) {
            list.push_back(hash(x) % max_hash + 1L);
        }
        sample->addLongList(name, list);
        return;
    }
    if (feature->has_long_list()) {
        for (auto x: feature->long_list().value()) {
            list.push_back(x);
        }
        sample->addLongList(name, list);
    }
}

void
TableExtractor::addLongList(Sample *sample, string name, Map<string, Feature> *features, string key, int max_hash) {
    if (features->contains(key))
        addLongList(sample, name, &features->find(key)->second, max_hash);
    else {
        addLongList(sample, name, nullptr, max_hash);
    }
}

void TableExtractor::addFloatList(Sample *sample, string name, const Feature *feature) {
    vector<long> index;
    vector<float> value;
    if (feature != nullptr) {
        auto list = feature->float_list().value();
        for (int i = 0; i < list.size(); i++) {
            if (list[i] != 0.0) {
                index.push_back(i + 1L);
                value.push_back(list[i]);
            } else {
                index.push_back(i + 1L);
                value.push_back(MIN_FLOAT);
            }
        }
    }
    sample->addLongListWeight(name, index, value);
}

void TableExtractor::addFloatList(Sample *sample, string name, Map<string, Feature> *features, string key) {
    if (features->contains(key))
        addFloatList(sample, name, &features->find(key)->second);
    else
        addFloatList(sample, name, nullptr);
}

void TableExtractor::addFloatList(Sample *sample, string name, const vector<float> &list) {
    vector<long> index;
    vector<float> value;
    for (int i = 0; i < list.size(); i++) {
        if (list[i] != 0.0) {
            index.push_back(i + 1L);
            value.push_back(list[i]);
        } else {
//            index.push_back(i + 1L);
//            value.push_back(MIN_FLOAT);
        }
    }
    sample->addLongListWeight(name, index, value);
}

void TableExtractor::extractAction(Sample *sample, Map<string, Feature> *features, string key, long now, int hash_size) {
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

    int cntSum = 0;
    if (features->contains(key)) {
        auto list = features->find(key)->second.user_action_list().value();

        for (UserAction userAction: list) {
            long item = hash(userAction.item_id()) % hash_size + 1;
            long firstTime = userAction.first_time();
            long lSecondTime = userAction.last_second_time();
            long lastTime = userAction.last_time();
            int cnt = userAction.cnt();

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
    }


}

//合并离线和实时特征，统一写回到离线,主要针对click_url_tag_series
void TableExtractor::mergeRTAction(Map<string, Feature> *features, string key) {
    string rt_key = "rt_" + key;
    if (features->contains(key)) {
        if (features->contains(rt_key)) { //离线有，实时也有，需要合并
            Feature offline = features->find(key)->second;
            Feature online = features->find(rt_key)->second;
            Feature update = merge(&offline, &online);
            (*features)[key] = update;
        }//离线有，实时没有，不需要处理

    } else if (features->contains(rt_key)) { //离线没有，只有实时有,把实时的写到离线key中
        Feature feature = features->find(rt_key)->second;
        auto itemMap = feature.rt_user_action_list().items();
        UserActionList *userActionList = new UserActionList();
        for (auto it = itemMap.begin(); it != itemMap.end(); ++it) {
            Item item = it->second;
            UserAction userAction = rt2off(&item);
            userActionList->mutable_value()->Add(std::move(userAction));
        }
        Feature new_feature;
        new_feature.set_allocated_user_action_list(userActionList);
        features->emplace(key, new_feature);
    }

}


//离线特征和实时特征都有的情况
Feature TableExtractor::merge(const Feature *offline, const Feature *online) {
    //在线离线协议不同，所以需要单独处理，这里是一个设计失误
    auto online_map = online->rt_user_action_list().items();
    auto offline_map = list2map(offline->user_action_list().value());

    for (auto it = online_map.begin(); it != online_map.end(); ++it) {
        UserAction update;
        string key = it->first;
        Item value = it->second;
        if (offline_map.contains(key)) {
            update = merge(&offline_map.find(key)->second, &value);
        } else {
            update = rt2off(&value);
        }
        offline_map[key] = std::move(update);
    }
    vector<UserAction> list;
    for (auto it = offline_map.begin(); it != offline_map.end(); ++it) {
        list.emplace_back(it->second);
    }
    UserActionList *userActionList = new UserActionList();
    userActionList->mutable_value()->Assign(list.begin(), list.end());
    Feature feature;
    feature.set_allocated_user_action_list(userActionList);
    return feature;
}

//同一个item 合并离线和实时特征
UserAction TableExtractor::merge(const UserAction *offline, const Item *online) {
    UserAction userAction;
    userAction.set_item_id(offline->item_id().c_str());
    userAction.set_first_time(
            online->first_time() < offline->first_time() ? online->first_time() : offline->first_time()
    );

    // last_second_time 取数逻辑需要特别小心
    long min_last_time = std::min(online->last_time(), offline->last_time());
    long max_last_second_time = std::max(online->last_second_time(), offline->last_second_time());
    long last_second_time = std::min(min_last_time, max_last_second_time);
    userAction.set_last_second_time(last_second_time);

    userAction.set_last_time(
            online->last_time() > offline->last_time() ? online->last_time() : offline->last_time()
    );

    userAction.set_cnt(online->cnt() + offline->cnt());
    return userAction;
}


UserAction TableExtractor::rt2off(const Item *item) {
    UserAction userAction;
    //todo 这里可能有问题,需要写单测验证
    userAction.set_item_id(item->item_id().c_str());
    userAction.set_first_time(item->first_time());
    userAction.set_last_second_time(item->last_second_time());
    userAction.set_last_time(item->last_time());
    userAction.set_cnt((int) item->cnt());
    return userAction;
}


void TableExtractor::getNGram(string &query, int n, int hash_size, vector<pair<long, float>> &list) {
    int start = 0;
    while (start + n <= query.length()) {
        string gram = query.substr(start, n);
//        string gram = string(query.begin()+start,query.begin()+start+n);
        list.emplace_back(hash(gram) % hash_size + 1L, 1.0f);
        start++;
    }
}


vector<long> TableExtractor::padding_list(vector<long> &list, int padding_size) {
    while (list.size() < padding_size) {
        list.push_back(0L);
    }
    if (list.size() > padding_size) {
        return vector<long>(list.begin(), list.begin() + padding_size);
    }
    return list;
}


vector<pair<long, float>> TableExtractor::padding_weight(vector<pair<long, float>> &list, int padding_size) {
    while (list.size() < padding_size) {
        list.emplace_back(0L, MIN_FLOAT);
    }
    if (list.size() > padding_size) {
        // 降序排序,取top
        std::sort(list.begin(), list.end(), [](auto x, auto y) {
            return x.second > y.second;
        });
        return vector<pair<long, float>>(list.begin(), list.begin() + padding_size);
    }
    return list;

}

long TableExtractor::date2Unix(string s, string fs) {
    //https://blog.csdn.net/chengxiaili/article/details/90051984
    //用指针会导致多次调用返回值不一样,不能自己申请内存
//        struct tm* tmp_time = (struct tm*)malloc(sizeof(struct tm));
    if (dateMap.contains(s)) return dateMap[s];
    struct tm tmp_time;
    strptime(s.c_str(), fs.c_str(), &tmp_time);
    // 如果不设置下面3个字段，将影响并发，并且速度会很慢
    tmp_time.tm_isdst = 0;
    tmp_time.tm_gmtoff = 0;
    tmp_time.tm_zone = "UTC";
    time_t t = mktime(&tmp_time);
    return t;
}

void TableExtractor::map2vec(unordered_map<long,int> map,vector<pair<long,float>> *vec){
    for(pair<long,int> p:map){
        vec->emplace_back(p.first,log(p.second));
    }
}

string TableExtractor::PATTERN = "%Y-%m-%d %H:%M:%S";
string TableExtractor::SHORT_PATTERN = "%Y-%m-%d";