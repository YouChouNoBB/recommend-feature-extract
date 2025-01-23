//
// Created by Bryan Zhou on 2022/10/31.
//

#ifndef EXTRACTOR_OUTER_H
#define EXTRACTOR_OUTER_H
#include "stdlib.h"
#include "string"
#include "extractor.h"
#include "thread_pool.h"
#include "phmap.h"

using phmap::parallel_node_hash_map;
using namespace std;

extern std::map<std::string, Extractor*> instanceMap;
extern std::threadpool executor;

typedef phmap::parallel_flat_hash_map<string, long, phmap::priv::hash_default_hash<string>,
        phmap::priv::hash_default_eq<string>,
        std::allocator<std::pair<const string, long>>, 4, std::shared_mutex>
        MapT;

extern parallel_node_hash_map<string,long> dateMap;

#define REGISTRY_CLASS(class_name) \
class registry_##class_name { \
public: \
registry_##class_name() { \
    instanceMap[#class_name] = new class_name(); \
} \
}; registry_##class_name registry_##class_name##inst;


#endif //EXTRACTOR_OUTER_H
