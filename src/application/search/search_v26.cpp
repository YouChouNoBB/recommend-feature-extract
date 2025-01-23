//
// Created by Bryan Zhou on 2023/8/16.
//

#include "search_v26.h"
#include "outer.h"
#include "labelcontext_item_hash.h"
#include "user_action_series.h"
#include "user_action_tag_series.h"
#include "item_ngram.h"
#include "item_event_cnt.h"
#include "item_base_info.h"
#include "query_splits.h"
#include "query_ngram.h"
#include "query_click_tag.h"
#include "user.h"
#include "item.h"
#include "query.h"
#include "item_search_word.h"
#include "user_utm_info.h"
#include "item_query_sim.h"

SearchV26::SearchV26() {
    int hash_size = 500000;
    int item_hash_size = 10000000;
    int size_10 = 10;
    int size_100 = 100;
    int size_1w = 10000;
    int size_10w = 100000;
    int size_50w = 500000;
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
            new QuerySplits(hash_size),
            new UserUtmInfo(User::utm_source,size_1w),
            new UserUtmInfo(User::utm_medium,size_10w),
            new UserUtmInfo(User::campaign_id,size_50w),
            new UserUtmInfo(User::ad_id,size_50w),
            new UserUtmInfo(User::utm_content,size_50w),
            new UserUtmInfo(User::utm_campaign,size_50w),
            new UserUtmInfo(User::utm_ad,size_50w),
            new UserUtmInfo(User::utm_placement,size_10),
            new UserUtmInfo(User::omega_utm_campaign,size_1w),
            new UserUtmInfo(User::utm_term,size_10w),
            new UserUtmInfo(User::utm_agid,size_1w),
            new UserUtmInfo(User::utm_name,size_1w),
            new UserUtmInfo(User::utm_id,size_1w),
            new UserUtmInfo(User::utm_aname,size_10),
            new UserUtmInfo(User::utm_term,size_10w),
            new UserUtmInfo(User::utm_site,size_100),
            new UserUtmInfo(User::utm_cname,size_100),
            new UserUtmInfo(User::utm_adset,size_50w),
            new UserUtmInfo(User::utm_os,size_100),
            new UserUtmInfo(User::utm_cid,size_1w),
            new UserUtmInfo(User::utm_category,size_1w),
            new UserUtmInfo(User::utm_GroupID,size_1w),
            new UserUtmInfo(User::utm_keyword,size_50w)
    };
    this->itemFeatureTables = vector<TableExtractor *>{
            new ItemSearchWord(extractor::Item::search_word_exp, hash_size),
            new ItemSearchWord(extractor::Item::search_word_clk, hash_size),
            new ItemEventCnt(extractor::Item::item_me_cnt), //size = 13
            new ItemBaseInfo(hash_size),
            new ItemNGram(extractor::Item::title_tags, extractor::Item::title_tags_3gram, 3, hash_size),
            new ItemNGram(extractor::Item::tags, extractor::Item::tags_3gram, 3, hash_size)
    };
    this->crossFeatureTables = vector<TableExtractor *>{
            new ItemQuerySim()
    };
}

SearchV26::~SearchV26() {
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
REGISTRY_CLASS(SearchV26)