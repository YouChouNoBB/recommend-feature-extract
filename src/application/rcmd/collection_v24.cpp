//
// Created by Bryan Zhou on 2022/10/24.
//

#include "labelcontext.h"
#include "collection_v24.h"
#include "user_action_tag_series.h"
#include "user_lda.h"
#include "item_lda.h"
#include "item_base_info.h"
#include "item_event_cnt.h"
#include "item_ngram.h"
#include "user_item_lda_cross.h"
#include "outer/outer.h"
#include "user_action_series.h"
#include "labelcontext_item_hash.h"
#include "item_search_word.h"
#include "user_utm_info.h"

CollectionRcmdExtractorV24::CollectionRcmdExtractorV24() {
    int size_10 = 10;
    int size_100 = 100;
    int size_1w = 10000;
    int size_10w = 100000;
    int size_50w = 500000;
    int size_500w = 5000000;
    int size_1000w = 10000000;
    this->userFeatureTables = vector<TableExtractor *>{
            new LabelContextItemHash(size_50w,size_1000w),
            new UserActionSeries(User::click_item_series, size_50w, size_1000w),
            new UserActionSeries(User::cart_item_series, size_50w, size_1000w),
            new UserActionSeries(User::purchase_item_series, size_50w, size_1000w),
            new UserActionTagSeries(User::click_url_tag_series, size_500w),
            new UserActionTagSeries(User::cart_url_tag_series, size_500w),
            new UserActionTagSeries(User::purchase_url_tag_series, size_500w),
            new UserActionSeries(User::search_word_series, size_50w, size_500w),
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
            new ItemSearchWord(extractor::Item::search_word_exp, size_500w),
            new ItemSearchWord(extractor::Item::search_word_clk, size_500w),
            new ItemBaseInfo(size_50w,size_500w),
            new ItemNGram(extractor::Item::title_tags, extractor::Item::title_tags_3gram, 3, size_50w),
            new ItemNGram(extractor::Item::tags, extractor::Item::tags_3gram, 3, size_50w),
            new ItemEventCnt(extractor::Item::item_me_cnt) //size = 13
    };
    this->crossFeatureTables = vector<TableExtractor *>{};
}

CollectionRcmdExtractorV24::~CollectionRcmdExtractorV24() {
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
REGISTRY_CLASS(CollectionRcmdExtractorV24)

