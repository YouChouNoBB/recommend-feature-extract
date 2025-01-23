//
// Created by Bryan Zhou on 2022/10/20.
//

#ifndef EXTRACTOR_USER_H
#define EXTRACTOR_USER_H

#include <string>

using namespace std;

namespace User {
    const string  country = "country";
    const string  client = "client";
    const string  exposure_item_series = "exposure_item_series";
    const string  click_item_series = "click_item_series";
    const string  cart_item_series = "cart_item_series";
    const string  checkout_item_series = "checkout_item_series";
    const string  purchase_item_series = "purchase_item_series";

    const string  rt_click_item_series = "rt_click_item_series";
    const string  rt_cart_item_series = "rt_cart_item_series";
    const string  rt_checkout_item_series = "rt_checkout_item_series";
    const string  rt_purchase_item_series = "rt_purchase_item_series";

    const string  click_collection_series = "click_collection_series";
    const string  ignore_item_series = "ignore_item_series";
    const string  ignore_collection_series = "ignore_collection_series";

    const string  click_url_tag_series = "click_url_tag_series";
    const string  cart_url_tag_series = "cart_url_tag_series";
    const string  purchase_url_tag_series = "purchase_url_tag_series";

    const string  rt_click_url_tag_series = "rt_click_url_tag_series";
    const string  rt_cart_url_tag_series = "rt_cart_url_tag_series";
    const string  rt_purchase_url_tag_series = "rt_purchase_url_tag_series";

    const string  user_lda_v1 = "user_lda_v1";
    const string  search_word_series = "search_word_series";

    //dssm short click series
    const string  click_series = "click_series";

    // utm info
    const string utm_source = "utm_source";
    const string utm_medium =  "utm_medium";
    const string campaign_id = "campaign_id";
    const string ad_id = "ad_id";
    const string utm_content = "utm_content";
    const string utm_campaign = "utm_campaign";
    const string utm_ad = "utm_ad";
    const string utm_placement = "utm_placement";
    const string omega_utm_campaign = "omega_utm_campaign";
    const string utm_term = "utm_term";
    const string utm_agid = "utm_agid";
    const string utm_name = "utm_name";
    const string utm_id = "utm_id";
    const string utm_aname = "utm_aname";
    const string utm_site = "utm_site";
    const string utm_cname = "utm_cname";
    const string utm_adset = "utm_adset";
    const string utm_os = "utm_os";
    const string utm_cid = "utm_cid";
    const string utm_category = "utm_category";
    const string utm_GroupID = "utm_GroupID";
    const string utm_keyword = "utm_keyword";

    const string rt_utm_source = "rt_utm_source";
    const string rt_utm_medium = "rt_utm_medium";
    const string rt_campaign_id = "rt_campaign_id";
    const string rt_ad_id = "rt_ad_id";
    const string rt_utm_content = "rt_utm_content";
    const string rt_utm_campaign = "rt_utm_campaign";
    const string rt_utm_ad = "rt_utm_ad";
    const string rt_utm_placement = "rt_utm_placement";
    const string rt_omega_utm_campaign = "rt_omega_utm_campaign";
    const string rt_utm_term = "rt_utm_term";
    const string rt_utm_agid = "rt_utm_agid";
    const string rt_utm_name = "rt_utm_name";
    const string rt_utm_id = "rt_utm_id";
    const string rt_utm_aname = "rt_utm_aname";
    const string rt_utm_site = "rt_utm_site";
    const string rt_utm_cname = "rt_utm_cname";
    const string rt_utm_adset = "rt_utm_adset";
    const string rt_utm_os = "rt_utm_os";
    const string rt_utm_cid = "rt_utm_cid";
    const string rt_utm_category = "rt_utm_category";
    const string rt_utm_GroupID = "rt_utm_GroupID";
    const string rt_utm_keyword = "rt_utm_keyword";

};


#endif //EXTRACTOR_USER_H
