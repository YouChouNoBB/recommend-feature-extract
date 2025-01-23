//
// Created by Bryan Zhou on 2022/10/19.
//
#include "extractor.h"
#include "jsonutil.h"
#include "phmap.h"
#include <thread>
#include <cstdlib>
#include <pthread.h>
#include "outer.h"
#include "mock.h"
//#include <omp.h>

using namespace std;
using phmap::flat_hash_map;
using phmap::parallel_flat_hash_map;

// todo 并发
// https://github.com/greg7mdp/parallel-hashmap
// openmp

Sample *Extractor::extractUser(Map<string, Feature> *map) {
    Sample *sample = new Sample();
    for (auto table: this->userFeatureTables) {
        table->extract(sample, map);
    }
    return sample;
}

Sample *Extractor::extractItem(Map<string, Feature> *map) {
    Sample *sample = new Sample();
    for (auto table: itemFeatureTables) {
        table->extract(sample, map);
    }
    return sample;
}

Sample *Extractor::extractCross(Sample *user, Sample *item) {
    Sample *sample = new Sample();
    for (auto table: crossFeatureTables) {
        table->extract(sample, user, item);
    }
    return sample;
}

int Extractor::num_threads = getenv("CXX_THREAD") == NULL ? 8 : atoi(getenv("CXX_THREAD"));

void Extractor::genDateMap(){
    if(dateMap.size()!=0)return;
    long now = TableExtractor::now();
    for(int i = 0;i<=360;i++){
        string d1 = TableExtractor::addDays(now,i);
        string d2 = TableExtractor::addDays(now,-i);
        dateMap[d1+" 00:00:00"] = TableExtractor::date2Unix(d1+" 00:00:00","%Y-%m-%d %H:%M:%S");
        dateMap[d2+" 00:00:00"] = TableExtractor::date2Unix(d2+" 00:00:00","%Y-%m-%d %H:%M:%S");
    }
}

struct Res{
    int size;
    byte* data;
};

//线程池调用
byte *Extractor::extractBatch(void *input, int size) {
    BatchFeatures batchFeatures;
    batchFeatures.ParseFromArray(input, size); //解析数据
    int length = batchFeatures.value_size();

    parallel_flat_hash_map<int, byte *> map(length); //存每行数据
    parallel_flat_hash_map<int, int> map_size(length);//存每行数据size

    auto thread_fn = [this](Features *features,int i) {
        Example example = extractor(features);
        int example_size = example.ByteSizeLong();
        byte *data = new byte[example_size];   //构造自定义数据的时候释放
        example.SerializeToArray(data, example_size);
        return Res{example_size,data};
    };

    vector<future<Res>> thread_out;
    for (int i = 0; i < length; ++i)
        thread_out.emplace_back(executor.commit(thread_fn,batchFeatures.mutable_value(i),i));

    int bytes_sum = 0; //数据总大小
    for (int i = 0; i < length; ++i){
        Res res = thread_out[i].get();
        map[i]=res.data;
        map_size[i]=res.size;
        bytes_sum += res.size;
    }

    byte *all = new byte[(length + 1) * 4 + bytes_sum]; //分配最终返回的数据空间
    memcpy(all, &length, 4); //sample数量

    int size_index = 4;            //size起始位置
    int data_index = (length + 1) * 4; //data起始位置

    for (int i = 0; i < length; i++) {          //构造自定义数据，包含sample大小和sample
        int example_size = map_size[i]; //size
        byte *data = map[i];            //data
        memcpy(all + size_index, &example_size, 4);
        memcpy(all + data_index, data, example_size);
        size_index += 4;
        data_index += example_size;
        delete[] data;             //释放前面new 出来的数据
    }

    return all;  //调用后需要释放
}

//调用openmp
//byte* Extractor::extractBatch(void *input,int size){
//    BatchFeatures batchFeatures ;
//    batchFeatures.ParseFromArray(input,size); //解析数据
//
//    int length = batchFeatures.value_size();
//    std::unique_ptr<std::thread> threads[num_threads]; //并发调用
//    parallel_flat_hash_map<int,byte*> map(length); //存每行数据
//    parallel_flat_hash_map<int,int> map_size(length);//存每行数据size
//
//    #pragma omp parallel for
//    for(int i=0;i<length;i++){
//        Example example = extractor(batchFeatures.mutable_value(i));
//        int example_size = example.ByteSizeLong();
//        byte* data=new byte[example_size];   //构造自定义数据的时候释放
//        example.SerializeToArray(data,example_size);
//        map[i] = data;
//        map_size[i] = example_size;
//    }
//
//    int bytes_sum=0; //数据总大小
//    for(auto x:map_size){
//        bytes_sum += x.second;
//    }
//
//    byte* all = new byte[(length+1)*4+bytes_sum]; //分配最终返回的数据空间
//    memcpy(all,&length,4); //sample数量
//
//    int size_index = 4;            //size起始位置
//    int data_index = (length+1)*4; //data起始位置
//
//    for(int i=0;i<length;i++){          //构造自定义数据，包含sample大小和sample
//        int example_size = map_size[i]; //size
//        byte* data = map[i];            //data
//        memcpy(all+size_index,&example_size,4);
//        memcpy(all+data_index,data,example_size);
//        size_index+=4;
//        data_index+=example_size;
//        delete [] data;             //释放前面new 出来的数据
//    }
//
//    return all;  //调用后需要释放
//}

//go在线调用接口,前4字节是长度,返回自定义协议数据
//调用thread
//byte* Extractor::extractBatch(void *input,int size){
//    BatchFeatures batchFeatures ;
//    batchFeatures.ParseFromArray(input,size); //解析数据
//
//    int length = batchFeatures.value_size();
//    std::unique_ptr<std::thread> threads[num_threads]; //并发调用
//    parallel_flat_hash_map<int,byte*> map(length); //存每行数据
//    parallel_flat_hash_map<int,int> map_size(length);//存每行数据size
//
//    auto thread_fn = [&map,&batchFeatures,&map_size,length,this](int thread_idx){
//        for(int i=0;i<length;i++){
//            if(i % num_threads == thread_idx){
//                Example example = extractor(batchFeatures.mutable_value(i));
//                int example_size = example.ByteSizeLong();
//                byte* data=new byte[example_size];   //构造自定义数据的时候释放
//                example.SerializeToArray(data,example_size);
//                map[i] = data;
//                map_size[i] = example_size;
//            }
//        }
//    };
//
//    for (int i=0; i<num_threads; ++i)
//        threads[i].reset(new std::thread(thread_fn, i));
//
//    for (int i=0; i<num_threads; ++i)
//        threads[i]->join();
//
//    int bytes_sum=0; //数据总大小
//    for(auto x:map_size){
//        bytes_sum += x.second;
//    }
//
//    byte* all = new byte[(length+1)*4+bytes_sum]; //分配最终返回的数据空间
//    memcpy(all,&length,4); //sample数量
//
//    int size_index = 4;            //size起始位置
//    int data_index = (length+1)*4; //data起始位置
//
//    for(int i=0;i<length;i++){          //构造自定义数据，包含sample大小和sample
//        int example_size = map_size[i]; //size
//        byte* data = map[i];            //data
//        memcpy(all+size_index,&example_size,4);
//        memcpy(all+data_index,data,example_size);
//        size_index+=4;
//        data_index+=example_size;
//        delete [] data;             //释放前面new 出来的数据
//    }
//
//    return all;  //调用后需要释放
//}

////go在线调用接口,前4字节是长度，返回batchSamples
//byte* Extractor::extractBatch(void *input,int size){
//    BatchFeatures batchFeatures ;
//    batchFeatures.ParseFromArray(input,size);
//
//    BatchSamples batchSamples ;
//    auto  bs= batchSamples.mutable_value();
//
//    //todo 并发调用
//    std::unique_ptr<std::thread> threads[num_threads];
//    parallel_flat_hash_map<int,Example> map(batchFeatures.value_size());
//
//    auto thread_fn = [&map,&batchFeatures,this](int thread_idx){
//        for(int i=0;i<batchFeatures.value_size();i++){
//            if(i % num_threads == thread_idx){
//                Example example = extractor(batchFeatures.mutable_value(i));
//                map[i] = std::move(example);
//            }
//        }
//    };
//
//    for (int i=0; i<num_threads; ++i)
//        threads[i].reset(new std::thread(thread_fn, i));
//
//    for (int i=0; i<num_threads; ++i)
//        threads[i]->join();
//
//    for(int i =0;i<batchFeatures.value_size();i++){
//        bs->Add(std::move(map[i]));
//    }
//
//    int byte_size = batchSamples.ByteSizeLong();
//    byte* output = new byte[byte_size];
//    batchSamples.SerializeToArray(output,byte_size);
//
//    byte* ret = new byte[byte_size+4];
//    memcpy(ret,&byte_size,4);
//    memcpy(ret+4,output,byte_size);
//    delete []output;
//    return ret;
//}

typedef struct {
    Extractor *ex;
    parallel_flat_hash_map<int, Example> *map;
    BatchFeatures *batchFeatures;
    int thread_idx;
    int num_threads;
} MY_ARGS;

void *thread_fn(void *args) {
    MY_ARGS *my_args = (MY_ARGS *) args;

    for (int i = 0; i < my_args->batchFeatures->value_size(); i++) {
        if (i % my_args->num_threads == my_args->thread_idx) {
            Example example = my_args->ex->extractor(my_args->batchFeatures->mutable_value(i));
            (*my_args->map)[i] = std::move(example);
        }
    }
    return NULL;
}

//pthread
//byte *Extractor::extractBatch(void *input, int size) {
//    BatchFeatures *batchFeatures = new BatchFeatures();
//    batchFeatures->ParseFromArray(input, size);
//    BatchSamples batchSamples;
//    auto bs = batchSamples.mutable_value();
//
//    //todo 并发调用
//    pthread_t threads[num_threads];
//    parallel_flat_hash_map<int, Example> map(batchFeatures->value_size());
//
//    MY_ARGS args[num_threads];
//    for (int i = 0; i < num_threads; ++i) {
//        args[i] = {this,&map, batchFeatures, i, num_threads};
//    }
//
//    for (int i = 0; i < num_threads; ++i) {
//        pthread_create(&threads[i], NULL, thread_fn, &args[i]);
//    }
//
//    for (int i = 0; i < num_threads; ++i)
//        pthread_join(threads[i], NULL);
//
//    for (int i = 0; i < batchFeatures->value_size(); i++) {
//        bs->Add(std::move(map[i]));
//    }
//
//    int byte_size = batchSamples.ByteSizeLong();
//    byte *output = new byte[byte_size];
//    batchSamples.SerializeToArray(output, byte_size);
//
//    byte *ret = new byte[byte_size + 4];
//    memcpy(ret, &byte_size, 4);
//    memcpy(ret + 4, output, byte_size);
//    delete[]output;
//    return ret;
//}

//离线调用接口,前4字节是长度
byte *Extractor::extract(void *input, int size) {
    Features features;
    features.ParseFromArray(input, size);
    Example example = extractor(&features);
    int byte_size = example.ByteSizeLong();
    byte *output = new byte[byte_size];
    example.SerializeToArray(output, byte_size);
    byte *ret = new byte[byte_size + 4];
    memcpy(ret, &byte_size, 4);
    memcpy(ret + 4, output, byte_size);
    delete[]output;
    return ret;
}


Example Extractor::extractor(Features *features) {
    auto map = features->value();
    Sample *user = extractUser(&map);
    Sample *item = extractItem(&map);
    Sample *cross = extractCross(user, item);
    Example res = Sample::merge(vector<Sample *>{user, item, cross});
    delete user;
    delete item;
    delete cross;

    return res;
}



void Extractor::print() {
    cout << this->userFeatureTables.size() << endl;
}
//vector<TableExtractor> Extractor::userFeatureTables = vector<TableExtractor>();
//vector<TableExtractor> Extractor::itemFeatureTables = vector<TableExtractor>();
//vector<TableExtractor> Extractor::crossFeatureTables = vector<TableExtractor>();