//
// Created by Bryan Zhou on 2023/3/9.
//

#include "search_v14.h"
#include "outer.h"
#include "labelcontext_item_hash.h"
#include "user_action_series.h"
#include "user_action_tag_series.h"
#include "item_ngram.h"
#include "item_action_cnt.h"
#include "item_base_info.h"
#include "query_splits.h"
#include "query_ngram.h"
#include "query_click_tag.h"
#include "user.h"
#include "item.h"
#include "query.h"
#include "item_search_word.h"
#include "item_query_sim.h"

SearchExtractorV14::SearchExtractorV14() {
    int hash_size = 500000;
    int item_hash_size = 10000000;
    this->userFeatureTables = vector<TableExtractor *>{
            new LabelContextItemHash(hash_size,item_hash_size),
            new UserActionSeries(User::click_item_series, hash_size, item_hash_size),
            new UserActionSeries(User::cart_item_series, hash_size, item_hash_size),
            new UserActionSeries(User::purchase_item_series, hash_size, item_hash_size),
            new UserActionTagSeries(User::click_url_tag_series, hash_size),
            new UserActionTagSeries(User::cart_url_tag_series, hash_size),
            new UserActionTagSeries(User::purchase_url_tag_series, hash_size),
            new UserActionSeries(User::search_word_series, hash_size, hash_size),
            new QueryClickTag(hash_size),
            new QueryNGram(3,hash_size,Query::query_3gram),
            new QueryNGram(4,hash_size,Query::query_4gram),
            new QuerySplits(hash_size)
    };
    int tag_hash_size = 5000000; //只有tags,title_tags是500w
    this->itemFeatureTables = vector<TableExtractor *>{
            new ItemSearchWord(extractor::Item::search_word_exp, hash_size),
            new ItemSearchWord(extractor::Item::search_word_clk, hash_size),
            new ItemActionCnt(hash_size),
            new ItemBaseInfo(hash_size,tag_hash_size),
            new ItemNGram(extractor::Item::title_tags, extractor::Item::title_tags_3gram, 3, hash_size),
            new ItemNGram(extractor::Item::tags, extractor::Item::tags_3gram, 3, hash_size),
            new ItemQuerySim()
    };
    this->crossFeatureTables = vector<TableExtractor *>{};
}

SearchExtractorV14::~SearchExtractorV14() {
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
REGISTRY_CLASS(SearchExtractorV14)