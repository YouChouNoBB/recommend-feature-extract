#include <iostream>
#include "sample.h"
#include "features.pb.h"
#include "tfexample.pb.h"
#include "tfrecord.h"
#include "labelcontext.h"
#include "collection_v10.h"
#include "collection_v12.h"
#include "jsonutil.h"
#include "five.h"
#include "user_lda.h"
#include <fstream>
#include "mock.h"
#include "phmap.h"
#include <thread>
#include <sys/select.h>
#include "phmap.h"
#include "outer/outer.h"
#include "similarity.h"
#include "search_v2.h"
#include "search_v3.h"
#include "so.h"
#include "thread_pool.h"
#include "outer.h"
#include <ctime>
#include "user_action_series.h"
#include "positive_v1.h"
#include "rank_gauss.h"
#include "positive_add_v1.h"
#include <string>

using namespace std;
using phmap::flat_hash_map;
using phmap::parallel_flat_hash_map;

long long SysMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}

void test_file(string file) {
    ifstream inFile(file, ios::in | ios::binary); //二进制读方式打开
    if (!inFile) {
        cout << "error" << endl;
        return;
    }

    Extractor *e = instanceMap["PositiveAddV1"];
    int i = 0;
    int a;
    int sum = 0;
    BatchFeatures batchFeatures;

    do {
        byte *data = new byte[8];
        inFile.read((char *) data, 8);
        long data_size;
        memcpy(&data_size, data, 8);
        data = new byte[4];
        inFile.read((char *) data, 4);
        data = new byte[data_size];
        inFile.read((char *) data, data_size);

        Features features;
        features.ParseFromArray(data, data_size);
        batchFeatures.mutable_value()->Add(std::move(features));
        inFile.read((char *) &a, 4);
        i++;
        delete[]data;
        if (i % 500 == 0)break;
    } while (a);
    cout << batchFeatures.value_size() << endl;
    string s;
    proto_to_string(batchFeatures.value(batchFeatures.value_size() - 1), &s);
    cout << s << endl;

    byte *data = new byte[batchFeatures.ByteSizeLong()];
    batchFeatures.SerializeToArray(data, batchFeatures.ByteSizeLong());
    auto start = SysMs();
    e->extractBatch(data, batchFeatures.ByteSizeLong());
    auto end = SysMs();
    cout << end - start << endl;
}

void test_reflect() {
    string class_name = "CollectionRcmdExtractorV10";
    Extractor *extra = instanceMap[class_name];
    if (extra == nullptr)cout << "instance is null" << endl;
    Features features;
    mock(features);
    Example example = extra->extractor(&features);
    string s;
    proto_to_string(example, &s);
    cout << s << endl;
}

static void sleep_ms(unsigned int secs) {
    struct timeval tval;
    tval.tv_sec = secs / 1000;
    tval.tv_usec = (secs * 1000) % 1000000;
    select(0, NULL, NULL, NULL, &tval);
}

void test_rank() {
    vector<int> list(1280000);
    for (int i = 0; i < list.size(); i++) {
        list[i] = rand();
    }
    auto fn = [list](int i) {
        auto t = vector<int>(list);
        auto start = SysMs();
        sort(t.begin(), t.end());
        cout << SysMs() - start << endl;
    };
    for (int i = 0; i < 20; i++) {
        executor.commit(fn, 0);
    }

}

void test_concurent() {

    auto fn = [](int i) {
        auto start = SysMs();
        Features features;
        mock(features);
        instanceMap["CollectionRcmdExtractorV10"]->extractor(&features);
        cout << SysMs() - start << endl;
    };
    for (int i = 0; i < 20; i++) {
        executor.commit(fn, 0);
    }
}


void test_thread() {
    int num_threads = getenv("CXX_THREAD") == NULL ? 8 : atoi(getenv("CXX_THREAD"));
    std::unique_ptr<std::thread> threads[num_threads];
    int size = 500;
    parallel_flat_hash_map<int, Example> map(size);

    Extractor *ex = new CollectionRcmdExtractorV10();
    Features features;
    mock(features);

    auto thread_fn = [size, num_threads, &map, &features, ex](int thread_idx) {
        for (int i = 0; i < size; i++) {
            if (i % num_threads == thread_idx) {

                auto start = SysMs();
                ex->extractor(&features);
//                map[i]=std::move(ex->extractor(features));
                cout << SysMs() - start << endl;
//                map[i]=example;
            }
        }
    };

    auto start = SysMs();
    for (int i = 0; i < num_threads; ++i)
        threads[i].reset(new std::thread(thread_fn, i));

    for (int i = 0; i < num_threads; ++i)
        threads[i]->join();

    cout << SysMs() - start << endl;
}


void test_extract() {
    Extractor *extra = new CollectionRcmdExtractorV10();

    for (int i = 0; i < 500; i++) {
        Features features;
        mock(features);
        Example example = extra->extractor(&features);
    }

    delete extra;
}

void test_bytes_extract() {
    Extractor *extra = new CollectionRcmdExtractorV10();
    Features features;
    mock(features);
    int input_size = features.ByteSizeLong();

    byte *input = new byte[input_size];
    features.SerializeToArray(input, input_size);
    byte *output = extra->extract(input, input_size);
    int out_size;
    memcpy(&out_size, output, 4);
    byte *res = new byte[out_size];
    memcpy(res, output + 4, out_size);

    cout << out_size << endl;

//    Example example;
//    example.ParseFromArray(res,out_size);
//    string s;
//    proto_to_string(example,&s);
//    cout<<s<<endl;
    delete[] input;
    delete[] output;
    delete[]res;
//    delete features;
    delete extra;
}

void test_batch_extract() {
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

    auto start = SysMs();
    byte *output = extra->extractBatch(input, input_size);
    cout << SysMs() - start << endl;


//    int out_size;
//    memcpy(&out_size, output, 4);
//    cout << out_size << endl;

    //找最后一个起始位置
//    int start_index = (out_size + 1) * 4;
//    int size_byte = 0;
//    for (int i = 0; i < out_size; i++) {
//        memcpy(&size_byte, output + 4 + (i) * 4, 4);
//        start_index += size_byte;
//    }
//    cout<<size_byte<<endl; //最后一个sample的size
//    byte *last_data = new byte[size_byte];
//    memcpy(last_data, output + start_index - size_byte, size_byte);
//
//    Example example;
//    example.ParseFromArray(last_data, size_byte);
//    string s;
//    proto_to_string(example, &s);
//    cout << s << endl;

//    byte *res = new byte[out_size];
//    memcpy(res, output + 4, out_size);
//    BatchSamples batchSamples ;
//    batchSamples.ParseFromArray(res, out_size);


//    string s;
//    proto_to_string(batchSamples.value(499),&s);
//    cout<<s<<endl;
    delete[]input;
    delete[] output;
//    delete[]res;

}

void test_pb_json() {
    Extractor *extra = new SearchV3();
    Features features;
    mock(features);
    string input;
    features.SerializeToString(&input);

    Features *new_input = new Features();
    new_input->ParseFromString(input);

    auto start = SysMs();
    Example example = extra->extractor(new_input);
    cout << SysMs() - start << endl;

    vector<string> list;
    for (auto x: example.features().feature()) {
        if (x.first.find("index") != std::string::npos)
            list.emplace_back(x.first);
    }
    sort(list.begin(), list.end());
    for (auto x: list)
        cout << x << endl;

    std::string json_string;
    proto_to_string(example, &json_string);

    ofstream ofs;
    ofs.open("example.txt", ios::out);
    ofs << json_string;
    ofs.close();

    delete extra;
}

void test_filter_vector() {
    vector<int> a{1, 2, 3, 4, 5, 3, 3};
    cout << a.size() << endl;
    a.erase(std::remove_if(a.begin(), a.end(), [](int i) {
//        if(i==3)
//        {
//            return true;
//        }
        return true;
    }), a.end());
    cout << a.empty() << endl;
    cout << a.size() << endl;
}

void test_string_transform() {
    string query = " afa af afWEFGa 23@#@ fAFGS af A ";
    cout << query << endl;
    transform(query.begin(), query.end(), query.begin(), ::tolower);
    cout << query << endl;
    cout << TableExtractor::trim(query) << endl;
    cout << query << endl;
    // output
//     afa af afWEFGa 23@#@ fAFGS af A
//     afa af afwefga 23@#@ fafgs af a
//    afa af afwefga 23@#@ fafgs af a
}

void test_vector_constractor() {
    vector<float> list(2, 1e-6);
    cout << list.capacity() << endl;
    cout << list.size() << endl;
    for (auto x: list)
        cout << x << endl;
    //output
//    2
//    2
//    1e-6
//    1e-6
}

void test_string() {
    string s;
    cout << s.empty() << endl;
    s = "";
    cout << s.empty() << endl;
    s = "   ";
    cout << s.empty() << endl;
    TableExtractor::trim(s);
    cout << s.empty() << endl;
    //output
//    1
//    1
//    0
//    1
}

void test_class_name() {
    TableExtractor *a = new LabelContext(1);
    cout << TableExtractor::className(a) << endl;
    a = new UserLDA();
    cout << TableExtractor::className(a) << endl;
    // failed !
}

void test_vecsim() {
    vector<float> vec1 = {0.1, 0.0, 0.0};
    vector<float> vec2 = {0.0, 0.0, 0.0};
    cout << Similarity::vecSim(vec1, vec2) << endl;
    vector<pair<long, float>> v1 = {{4, 0.1},
                                    {3, 0.2},
                                    {5, 0.3}};
    vector<pair<long, float>> v2 = {{1, 0.1},
                                    {2, 0.2},
                                    {6, 0.4}};
    cout << Similarity::vecSim(v1, v2) << endl;
}

//void test_outer(){
//    string class_name = "CollectionRcmdExtractorV10";
//    long ins = getInstance(class_name);
//    Features features;
//    mock(features);
//    int size = features.ByteSizeLong();
//    byte* data = new byte[size];
//    features.SerializeToArray(data,size);
//    byte* res = extract(ins,data,size);
//    Example example;
//    memcpy(&size,res,4);
//    example.ParseFromArray(res+4,size);
//    string s;
//    proto_to_string(example,&s);
//    cout<<s<<endl;
//}

void test_time() {
    struct tm tmp_time;
    string s = "2022-11-10 00:00:00";
    string fs = TableExtractor::PATTERN;
    strptime(s.c_str(), fs.c_str(), &tmp_time);
    auto start = SysMs();
    time_t t = mktime(&tmp_time);
    cout << SysMs() - start << endl;
    cout << tmp_time.tm_isdst << endl;
    cout << tmp_time.tm_gmtoff << endl;
    cout << tmp_time.tm_zone << endl;
    cout << t << endl;

    struct tm tmp_time2;
    tmp_time2.tm_isdst = 0;
    tmp_time2.tm_gmtoff = 0;
    tmp_time2.tm_zone = "UTC";
    strptime(s.c_str(), fs.c_str(), &tmp_time2);
    start = SysMs();
    t = mktime(&tmp_time2);
    cout << SysMs() - start << endl;
    cout << t << endl;
}

void test_store_45053_extract() {
    char *class_name = "CollectionRcmdExtractorV10";
    long ins = getInstance(class_name);

    // Reading size of file
    // s3://mlalgorithm/test/rawsamples_bytes
    FILE *file = fopen("./rawsamples_bytes", "rb");
    if (file == NULL) return;
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    cout << size << endl;

    // Reading data to array of unsigned chars
    file = fopen("./rawsamples_bytes", "rb");
    unsigned char *in = (unsigned char *) malloc(size);
    int bytes_read = fread(in, sizeof(unsigned char), size, file);
    fclose(file);

    auto start = SysMs();
    auto res = extractBatch(ins, in, size);

    cout << SysMs() - start << endl;
//    free(in);
    release(res);

//    int size_out ;
//    memcpy(&size_out,res,4);
//    cout<<size_out<<endl;
//    memcpy(&size_out,res+4,4);
//    cout<<size_out<<endl;
}

void test_time_trans() {
    long now = TableExtractor::now();
    cout << now << endl;
    cout << TableExtractor::unix2Time(now) << endl;
    cout << TableExtractor::unix2Date(now) << endl;
    string s2 = TableExtractor::unix2Date(now + 70 * 24 * 60 * 60);
    cout << s2 << endl;
    cout << TableExtractor::date2Unix("2022-11-15") << endl;
    for (auto x: dateMap) {
        cout << x.first << ":" << x.second << endl;
    }
    cout << dateMap.size() << endl;
}

void test_merge_user_action() {
    UserActionSeries user(User::click_item_series, 50, 10000000);
    Features features;
    mock(features);
    string s;
    proto_to_string(features.value().find("click_item_series")->second, &s);
    cout << s << endl;

    user.mergeActionSeries(features.mutable_value(), "click_item_series");

    proto_to_string(features.value().find("click_item_series")->second, &s);
    cout << s << endl;

    proto_to_string(features.value().find("rt_click_item_series")->second, &s);
    cout << s << endl;
}

void test_merge_series() {
    UserActionSeries user(User::click_item_series, 50, 10000000);
    Features features;
    mock(features);
    string s;
    Feature offline = features.value().find("click_item_series")->second;
    Feature online = features.value().find("rt_click_item_series")->second;
    proto_to_string(offline, &s);
    cout << s << endl;
    user.mergeSeries(&offline, &online);
    proto_to_string(offline, &s);
    cout << s << endl;
}

void test_merge_item() {
    Item offline;
    Item online;
    offline.set_item_id("1");
    offline.set_collection_id("");
    offline.set_first_time(1234);
    offline.set_last_second_time(1238);
    offline.set_last_time(2233);
    offline.set_cnt(2);

    online.set_item_id("1");
    online.set_collection_id("collection2");
    online.set_first_time(12);
    online.set_last_second_time(12381);
    online.set_last_time(22333);
    online.set_cnt(4);

    UserActionSeries user(User::click_item_series, 50, 10000000);
    user.mergeItem(&offline, &online);
    string s;
    proto_to_string(offline, &s);
    cout << s << endl;
}

void test_merge_tag_series(){
    TableExtractor extractor;
    string  key = "click_url_tag_series";
    Features features;
    mock(features);
    string s;
    proto_to_string(features.value().find(key)->second, &s);
    cout<<s<<endl;

    proto_to_string(features.value().find("rt_"+key)->second, &s);
    cout<<s<<endl;

    extractor.mergeRTAction(features.mutable_value(),key);
    proto_to_string(features.value().find(key)->second, &s);
    cout<<s<<endl;

}

void test_recall_positive() {
    Extractor *extra = new PositiveRecallV1();
    Features features;
    mock(features);
    string s;

    auto start = SysMs();
    Example example = extra->extractor(&features);
    cout << SysMs() - start << endl;
    proto_to_string(example, &s);
    cout << s << endl;
}

void test_recall_batchFeatures() {
    Extractor *extra = new CollectionRcmdExtractorV12();
    FILE *file = fopen("./batchFeatures", "rb");

    long size;
    fread(&size, sizeof(unsigned char), 8, file);
    int crc;
    fread(&crc, sizeof(unsigned char), 4, file);
    cout << crc << endl;
    cout << size << endl;
    unsigned char *in = (unsigned char *) malloc(size);

    int bytes_read = fread(in, sizeof(unsigned char), size, file);
    fclose(file);
    cout << bytes_read << endl;

//    BatchFeatures batchFeatures;
//    batchFeatures.ParseFromArray(in, size);
//    string s_b;
//    proto_to_string(batchFeatures.value(0),&s_b);
//    cout<<s_b<<endl;

    auto start = SysMs();
    byte *output = extra->extractBatch(in, size);
    cout << SysMs() - start << endl;
    int out_size;
    memcpy(&out_size, output, 4);
    cout << out_size << endl;

//    找最后一个起始位置
    int start_index = (out_size + 1) * 4;
    int size_byte = 0;
    for (int i = 0; i < out_size; i++) {
        memcpy(&size_byte, output + 4 + (i) * 4, 4);
        start_index += size_byte;
    }
    cout << size_byte << endl; //最后一个sample的size
    byte *last_data = new byte[size_byte];
    memcpy(last_data, output + start_index - size_byte, size_byte);

    Example example;

    example.ParseFromArray(last_data, size_byte);
    vector<string> list;
    for (auto it: example.features().feature()) {
        if (it.first.ends_with("_index")) {
            int index = it.first.find("_index", 0);
            string key = it.first;
            key.replace(index, 6, "");
            list.emplace_back(key);
        }
    }
    std::sort(list.begin(), list.end());
    for (auto i: list) {
        cout << i << endl;
    }

    string s;
    proto_to_string(example, &s);
//    cout << s << endl;

    ofstream ofs;
    ofs.open("example.txt", ios::out);
    ofs << s;
    ofs.close();
}

void test_rank_gauss() {
    vector<float> data{-19.9378, 10.5341, -32.4515, 33.0969, 24.3530, -1.1830, -1.4106, -4.9431,
                       14.2153, 26.3700, -7.6760, 60.3346, 36.2992, -126.8806, 14.2488, -5.0821,
                       1.6958, -21.2168, -49.1075, -8.3084, -1.5748, 3.7900, -2.1561, 4.0756,
                       -9.0289, -13.9533, -9.8466, 79.5876, -13.3332, -111.9568, -24.2531, 120.1174};

    map<float, float> m;  // this stores the rank transformation
    vector<float> dataOut;  // output data (rankGauss transformed)
    buildRankGaussTrafo(data, m);
    applyRankTrafo(data, m, dataOut);
    cout << "input: data(size:" << data.size() << ")" << endl;
    for (int i = 0; i < data.size(); i++)
        cout << data[i] << endl;
    cout << endl << "output: dataOut(size:" << dataOut.size() << ")" << endl;
    for (int i = 0; i < dataOut.size(); i++)
        cout << dataOut[i] << endl;
}

void test_positive_add() {
    Extractor *extra = instanceMap["PositiveSearchV1"];
//    Extractor *extra = new PositiveAddV1();
    Features features;
    mock(features);
    string s;

    auto start = SysMs();
    Example example = extra->extractor(&features);
    cout << SysMs() - start << endl;
    proto_to_string(example, &s);
    cout << s << endl;
    vector<string> keys;
    for (auto kv: example.features().feature()) {
        if (kv.first.ends_with("_index")) {
            string key = kv.first;
            keys.emplace_back(key.replace(kv.first.find("_index"), 6, ""));
        }
    }
    std::sort(keys.begin(), keys.end());
    for (auto key: keys) { cout << key << endl; }

}

void test_file_class(string file, string class_name) {
    ifstream inFile(file, ios::in | ios::binary);
    if (!inFile) {
        cout << "error" << endl;
        return;
    }

//    Extractor *e = new PositiveAddV1();
    Extractor *e = instanceMap[class_name];
    byte *data = new byte[8];
    inFile.read((char *) data, 8);
    long data_size;
    memcpy(&data_size, data, 8);
    data = new byte[4];
    inFile.read((char *) data, 4);
    data = new byte[data_size];
    inFile.read((char *) data, data_size);
    Features features;
    features.ParseFromArray(data, data_size);
    string s;
    proto_to_string(features, &s);
//    cout<<s<<endl;

    Example example = e->extractor(&features);
    proto_to_string(example, &s);
//    cout<<s<<endl;

    vector<string> keys;
    for (auto kv: example.features().feature()) {
        if (kv.first.ends_with("_index")) {
            string key = kv.first;
            keys.emplace_back(key.replace(kv.first.find("_index"), 6, ""));
        }
    }
    std::sort(keys.begin(), keys.end());
    for (auto key: keys) { cout << key << endl; }
}

void test_reflect_extract(){
    Extractor *extra = instanceMap["SearchV26"];
//    Extractor *extra = new CollectionRcmdExtractorV23();
    Features features;
    mock(features);
    string s;

    auto start = SysMs();
    Example example;
    for(int i=0;i<100;i++){
        example = extra->extractor(&features);
    }

    cout << SysMs() - start << endl;
    proto_to_string(example, &s);
    cout << s << endl;
//    vector<string> keys;
//    for (auto kv: example.features().feature()) {
//        if (kv.first.ends_with("_index")) {
//            string key = kv.first;
//            keys.emplace_back(key.replace(kv.first.find("_index"), 6, ""));
//        }
//    }
//    std::sort(keys.begin(), keys.end());
//    for (auto key: keys) { cout << key << endl; }
}

int main(int argc, char *argv[]) {
//    test_time();
//    test_concurent();
//    test_rank();
//    string s = "CollectionRcmdExtractorV10";
//    test(s.data());
//    test("SearchV2");
//    test_outer();
//    test_vecsim();
//    cout<<argv[1]<<endl;
//    testFile(argv[1]);
//    test_reflect();
//    test_rank();
//    test_thread();
//    for(int i=0;i<10;i++)
//        test_batch_extract();
//    for(int i=0;i<500;i++)
//        test_extract();
//        test_bytes_extract();
//    test_class_name();
//    test_pb_json();
//    test_filter_vector();
//    test_string_transform();
//    test_vector_constractor();
//    test_string();
//    while(1)
//        test_store_45053_extract();
//    test_file("./part-r-000000");
//    test_time_trans();
//    test_merge_user_action();
//    test_merge_item();
//    test_recall_positive();
//    test_recall_batchFeatures();
//    test_rank_gauss();
    test_reflect_extract();
//    test_file_class("./part-r-000000", "PositiveAddV1");
//    test_merge_user_action();
//    test_merge_tag_series();
//    TableExtractor t;
//    cout<<argv[1]<<endl;
//    cout<<t.hash(argv[1]) % 500000 + 1<<endl;

    return 0;
}


