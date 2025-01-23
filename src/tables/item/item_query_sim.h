//
// Created by Bryan Zhou on 2022/10/26.
//

#ifndef EXTRACTOR_ITEM_QUERY_SIM_H
#define EXTRACTOR_ITEM_QUERY_SIM_H
#include "table_extractor.h"
#include "label.h"
#include "item.h"
#include "query.h"
#include "cross.h"
#include "similarity.h"

using namespace std;
using namespace extractor;
class ItemQuerySim:public TableExtractor{
public:
    ItemQuerySim(){}
    ~ItemQuerySim(){}

    /**
     * item title_tags与query相似度
     * 编辑距离，匹配编辑距离，匹配距离，LDA相似度
     * query_tag与title_tags相似
     * @param sample
     * @param features
     */
    void extract(Sample *sample, Map<string, Feature> *features){
        string query;
        vector<float> list(5,MIN_FLOAT);
        if(features->contains(Label::query)){
            query = features->find(Label::query)->second.str_value();
            transform(query.begin(),query.end(),query.begin(),::tolower);
            trim(query);
        }else{
            query="";
        }
        //query 为空，所有都不处理
        if(query.empty()){
            sample->addFloatList(Cross::item_query_sim, list);
            return;
        }

        //商品带标题的才处理
        if(features->contains(extractor::Item::title_tags)){
            vector<string> title_tag_list;
            string title_tags;
            for(auto x:features->find(extractor::Item::title_tags)->second.str_list().value()){
                title_tag_list.emplace_back(x);
                title_tags += (" "+x);
            }
            vector<string> query_tag_list ;
            split(query, ' ',query_tag_list);
            list[0] = Similarity::editSim(query, title_tags);
            list[1] = Similarity::matchEditSim(query_tag_list, title_tag_list);
            list[2] = Similarity::matchSim(query_tag_list, title_tag_list);

            if(features->contains(Query::query_click_tag)){
                vector<StrInt> query_str_int_list;
                for(auto x:features->find(Query::query_click_tag)->second.str_int_list().value()){
                    query_str_int_list.emplace_back(x);
                }
                vector<float> tagsim2 = Similarity::tagSim(query_str_int_list,title_tag_list);
                list[3] = tagsim2[0];
                list[4] = tagsim2[1];
            }
        }

        sample->addFloatList(Cross::item_query_sim, list);
    }
};
#endif //EXTRACTOR_ITEM_QUERY_SIM_H
