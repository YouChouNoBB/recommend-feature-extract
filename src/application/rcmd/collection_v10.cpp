//
// Created by Bryan Zhou on 2022/10/24.
//

#include "labelcontext.h"
#include "collection_v10.h"
#include "user_action_tag_series.h"
#include "user_lda.h"
#include "item_lda.h"
#include "item_base_info.h"
#include "item_action_cnt.h"
#include "item_ngram.h"
#include "user_item_lda_cross.h"
#include "outer/outer.h"
CollectionRcmdExtractorV10::CollectionRcmdExtractorV10() {
    this->userFeatureTables = vector<TableExtractor *>{
            new LabelContext(500000),
            new UserLDA(),
            new UserItemLDACross()
    };
    this->itemFeatureTables = vector<TableExtractor *>{
        new ItemActionCnt(500000),
        new ItemBaseInfo(500000,20),
        new ItemLDA(100),
        new ItemNGram(3,500000,50)
    };
    this->crossFeatureTables = vector<TableExtractor *>{

    };
}

CollectionRcmdExtractorV10::~CollectionRcmdExtractorV10() {
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
REGISTRY_CLASS(CollectionRcmdExtractorV10);

void CollectionRcmdExtractorV10::fun() {

}

