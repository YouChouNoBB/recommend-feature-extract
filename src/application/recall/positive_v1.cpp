//
// Created by Bryan Zhou on 2022/11/24.
//

#include "positive_v1.h"
#include "outer.h"
#include "labelcontext.h"
#include "user_action_series.h"
#include "user.h"
#include "softmax_label.h"
PositiveRecallV1::PositiveRecallV1(){
    int hash_size = 500000;
    int item_hash_size = 10000000;
    this->userFeatureTables = vector<TableExtractor *>{
            new SoftMaxLabel(hash_size,item_hash_size),
            new UserActionSeries(User::click_item_series,hash_size,item_hash_size),
            new UserActionSeries(User::cart_item_series,hash_size,item_hash_size),
            new UserActionSeries(User::purchase_item_series,hash_size,item_hash_size),
            new UserActionSeries(User::ignore_item_series,hash_size,item_hash_size)
    };
    this->itemFeatureTables = vector<TableExtractor *>{};
    this->crossFeatureTables = vector<TableExtractor *>{};
}

PositiveRecallV1::~PositiveRecallV1(){
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

REGISTRY_CLASS(PositiveRecallV1)