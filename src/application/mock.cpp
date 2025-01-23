//
// Created by Bryan Zhou on 2022/11/8.
//
#include "mock.h"
#include "user.h"
#include "item.h"
using namespace std;

Feature strFeature(string s) {
    Feature feature;
    feature.set_str_value(s);
    return feature;
}

Feature intFeature(int i) {
    Feature feature;
    feature.set_int_value(i);
    return feature;
}

Feature floatFeature(float i) {
    Feature feature;
    feature.set_float_value(i);
    return feature;
}

Feature longFeature(long i) {
    Feature feature;
    feature.set_long_value(i);
    return feature;
}

Feature strListFeature(vector<string> list) {
    Feature feature;
    for (auto x: list) {
        feature.mutable_str_list()->add_value(x);
    }
    return feature;
}

Feature strListFeature(set<string> list) {
    Feature feature;
    for (auto x: list) {
        feature.mutable_str_list()->add_value(x);
    }
    return feature;
}

Feature floatListFeature(vector<float> list) {
    Feature feature;
    for (auto x: list) {
        feature.mutable_float_list()->add_value(x);
    }
    return feature;
}

Feature strIntListFeature(vector<pair<string, int>> list) {
    Feature feature;
    for (auto x: list) {
        auto i = feature.mutable_str_int_list()->mutable_value()->Add();
        i->set_key(x.first);
        i->set_value(x.second);
    }
    return feature;
}

Feature userActionListFeature(vector<Five<string, long, long, long, int>> list) {
    Feature feature;
    for (auto x: list) {
        auto i = feature.mutable_user_action_list()->mutable_value()->Add();
        i->set_item_id(x.first.c_str());
        i->set_first_time(x.second);
        i->set_last_second_time(x.third);
        i->set_last_time(x.fourth);
        i->set_cnt(x.fifth);
    }
    return feature;
}

Feature rTUserActionList(vector<Five<string, long, long, long, int>> list) {
    Feature feature;
    auto i = feature.mutable_rt_user_action_list()->mutable_items();
    for (auto x: list) {
        Item item;
        item.set_item_id(x.first);
        item.set_first_time(x.second);
        item.set_last_second_time(x.third);
        item.set_last_time(x.fourth);
        item.set_cnt(x.fifth);
        i->insert(google::protobuf::MapPair<string, Item>(x.first, item));
    }
    return feature;
}

Feature itemActionFeature(vector<Seven<string, int, int, int, int, string, string>> list) {
    Feature feature;
    for (auto x: list) {
        auto i = feature.mutable_item_action_list()->mutable_value()->Add();
        i->set_event(x.first);
        i->set_cnt(x.second);
        i->set_dist_cnt(x.third);
        i->set_buy_cnt(x.fourth);
        i->set_click_cnt(x.fifth);
        i->set_context(x.six);
        i->set_date(x.seven);
    }
    return feature;
}

void mock_context(google::protobuf::Map<string, Feature> *map) {
    map->emplace("collection_id", strFeature("kjsfj236llqflj25jl"));
    map->emplace("item_id", strFeature("03ce4598-14bb-421b-8be3-7f1618f5932d"));
    map->emplace("label", floatFeature(0.0));
    map->emplace("time", longFeature(1680260670));
    map->emplace("store_id", intFeature(159459));
    map->emplace("user_id", longFeature(1654009979354424L));
    map->emplace("city", strFeature("Karachi"));
    map->emplace("province", strFeature("Sindh"));
    map->emplace("country", strFeature("Pakistan"));
    map->emplace("os", strFeature("mac"));
    map->emplace("scm", strFeature("collection.v23"));
}

void mock_query(google::protobuf::Map<string, Feature> *map) {
    map->emplace("query", strFeature("short dress,clothing"));
    map->emplace("query_lda_v1", floatListFeature(vector<float>{0.2f, 0.1f, 0.35f}));
    map->emplace("query_click_tag", strIntListFeature(vector<pair<string, int>>{{"short", 1}}));
}

void mock_ctx_item(google::protobuf::Map<string, Feature> *map) {
    map->emplace("ctx_title_tags", strListFeature(vector<string>{"shorter", "dressor"}));
    map->emplace("ctx_item_lda_v1", floatListFeature(vector<float>{0.2f, 0.1f, 0.35f}));
    map->emplace("ctx_item_stat", itemActionFeature(vector<Seven<string, int, int, int, int, string, string>>{
            {"product_view",       12, 5,  1, 2, "collection", "2022-09-01"},
            {"impressions_single", 56, 11, 1, 6, "collection", "2022-09-01"},
    }));
}

void mock_item(google::protobuf::Map<string, Feature> *map) {
    map->emplace("cate1", strFeature("clothing"));
    map->emplace("cate2", strFeature("under"));
    map->emplace("cate3", strFeature("under ware"));
    map->emplace("collections", strListFeature(vector<string>{"afhahoa25-af5fa-fa", "25af-af5-af-g3-ag"}));
    map->emplace("title_tags", strListFeature(vector<string>{"short", "dress"}));
    map->emplace("tags", strListFeature(vector<string>{"mega", "huge"}));
    map->emplace("item_lda_v1", floatListFeature(vector<float>{0.0f, 0.0f, 0.0f}));
    map->emplace("item_stat", itemActionFeature(vector<Seven<string, int, int, int, int, string, string>>{
            {"product_view",       23, 18, 2, 6, "collection", "2022-09-01"},
            {"impressions_single", 49, 18, 2, 6, "collection", "2022-09-01"},
    }));
    map->emplace("price_min", floatFeature(23.1));
    map->emplace("price_max", floatFeature(24.1f));
    map->emplace("created_at", longFeature(1656471488));
    map->emplace("updated_at", longFeature(1656371488));
    map->emplace("published_at", longFeature(1626671488));
    map->emplace(
            "search_word_exp",
            strIntListFeature(
                    vector<pair<string, int>>{
                            {"word1", 32},
                            {"word2", 24}
                    }
            ));
    map->emplace(
            "search_word_clk",
            strIntListFeature(
                    vector<pair<string, int>>{
                            {"word3", 4},
                            {"word4", 2}
                    }
            ));
    map->emplace(extractor::Item::item_me_cnt, floatListFeature({1245,123,23,12,2}));
}

void mock_user(google::protobuf::Map<string, Feature> *map) {
    //action item
    map->emplace("click_item_series", rTUserActionList(vector<Five<string, long, long, long, int>>{
            Five<string, long, long, long, int>("item1", 11, 22, 33, 1),
            Five<string, long, long, long, int>("item2", 11, 22, 33, 1),
            Five<string, long, long, long, int>("item3", 11, 22, 33, 1),
            Five<string, long, long, long, int>("item4", 11, 22, 33, 1)
    }));
    map->emplace("rt_click_item_series", rTUserActionList(vector<Five<string, long, long, long, int>>{
            Five<string, long, long, long, int>("item1", 44, 44, 44, 2),
            Five<string, long, long, long, int>("item2", 44, 44, 44, 2),
            Five<string, long, long, long, int>("item3", 44, 44, 44, 2)
    }));

    map->emplace("cart_item_series", rTUserActionList(vector<Five<string, long, long, long, int>>{
            Five<string, long, long, long, int>("item2", 11, 22, 33, 1)
    }));
    map->emplace("rt_cart_item_series", rTUserActionList(vector<Five<string, long, long, long, int>>{
            Five<string, long, long, long, int>("item2", 1, 33, 44, 2)
    }));

    map->emplace("purchase_item_series", rTUserActionList(vector<Five<string, long, long, long, int>>{
            Five<string, long, long, long, int>("item3", 11, 22, 33, 1)
    }));
    map->emplace("rt_purchase_item_series", rTUserActionList(vector<Five<string, long, long, long, int>>{
            Five<string, long, long, long, int>("item2", 1, 33, 44, 2)
    }));

    //action tag
    map->emplace("click_url_tag_series", userActionListFeature(vector<Five<string, long, long, long, int>>{
            Five<string, long, long, long, int>("loga", 11, 22, 33, 1)
    }));
    map->emplace("rt_click_url_tag_series", rTUserActionList(vector<Five<string, long, long, long, int>>{
            Five<string, long, long, long, int>("loga", 44, 44, 44, 1)
    }));

//    map->emplace("cart_url_tag_series", userActionListFeature(vector<Five<string, long, long, long, int>>{
//            Five<string, long, long, long, int>("mega", 1656670488, 1663915750, 1663915845, 3)
//    }));
//    map->emplace("rt_cart_url_tag_series", rTUserActionList(vector<Five<string, long, long, long, int>>{
//            Five<string, long, long, long, int>("mega", 1656670488, 1663915750, 1663915845, 1)
//    }));
//
//    map->emplace("purchase_url_tag_series", userActionListFeature(vector<Five<string, long, long, long, int>>{
//            Five<string, long, long, long, int>("yota", 1656670488, 1663915750, 1663915845, 3)
//    }));
//    map->emplace("rt_purchase_url_tag_series", rTUserActionList(vector<Five<string, long, long, long, int>>{
//            Five<string, long, long, long, int>("yota", 1656670488, 1663915750, 1663915845, 1)
//    }));
//
//
//    map->emplace("search_word_series", rTUserActionList(vector<Five<string, long, long, long, int>>{
//            Five<string, long, long, long, int>("optimus", 1656070488, 1663905750, 1663405845, 2)
//    }));
//    map->emplace("rt_search_word_series", rTUserActionList(vector<Five<string, long, long, long, int>>{
//            Five<string, long, long, long, int>("optimus", 1656070488, 1663915750, 1663915845, 1)
//    }));

    map->emplace("user_lda_v1", floatListFeature(vector<float>{0.1f, 0.2f, 0.3f}));
    map->emplace(User::click_series, strListFeature(vector<string>{"2352","akfhahfa","sfakaf"}));
    vector<string> utm_category = {"Mature","Amateur","Fetish","Anal / Extreme","Blowjob / Creampie","Swingers / Group sex","Taboo", "Lesbian","Teen ( 18)"};
//    vector<string> utm_GroupID = {"1","2", "3","6"};
    vector<string> rt_utm_GroupID = {"1","2", "3", "4", "5"};
    vector<string> utm_keyword = {"auntie","trisha","big","tits","compilation","adult","oma","saggy","xxx","omas","Watch","XXX","Omas","tube","sex","video","for","free","xHamster","with","the","sexiest","collection","Free","Xnxc","Ujizz","Free","Adult","porn","movie","scenes","XXX","Omas","Free","Xnxc","Ujizz","Free","Porn","Video","xHamster","xxx","omas","xhjmrHW"};
    vector<pair<string,Feature>> utms = {
            pair(User::utm_category, strListFeature(utm_category)),
//            pair(User::utm_GroupID, strListFeature(utm_GroupID)),
            pair(User::utm_keyword, strListFeature(utm_keyword)),
            pair(User::rt_utm_GroupID, strListFeature(rt_utm_GroupID))
    };
    for(auto x:utms){
        map->emplace(x.first,x.second);
    }
}

void mock(Features &features) {
    google::protobuf::Map<string, Feature> *map = features.mutable_value();
    //context
    mock_context(map);

    //query
    mock_query(map);

    //ctx_item
    mock_ctx_item(map);

    //item
    mock_item(map);

    //user
    mock_user(map);
}