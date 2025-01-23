//
// Created by Bryan Zhou on 2022/10/26.
//

#ifndef EXTRACTOR_MOCK_H
#define EXTRACTOR_MOCK_H
#include "features.pb.h"
#include "five.h"
#include "seven.h"
using namespace std;

/**
 * mock数据，基础类型数据转换成Feature类型数据
 */
Feature strFeature(string s);
Feature intFeature(int i);
Feature floatFeature(float i);
Feature longFeature(long i);
Feature strListFeature(vector<string> list);
Feature strListFeature(set<string> list);
Feature floatListFeature(vector<float> list);
Feature strIntListFeature(vector<pair<string,int>> list);
Feature userActionListFeature(vector<Five<string,long,long,long,int>> list);
Feature rTUserActionList(vector<Five<string,long,long,long,int>> list);
Feature itemActionFeature(vector<Seven<string,int,int,int,int,string,string>> list);
void mock(Features &features);
#endif //EXTRACTOR_MOCK_H
