//
// Created by Bryan Zhou on 2022/10/20.
//

#ifndef EXTRACTOR_ITEM_H
#define EXTRACTOR_ITEM_H

#include <string>

using namespace std;
namespace extractor {
    namespace Item {
        const string product_title = "product_title";
        const string title_tags = "title_tags";
        const string title_tags_3gram = "title_tags_3gram";
        const string tags = "tags";
        const string tags_3gram = "tags_3gram";
        const string item_id = "item_id";
        const string price_max = "price_max";
        const string price_min = "price_min";
        const string collections = "collections";


        const string item_lda_v1 = "item_lda_v1";
        const string created_at = "created_at";
        const string updated_at = "updated_at";
        const string published_at = "published_at";

        const string price_and_3diff = "price_and_3diff";
        const string item_stat = "item_stat";
        const string item_ngram = "item_ngram";

        const string cate1 = "cate1";
        const string cate2 = "cate2";
        const string cate3 = "cate3";

        const string search_word_exp = "search_word_exp";
        const string search_word_clk = "search_word_clk";

        const string item_me_cnt = "item_me_cnt";
    };

}


#endif //EXTRACTOR_ITEM_H
