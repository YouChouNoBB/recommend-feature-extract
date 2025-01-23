//
// Created by Bryan Zhou on 2022/11/8.
//

#ifndef EXTRACTOR_SO_H
#define EXTRACTOR_SO_H
//#include <string>
//using namespace std;

#ifdef __cplusplus
extern "C"{
#endif
//获取实例对象
long getInstance(char* s);

//单行数据抽取
//byte* extract(long ins,byte* data,int size);
unsigned char* extract(long ins,unsigned char* data,int size);

//batch数据抽取
//byte* extractBatch(long ins,byte* data,int size);
unsigned char* extractBatch(long ins,unsigned char* data,int size);

//释放数据
//void release(byte* data);
void release(unsigned char* data);

//mock数据
//byte* mockByte();
unsigned char* mockChar();

//test
void test(char* s);
//void test(string s);

//benchmark
int benchmark();
#ifdef __cplusplus
};
#endif
#endif //EXTRACTOR_SO_H
