//
// Created by Bryan Zhou on 2022/10/24.
//

#include "labelcontext.h"
#include "collection_v12.h"
#include "user_action_tag_series.h"
#include "user_lda.h"
#include "item_lda.h"
#include "item_base_info.h"
#include "item_action_cnt.h"
#include "item_ngram.h"
#include "user_item_lda_cross.h"
#include "outer/outer.h"
#include "user_action_series.h"
#include "labelcontext_item_hash.h"

CollectionRcmdExtractorV12::CollectionRcmdExtractorV12() {
    int hash_size = 500000;
    int item_hash_size = 10000000;
    this->userFeatureTables = vector<TableExtractor *>{
            new LabelContextItemHash(hash_size,item_hash_size),
            new UserLDA(),
            new UserItemLDACross(),
            new UserActionSeries(User::click_item_series,hash_size,item_hash_size),
            new UserActionSeries(User::cart_item_series,hash_size,item_hash_size),
            new UserActionSeries(User::purchase_item_series,hash_size,item_hash_size),
            new UserActionSeries(User::ignore_item_series,hash_size,item_hash_size)
    };
    this->itemFeatureTables = vector<TableExtractor *>{
        new ItemActionCnt(hash_size),
        new ItemBaseInfo(hash_size,20),
        new ItemLDA(100),
        new ItemNGram(3,hash_size,50)
    };
    this->crossFeatureTables = vector<TableExtractor *>{

    };
}

CollectionRcmdExtractorV12::~CollectionRcmdExtractorV12() {
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
REGISTRY_CLASS(CollectionRcmdExtractorV12)

