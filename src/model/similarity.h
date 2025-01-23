//
// Created by Bryan Zhou on 2022/10/26.
//

#ifndef EXTRACTOR_SIMILARITY_H
#define EXTRACTOR_SIMILARITY_H
#include <vector>
#include "vector.h"
#include "features.pb.h"
#include "table_extractor.h"
#include "edit_distance.h"
#include <set>

using namespace std;
class Similarity{
public:
    constexpr static float SMALL_FLOAT = 1e-6f;
    //纯单词匹配数量
    static float matchSim(vector<string> query, vector<string> title_tags){
        if (query.size() == 0 || title_tags.size() == 0) return SMALL_FLOAT;
        float count = 0;
        for (string s : query) {
            if (s.empty()) continue;
            for (string t : title_tags) {
                if (t.empty()) continue;
                if (s==t) count++;
            }
        }
        float sim = count / title_tags.size();
        if (sim == 0.0)
            sim = SMALL_FLOAT;
        return sim;
    }


    //纯文本编辑距离
    static float editSim(string s1, string s2){
        if (TableExtractor::trim(s1).empty() || TableExtractor::trim(s2).empty()) return SMALL_FLOAT;
        return EditDistance::getsimilarity(s1, s2);
    }


    /**
     * 匹配编辑距离
     *
     * @param query      正常query,中间可能包含空格
     * @param title_tags 从特征中获取的title_tags,逗号分割
     * @return
     */
    static float matchEditSim(vector<string> query, vector<string> title_tags){
        if (query.size() == 0) return SMALL_FLOAT;
        float sim_sum = SMALL_FLOAT;
        for (string s : query) {
            if (s.empty()) continue;
            float max_sim = 0;
            for (string t : title_tags) {
                if (t.empty()) continue;
                float sim = editSim(s, t);
                if (sim > max_sim) max_sim = sim;
            }
            sim_sum += max_sim;
        }
        return sim_sum / query.size();
    }


    //向量余弦距离
    static float vecSim(Vector vec1, Vector vec2){
        return vec1.cosSim(vec2);
    }


    static float vecSim(vector<float> vec1, vector<float> vec2){
        return Vector(vec1).cosSim(Vector(vec2));
    }

    static float vecSim(vector<pair<long,float>> vec1,vector<pair<long,float>> vec2){
        unordered_map<long,float>map1(vec1.begin(),vec1.end());
        unordered_map<long,float>map2(vec2.begin(),vec2.end());
        double product=0;
        double norm1=0,norm2=0;
        for(auto x:map1){
            long key=x.first;
            float value=x.second;
            norm1 += value*value;
            if(map2.contains(key)){
                product+= value*map2[key];
            }
        }
        if(product* norm1 ==0) return 1e-6f;

        for(auto x:map2){
            float value=x.second;
            norm2 += value*value;
        }
        if(norm2==0) return 1e-6f;

        return product/(sqrt(norm1)* sqrt(norm2));
    }

    //query 和item的tag相似
    static vector<float> tagSim(vector<StrInt> query_tags, vector<string> item_tag){
        vector<float> ret{SMALL_FLOAT, SMALL_FLOAT};
        if (query_tags.empty() || item_tag.empty()) {
            return ret;
        }
        set<string> item_tags ;
        for(auto x:item_tag){
            transform(x.begin(),x.end(),x.begin(),::tolower);
            TableExtractor::trim(x);
            item_tags.insert(x);
        }

        double sum = 0;
        double tag_sum = 0;
        float position_sim = 0;
        float i = 1;
        for (StrInt query_tag : query_tags) {
            string tag = query_tag.key();
            transform(tag.begin(),tag.end(),tag.begin(),::tolower);
            TableExtractor::trim(tag);
            int cnt = query_tag.value();
            if (item_tags.contains(tag)) {
                tag_sum += cnt;
                position_sim += 1.0f / (i + 1);
            }
            sum += cnt;
            i++;
        }
        float tag_sim = (float) (sum == 0 ? 0 : tag_sum / sum);
        ret[0] = (tag_sim == 0.0 ? SMALL_FLOAT : tag_sim);
        ret[1] = (position_sim == 0.0 ? SMALL_FLOAT : position_sim);
        return ret;
    }


};
#endif //EXTRACTOR_SIMILARITY_H
