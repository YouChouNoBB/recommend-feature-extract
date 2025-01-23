//
// Created by Bryan Zhou on 2023/3/2.
//

#ifndef EXTRACTOR_ITEM_SEARCH_WORD_H
#define EXTRACTOR_ITEM_SEARCH_WORD_H

#include "sample.h"
#include "features.pb.h"
#include "item.h"
#include "table_extractor.h"

using namespace extractor;

class ItemSearchWord : public TableExtractor {
public:
    ItemSearchWord(string key, int hash_size) : key(key), hash_size(hash_size) {}

    void extract(Sample *sample, Map<string, Feature> *features) {
        vector<pair<long, float>> list;
        if (features->contains(key)) {
            for (auto kv: features->find(key)->second.str_int_list().value()) {
                list.emplace_back(hash(kv.key()) % hash_size + 1, log(kv.value()));
            }
        }
        if (!list.empty()) {
            std::remove_if(list.begin(), list.end(), [](pair<long, float> i) {
                return i.second == 0.0;
            });
        }
        sample->addLongListWeight(key, list);
    }

private:
    string key;
    int hash_size;

};

#endif //EXTRACTOR_ITEM_SEARCH_WORD_H
