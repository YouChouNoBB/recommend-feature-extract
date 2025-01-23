//
// Created by Bryan Zhou on 2022/10/25.
//

#ifndef EXTRACTOR_USER_UTM_INFO_H
#define EXTRACTOR_USER_UTM_INFO_H

#include "user.h"
#include "table_extractor.h"
#include "label.h"
#include "mock.h"

class UserUtmInfo : public TableExtractor {
public:
    UserUtmInfo(string key, int hash_size) : key(key), hash_size(hash_size) {}

    ~UserUtmInfo() {}

    /**
     * 用户广告utm携带的信息，包含与实时特征合并
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features) {
        // 合并离线和实时特征
        mergeRT(features, key);
        if (features->contains(key)) {
            addLongList(sample, key, features, key, hash_size);
        }
    }

    void mergeRT(Map<string, Feature> *features, string feature) {
        string rt_key = "rt_" + feature;
        // 离线没有,在线有，直接放进去
        if (!features->contains(feature) && features->contains(rt_key)) {
            (*features)[feature] = (*features)[rt_key];
        }
        // 离线有，在线没有，不需要处理
        // 离线没有，在线没有，不需要处理
        // 离线有，在线有，需要合并
        if (features->contains(feature) && features->contains(rt_key)) {
            (*features)[feature] = merge((*features)[feature], (*features)[rt_key]);
        }
    }

    Feature merge(Feature f1, Feature f2) {
        set<string> s;
        for (auto x: f1.str_list().value()) {
            s.emplace(x);
        }
        for (auto x: f2.str_list().value()) {
            s.emplace(x);
        }
        return strListFeature(s);
    }

private:
    string key;
    int hash_size;
};

#endif //EXTRACTOR_USER_UTM_INFO_H
