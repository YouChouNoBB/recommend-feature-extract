//
// Created by Bryan Zhou on 2022/10/24.
//

#include "labelcontext.h"
#include "collection_v23.h"
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
#include "item_search_word.h"

CollectionRcmdExtractorV23::CollectionRcmdExtractorV23() {
    int hash_size = 500000;
    int tag_hash_size = 5000000;
    int item_hash_size = 10000000;
    this->userFeatureTables = vector<TableExtractor *>{
            new LabelContextItemHash(hash_size,item_hash_size),
            new UserActionSeries(User::click_item_series, hash_size, item_hash_size),
            new UserActionSeries(User::cart_item_series, hash_size, item_hash_size),
            new UserActionSeries(User::purchase_item_series, hash_size, item_hash_size),
            new UserActionTagSeries(User::click_url_tag_series, tag_hash_size),
            new UserActionTagSeries(User::cart_url_tag_series, tag_hash_size),
            new UserActionTagSeries(User::purchase_url_tag_series, tag_hash_size),
            new UserActionSeries(User::search_word_series, hash_size, tag_hash_size)
    };
    this->itemFeatureTables = vector<TableExtractor *>{
            new ItemSearchWord(extractor::Item::search_word_exp, tag_hash_size),
            new ItemSearchWord(extractor::Item::search_word_clk, tag_hash_size),
            new ItemActionCnt(hash_size),
            new ItemBaseInfo(hash_size,tag_hash_size),
            new ItemNGram(extractor::Item::title_tags, extractor::Item::title_tags_3gram, 3, hash_size),
            new ItemNGram(extractor::Item::tags, extractor::Item::tags_3gram, 3, hash_size)
    };
    this->crossFeatureTables = vector<TableExtractor *>{};
}

CollectionRcmdExtractorV23::~CollectionRcmdExtractorV23() {
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
REGISTRY_CLASS(CollectionRcmdExtractorV23)

