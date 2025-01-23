//
// Created by Bryan Zhou on 2022/10/20.
//

#ifndef EXTRACTOR_CTXITEM_H
#define EXTRACTOR_CTXITEM_H

#include <string>
using namespace std;

namespace CtxItem {
    const string ctx_title_tags = "ctx_title_tags";
    const string ctx_item_id = "ctx_item_id";
    const string ctx_price_max = "ctx_price_max";
    const string ctx_price_min = "ctx_price_min";

    const string ctx_item_lda_v1 = "ctx_item_lda_v1";
    const string ctx_created_at = "ctx_created_at";
    const string ctx_updated_at = "ctx_updated_at";
    const string ctx_published_at = "ctx_published_at";

    const string ctx_price_and_3diff = "ctx_price_and_3diff";
    const string ctx_item_stat = "ctx_item_stat";
    const string ctx_item_ngram = "ctx_item_ngram";
};


#endif //EXTRACTOR_CTXITEM_H
