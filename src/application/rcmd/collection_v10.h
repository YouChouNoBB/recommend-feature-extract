//
// Created by Bryan Zhou on 2022/10/24.
//

#ifndef EXTRACTOR_COLLECTION_V10_H
#define EXTRACTOR_COLLECTION_V10_H

#include "table_extractor.h"
#include "extractor.h"
class CollectionRcmdExtractorV10 : public Extractor{
public:
    CollectionRcmdExtractorV10();
    ~CollectionRcmdExtractorV10();
//    vector<TableExtractor> userFeatureTables;
//    vector<TableExtractor> itemFeatureTables;
//    vector<TableExtractor> crossFeatureTables;
    void fun();
};
#endif //EXTRACTOR_COLLECTION_V10_H
