//
// Created by Bryan Zhou on 2022/10/24.
//

#ifndef EXTRACTOR_COLLECTION_V12_H
#define EXTRACTOR_COLLECTION_V12_H

#include "table_extractor.h"
#include "extractor.h"
class CollectionRcmdExtractorV12 : public Extractor{
public:
    CollectionRcmdExtractorV12();
    ~CollectionRcmdExtractorV12();
//    vector<TableExtractor> userFeatureTables;
//    vector<TableExtractor> itemFeatureTables;
//    vector<TableExtractor> crossFeatureTables;
    void fun();
};
#endif //EXTRACTOR_COLLECTION_V12_H
