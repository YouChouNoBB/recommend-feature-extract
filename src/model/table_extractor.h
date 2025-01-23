//
// Created by Bryan Zhou on 2022/10/19.
//

#ifndef EXTRACTOR_TABLE_EXTRACTOR_H
#define EXTRACTOR_TABLE_EXTRACTOR_H

#include "sample.h"
#include "features.pb.h"
#include <string>
#include <cmath>
#include <iostream>
#include <limits>
#include <ctime>

using namespace std;
using google::protobuf::Map;

class TableExtractor {
public:
    virtual ~TableExtractor(){}

    /**
     * 获取类名，目前测试到是无法在多态场景下获取多态类名，只能获取到基类名，应该用宏定义来实现
     * @tparam T_
     * @param a
     * @return
     */
    template<class T_>
    static std::string className(T_ a){
        //返回的类的全名
        std::string className = std::string(typeid(a).name());
        //第一个空格索引
        int firstSpaceIndex = className.find_first_of(" ") + 1;
        //最后一个空格索引
        int lastSpaceIndex = className.find_last_of(" ");
        //类名长度
        int nameLength = lastSpaceIndex - firstSpaceIndex - 2;
        return className.substr(firstSpaceIndex, nameLength);
    }

    /**
     * 继承的子类实现这个函数，用于对原始数据的抽取
     * @param sample 抽取结果sample
     * @param features rawsample数据
     */
    virtual void extract(Sample *sample, Map<string, Feature> *features){cout<<"table_extractor.h:21"<<endl;}

    /**
     * 继承的子类实现这个函数，用于对sample数据的交叉特征抽取
     * @param sample 抽取结果sample
     * @param user user/query/context特征
     * @param item item特征
     */
    virtual void extract(Sample *sample, Sample *user, Sample *item){}

    /**
     * 对数据规范化，防止输入数据为0或者1
     * @param i
     * @return
     */
    float log(int i) { return (float) std::log(i + 1 + MIN_FLOAT); }

    float log(float i) { return (float) std::log(i + 1 + MIN_FLOAT); }

    float log(double i) { return (float) std::log(i + 1 + MIN_FLOAT); }

    float log(long i) { return (float) std::log(i + 1 + MIN_FLOAT); }

    /**
     * 保留4位小数
     * @param i
     * @return
     */
    float num6(float i){
        int j = i*1000000;
        float k = j/1000000.0;
        return k;
    }

    /**
     * 对字符串hash方法，不一定是高效的，目前不是瓶颈，后续再优化
     * @param s
     * @return int
     */
    int hash(string s) {
        auto t = std::hash<std::string>{}(s);
        return t % INT_MAX;
    }

    /**
     * unix time 统一单位为s,长度10位
     * @param time long
     * @return long
     */
    long formatTime(long time) {
        if (time / TIME_LEN > 1) {
            return time / 1000;
        } else {
            return time;
        }
    }

    /**
     * 校验key是否合法
     * @param map
     * @param key
     * @return
     */
    bool isValidKey(Map<string, Feature> *map, string key) {
        if (map->find(key) == map->end())return false;
        return true;
    }

    /**
     * 当前unix time,单位s
     * @return long
     */
    static long now() {
        return time(NULL);
    }

    /**
     * 当前unix time,单位ms
     * @return long long
     */
    static long long nowMS() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    }

    /**
     * 日期转换成unix time
     * 该函数花了较多精力优化
     * 1.mac和linux执行机制不一样，导致mac执行时间很快，linux上执行很慢，看起来并发不生效
     * 2.对tm结构体设置了部分字段默认值，解决了上述问题
     * 3.频繁调用使得该函数成为了耗时大头
     * 4.为了解决上述问题，引用了并发Map，先查找，没找到再写进去
     * 5.上述方案引发了新的问题，同时读写并发map有几率core
     * 6.为了解决上述问题，对并发map加锁避免了core
     * 7.为了让该函数耗时最小，引入了究极方案，设置全局变量，把当前时间前后1年的日期先全部存到并发map中
     * 8.在程序初始化的时候，初始化这个并发map，经过上述一顿骚操作的优化，该函数终于不再是瓶颈
     * @param s date time
     * @param fs format
     * @return unix time
     */
    static long date2Unix(string s, string fs) ;

    /**
     * 对unix时间转换成date格式,format=%Y-%m-%d %H:%M:%S
     * @param n unix time
     * @return date time
     */
    static string unix2Time(long n) {
        time_t tick = (time_t)n;
        struct tm tmp_time;
        tmp_time = *localtime(&tick);
        char s[20] = {0};
        memset(s, 0, sizeof(s));
        tmp_time.tm_isdst=0;
        tmp_time.tm_gmtoff=0;
        tmp_time.tm_zone="UTC";
        strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tmp_time);
        return std::string(s);
    }

    /**
     * unix time转成明文日期
     * @param n
     * @return
     */
    static string unix2Date(long n){
        return unix2Time(n).substr(0,10);
    }

    /**
     * unix time增加天数，返回明文日期格式数据
     * @param n
     * @param days 天数
     * @return
     */
    static string addDays(long n,int days){
        return unix2Date(n+days*24*60*60);
    }

    /**
     * 日期转unix time
     * @param s
     * @return
     */
    static long date2Unix(string s) {
        //2022-10-21 SHORT_PATTERN 这种传参会导致返回值异常
        return date2Unix(s + " 00:00:00", PATTERN);
    }

    /**
     * unix time diff ，返回单位day
     * @param a
     * @param b
     * @return days
     */
    float diffDay(long a, long b) {
        a = formatTime(a);
        b = formatTime(b);
        return (a - b) / DAY_MILLS;
    }

    /**
     * date diff,date明文格式
     * @param a
     * @param b
     * @return days
     */
    float diffDay(string a, string b) {
        return diffDay(date2Unix(a), date2Unix(b));
    }

    /**
     * 获取星期
     * @param time
     * @return
     */
    long getWeekDay(long time) {
        time = formatTime(time);
        struct tm local;
        local = *gmtime(&time);
        long day = local.tm_wday;
        if (day == 0)day = 7;
        return day;
    }

    /**
     * 月第几天
     * @param time
     * @return
     */
    long getDayMonth(long time) {
        time = formatTime(time);
        struct tm *local = gmtime(&time);
        long day = local->tm_mday;
        return day;
    }

    /**
     * 小时
     * @param time
     * @return
     */
    long getHour(long time) {
        time = formatTime(time);
        struct tm *local = gmtime(&time);
        long hour = local->tm_hour;
        return hour;
    }

    /**
     * 是否周末
     * @param time
     * @return
     */
    bool isWeekend(long time) {
        return getWeekDay(time) > 5;
    }

    /**
     * 时间分片
     * @param hour
     * @return
     */
    long getHourSlice(long hour) {
        if (hour >= 1 && hour <= 7) return 1;
        if (hour >= 8 && hour <= 12) return 2;
        if (hour >= 13 && hour <= 19) return 3;
        return 4;
    }

    /**
     * 一次性计算5个时间相关特征
     * @param time
     * @return
     */
    vector<long> getTimeFeature(long time) {
        time = formatTime(time);
        struct tm local;
        local = *gmtime(&time);
        long day_of_week = local.tm_wday;
        if (day_of_week == 0)day_of_week = 7;
        long day_of_month = local.tm_mday;
        long hour_of_day = local.tm_hour;
        long is_weekend = day_of_week > 5 ? 1 : 0;
        long hour_slice = getHourSlice(hour_of_day);
        return vector<long>{day_of_week, day_of_month, hour_of_day, hour_slice, is_weekend};
    }

    /**
     * repeated UserAction 转成map<item_id,UserAction>
     * @param list
     * @return Map
     */
    Map<string, UserAction> list2map(google::protobuf::RepeatedPtrField<UserAction> list) {
        Map < string, UserAction > map;
        for (auto x: list) {
            map[x.item_id()] = x;
        }
        return map;
    }

    /**
     * 字符串去除首尾空格
     * @param s
     * @return
     */
    static std::string &trim(std::string &s) {
        if (s.empty())return s;
        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
        return s;
    }

    /**
     * 字符串转小写
     * @param s
     */
    void tolower(string &s){
        transform(s.begin(),s.end(),s.begin(),::tolower);
    }

    /**
     * 字符串分割
     * @param s
     * @param delimiter char
     * @param list 分割结果
     */
    void split(string &s, char delimiter, vector<string> &list) {
        int lastIndex = 0;
        for (int index = 0; index < s.length(); index++) {
            char c = s[index];
            if (c == delimiter) {
                string t = s.substr(lastIndex, index - lastIndex);
                if (!trim(t).empty())list.emplace_back(t);
                lastIndex = index + 1; //because if it were index, it would include the operator
            }
        }
        string t = s.substr(lastIndex);
        if (!trim(t).empty())list.emplace_back(t);
    }

    void split(string &s, char delimiter1,char delimiter2, vector<string> &list) {
        int lastIndex = 0;
        for (int index = 0; index < s.length(); index++) {
            char c = s[index];
            if (c == delimiter1 || c == delimiter2) {
                string t = s.substr(lastIndex, index - lastIndex);
                if (!trim(t).empty())list.emplace_back(t);
                lastIndex = index + 1; //because if it were index, it would include the operator
            }
        }
        string t = s.substr(lastIndex);
        if (!trim(t).empty())list.emplace_back(t);
    }

    /**
     * 使用字符分割
     * @param str
     * @param split char
     * @param res
     */
    void stringSplit(const string &str, const char split, vector<string> &res) {
        if (str == "") return;
        //在字符串末尾也加入分隔符，方便截取最后一段
        string strs = str + split;
        size_t pos = strs.find(split);

        // 若找不到内容则字符串搜索函数返回 npos
        while (pos != strs.npos) {
            string temp = strs.substr(0, pos);
            res.push_back(temp);
            //去掉已分割的字符串,在剩下的字符串中进行分割
            strs = strs.substr(pos + 1, strs.size());
            pos = strs.find(split);
        }
    }

    /**
     * 使用字符串分割
     * @param str
     * @param splits string
     * @param res
     */
    void stringSplit(const string &str, const string &splits, vector<string> &res) {
        if (str == "") return;
        //在字符串末尾也加入分隔符，方便截取最后一段
        string strs = str + splits;
        size_t pos = strs.find(splits);
        int step = splits.size();

        // 若找不到内容则字符串搜索函数返回 npos
        while (pos != strs.npos) {
            string temp = strs.substr(0, pos);
            res.push_back(temp);
            //去掉已分割的字符串,在剩下的字符串中进行分割
            strs = strs.substr(pos + step, strs.size());
            pos = strs.find(splits);
        }
    }

    /**
     * pb list转vector
     * @tparam t class type
     * @param pb RepeatedField
     * @param list vector<t>
     */
    template<class t>
    static void pbVector2Vector(google::protobuf::RepeatedField<t> pb,vector<t> &list){
        for(t x:pb){
            list.template emplace_back(x);
        }
    }

    void addLabel(Sample *sample, const Feature *feature);
    void addLabel(Sample *sample, Map<string, Feature> *features,string key);

    void addLong(Sample *sample, string name, const Feature *feature, int hash_size);
    void addLong(Sample *sample,string name,Map<string, Feature> *features,string key,int hash_size);

    void addString(Sample *sample, string name, const Feature *feature);
    void addString(Sample *sample, string name, Map<string, Feature> *features,string key);

    void addFloat(Sample *sample, string name, const Feature *feature);
    void addFloat(Sample *sample, string name, Map<string, Feature> *features,string key);

    void addLongList(Sample *sample, string name, const Feature *feature, int max_hash, int padding_size);
    void addLongList(Sample *sample, string name, Map<string, Feature> *features,string key, int max_hash, int padding_size);

    void addLongList(Sample *sample, string name, const Feature *feature, int max_hash);
    void addLongList(Sample *sample, string name, Map<string, Feature> *features,string key, int max_hash);

    void addFloatList(Sample *sample, string name, const Feature *feature);
    void addFloatList(Sample *sample, string name, Map<string, Feature> *features,string key);

    void addFloatList(Sample *sample, string name, const vector<float> &list);

    /**
     * 用户tag行为特征抽取，包含实时和离线数据的处理
     * @param sample
     * @param features
     * @param key
     * @param now 样本时间
     * @param hash_size
     */
    void extractAction(Sample *sample, Map<string, Feature> *features, string key, long now, int hash_size);

    /**
     * 合并实时和离线特征
     * @param features
     * @param key
     */
    void mergeRTAction(Map<string, Feature> *features, string key);

    Feature merge(const Feature *offline, const Feature *online);

    UserAction merge(const UserAction *offline, const Item *online);

    UserAction rt2off(const Item *item);

    void map2vec(unordered_map<long,int> map,vector<pair<long,float>> *vec);

    /**
     * 获取ngram
     * @param query
     * @param n N
     * @param hash_size
     * @param list N-gram结果
     */
    void getNGram(string &query, int n, int hash_size, vector<pair<long, float>> &list);

    vector<long> padding_list(vector<long> &list, int padding_size);

    vector<pair<long, float>> padding_weight(vector<pair<long, float>> &list, int padding_size);


    constexpr static const double DAY_MILLS = 86400.0;
    static string PATTERN;
    static string SHORT_PATTERN;
    constexpr const static long TIME_LEN = 10000000000L;
    constexpr const static float MIN_FLOAT = 1e-6f;
};
#endif //EXTRACTOR_TABLE_EXTRACTOR_H
