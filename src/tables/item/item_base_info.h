//
// Created by Bryan Zhou on 2022/10/25.
//

#ifndef EXTRACTOR_ITEM_BASE_INFO_H
#define EXTRACTOR_ITEM_BASE_INFO_H

#include "table_extractor.h"
#include "item.h"
#include "label.h"

using namespace extractor;

class ItemBaseInfo : public TableExtractor {
public:
    ItemBaseInfo(int hash_size, int tag_hash_size) : hash_size(hash_size), tag_hash_size(tag_hash_size) {}

    ItemBaseInfo(int hash_size) : hash_size(hash_size) {tag_hash_size = hash_size;}

    ~ItemBaseInfo() {}

    /**
     * item基础特征,title_tags,price_min,price_max,created_at,updated_at,published_at
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features) {
//        int tag_hash_size=5000000;
        // title_tags,price_min,price_max,created_at,updated_at,published_at
        addLongList(sample, extractor::Item::title_tags, features, extractor::Item::title_tags, tag_hash_size);
        addLongList(sample, extractor::Item::tags, features, extractor::Item::tags, tag_hash_size);
        vector<float> list;
        for (int i = 0; i < 5; i++)list.push_back(MIN_FLOAT);

        int i = 0;
        if (features->contains(extractor::Item::price_min)) {
            list[i] = log(features->find(extractor::Item::price_min)->second.float_value());
        }
        i++;

        if (features->contains(extractor::Item::price_max)) {
            list[i] = log(features->find(extractor::Item::price_min)->second.float_value());
        }
        i++;

        long now_time;
        if (!features->contains(Label::time)) {
            now_time = now();
        } else {
            now_time = features->find(Label::time)->second.long_value();
        }

        if (features->contains(extractor::Item::created_at)) {
            float value = log(diffDay(now_time, features->find(extractor::Item::created_at)->second.long_value()));
            list[i] = value;
        }
        i++;

        if (features->contains(extractor::Item::updated_at)) {
            float value = log(diffDay(now_time, features->find(extractor::Item::updated_at)->second.long_value()));
            list[i] = value;
        }
        i++;

        if (features->contains(extractor::Item::published_at)) {
            float value = log(diffDay(now_time, features->find(extractor::Item::published_at)->second.long_value()));
            list[i] = value;
        }

        addFloatList(sample, extractor::Item::price_and_3diff, list);

        if (features->contains(extractor::Item::cate1)) {
            addLong(sample, extractor::Item::cate1, features, extractor::Item::cate1, hash_size);
        }

        if (features->contains(extractor::Item::cate2)) {
            addLong(sample, extractor::Item::cate2, features, extractor::Item::cate2, hash_size);
        }

        if (features->contains(extractor::Item::cate3)) {
            addLong(sample, extractor::Item::cate3, features, extractor::Item::cate3, hash_size);
        }

        if (features->contains(extractor::Item::collections)) {
            addLongList(sample, extractor::Item::collections, features, extractor::Item::collections, hash_size);
        }

    }


private:
    int hash_size;
    int tag_hash_size;
};

#endif //EXTRACTOR_ITEM_BASE_INFO_H
