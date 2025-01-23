//
// Created by Bryan Zhou on 2022/11/7.
//

#include "search_v2.h"
#include "labelcontext.h"
#include "collection_v10.h"
#include "user_action_tag_series.h"
#include "user_lda.h"
#include "item_lda.h"
#include "item_base_info.h"
#include "item_action_cnt.h"
#include "item_ngram.h"
#include "user_item_lda_cross.h"
#include "outer.h"
#include "query_click_tag.h"
#include "query_lda.h"
#include "query_ngram.h"
#include "query_splits.h"
#include "query_item_lda_cross.h"
#include "item_query_sim.h"

SearchV2::SearchV2(){
    this->userFeatureTables = vector<TableExtractor *>{
            new LabelContext(500000),
            new UserLDA(),

            new QueryClickTag(500000),
            new QueryLDA(),
            new QueryNGram(3,500000),
            new QuerySplits(500000)
    };
    this->itemFeatureTables = vector<TableExtractor *>{
            new ItemActionCnt(500000),
            new ItemBaseInfo(500000,20),
            new ItemLDA(100),
            new ItemNGram(3,500000,50),

            new UserItemLDACross(),
            new QueryItemLDACross(),
            new ItemQuerySim()
    };
    this->crossFeatureTables = vector<TableExtractor *>{

    };
}

SearchV2::~SearchV2(){
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

REGISTRY_CLASS(SearchV2)