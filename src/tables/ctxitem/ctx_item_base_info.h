//
// Created by Bryan Zhou on 2022/10/25.
//

#ifndef EXTRACTOR_ITEM_BASE_INFO_H
#define EXTRACTOR_ITEM_BASE_INFO_H

#include "table_extractor.h"
#include "ctxitem.h"
#include "label.h"

class CtxItemBaseInfo : public TableExtractor {
public:
    CtxItemBaseInfo(int hash_size) : hash_size(hash_size) {}

    ~CtxItemBaseInfo() {}

    /**
     * ctx_item的base特征
     * title_tags,price_min,price_max,created_at,updated_at,published_at
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features) {
        // title_tags,price_min,price_max,created_at,updated_at,published_at
        // padding
//        addLongList(sample, extractor::Item::title_tags, features, extractor::Item::title_tags, hash_size,padding_size);
        addLongList(sample, CtxItem::ctx_title_tags, features, CtxItem::ctx_title_tags, hash_size);
        vector <float> list ;
        for (int i = 0; i < 5; i++)list.push_back(MIN_FLOAT);

        int i = 0;
        if (features->contains(CtxItem::ctx_price_min)) {
            list[i] = log(features->find(CtxItem::ctx_price_min)->second.float_value());
        }
        i++;

        if (features->contains(CtxItem::ctx_price_max)) {
            list[i]= log(features->find(CtxItem::ctx_price_min)->second.float_value());
        }
        i++;

        long now_time;
        if (!features->contains(Label::time)) {
            now_time = now();
        } else {
            now_time = features->find(Label::time)->second.long_value();
        }

        if (features->contains(CtxItem::ctx_created_at) ) {
            float value = log(diffDay(now_time, features->find(CtxItem::ctx_created_at)->second.long_value()));
            list[i]= value;
        }
        i++;

        if (features->contains(CtxItem::ctx_updated_at)) {
            float value = log(diffDay(now_time, features->find(CtxItem::ctx_updated_at)->second.long_value()));
            list[i]= value;
        }
        i++;

        if (features->contains(CtxItem::ctx_published_at) ) {
            float value = log(diffDay(now_time, features->find(CtxItem::ctx_published_at)->second.long_value()));
            list[i]= value;
        }

        addFloatList(sample, CtxItem::ctx_price_and_3diff, list);
    }


private:
    int hash_size;
};

#endif //EXTRACTOR_ITEM_BASE_INFO_H
