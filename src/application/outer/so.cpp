//
// Created by Bryan Zhou on 2022/11/8.
//
#include "so.h"
#include "outer.h"
#include "mock.h"
#include "jsonutil.h"
#include "collection_v10.h"
#include "table_extractor.h"
//获取实例对象
long getInstance(char* className){
    string s(className);
    assert(instanceMap.contains(s));
    Extractor *e = instanceMap[s];
    return long(e);
}

long getInstance(string className){
    assert(instanceMap.contains(className));
    Extractor *e = instanceMap[className];
    return long(e);
}

//单行数据抽取
byte* extract(long ins,byte* data,int size){
    Extractor *e = reinterpret_cast<Extractor*>(ins);
    void* vdata = reinterpret_cast<void*>(data);
    return e->extract(vdata,size);
}

unsigned char* extract(long ins,unsigned char* data,int size){
    Extractor *e = reinterpret_cast<Extractor*>(ins);
    void* vdata = reinterpret_cast<void*>(data);
    byte* res = e->extract(vdata,size);
    unsigned char* output = reinterpret_cast<unsigned char*>(res);
    return output;
}

//batch数据抽取
byte* extractBatch(long ins,byte* data,int size){
    Extractor *e = reinterpret_cast<Extractor*>(ins);
    void* vdata = reinterpret_cast<void*>(data);
    return e->extractBatch(vdata,size);
}

unsigned char* extractBatch(long ins,unsigned char* data,int size){
    Extractor *e = reinterpret_cast<Extractor*>(ins);
    void* vdata = reinterpret_cast<void*>(data);
    byte* res = e->extractBatch(vdata,size);
    unsigned char* output = reinterpret_cast<unsigned char*>(res);
    release(data);
    return output;
}

//释放数据
void release(byte* data){
    delete []data;
}

void release(unsigned char* data){
    delete []data;
}

//mock 数据,前4字节是长度
byte* mockByte(){
    Features features;
    mock(features);
    int size = features.ByteSizeLong();
    byte* out = new byte[size+4];
    memcpy(out,&size,4);
    features.SerializeToArray(out+4,size);
    return out;
}
unsigned char* mockChar(){
    byte* data = mockByte();
    unsigned char* out = reinterpret_cast<unsigned char*>(data);
    return out;
}

void test(char* s){
    long ins = getInstance(s);
    byte* data = mockByte();
    int size;
    memcpy(&size,data,4);
    byte *res = extract(ins,data+4,size);
    memcpy(&size,res,4);
    Example example;
    example.ParseFromArray(res+4,size);
    string str;
    proto_to_string(example,&str);
    cout<<str<<endl;
}


int benchmark(){
    Extractor *extra = instanceMap["CollectionRcmdExtractorV10"];
    BatchFeatures batchFeatures;
    auto v = batchFeatures.mutable_value();
    for (int i = 0; i < 500; i++) {
        Features features;
        mock(features);
        v->Add(std::move(features));
    }

    int input_size = batchFeatures.ByteSizeLong();

    byte *input = new byte[input_size];
    batchFeatures.SerializeToArray(input, input_size);

    auto start = TableExtractor::nowMS();
    byte *output = extra->extractBatch(input, input_size);
    auto end = TableExtractor::nowMS();
    cout << end - start << endl;

    delete[]input;
    delete[] output;
    return end - start;
}
