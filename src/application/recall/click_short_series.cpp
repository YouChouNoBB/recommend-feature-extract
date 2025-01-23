//
// Created by Bryan Zhou on 2023/3/2.
//
#include "click_short_series.h"
#include "outer.h"
#include "labelcontext_item_hash.h"
#include "user_action_series.h"
#include "user_action_tag_series.h"
#include "item_ngram.h"
#include "item_action_cnt.h"
#include "item_base_info.h"
#include "user.h"
#include "item.h"
#include "dssm_short_click_context.h"
#include "item_search_word.h"
#include "user_simple_action_series.h"

ClickShortSeries::ClickShortSeries() {
    int hash_size = 500000;
    int item_hash_size = 10000000;
    this->userFeatureTables = vector<TableExtractor *>{
            new DssmShortClickContext(hash_size,item_hash_size),
            new UserSimpleActionSeries(User::click_series,hash_size,item_hash_size),
    };
    this->itemFeatureTables = vector<TableExtractor *>{
            new ItemBaseInfo(hash_size)
    };
    this->crossFeatureTables = vector<TableExtractor *>{};
}

ClickShortSeries::~ClickShortSeries() {
    for (auto it = userFeatureTables.begin(); it != userFeatureTables.end(); ++it) {
        delete *it;
    }
    for (auto it = itemFeatureTables.begin(); it != itemFeatureTables.end(); ++it) {
        delete *it;
    }
    for (auto it = crossFeatureTables.begin(); it != crossFeatureTables.end(); ++it) {
        delete *it;
    }
    userFeatureTables.clear();
    itemFeatureTables.clear();
    crossFeatureTables.clear();
    google::protobuf::ShutdownProtobufLibrary();
}

REGISTRY_CLASS(ClickShortSeries)