//
// Created by Bryan Zhou on 2022/10/31.
//
#include "outer.h"

std::map<std::string, Extractor*> instanceMap = std::map<std::string, Extractor*>();

std::threadpool executor=std::threadpool(30);

parallel_node_hash_map<string,long> dateMap = parallel_node_hash_map<string,long>();