//
// Created by Bryan Zhou on 2022/10/31.
//
#include "com_shoplazza_algorithm_feature_extractorpb_model_JNIExtractor.h"
#include "outer.h"

/**
 * 获取cpp实例对象
 * @param env jni上下文
 * @param obj 对象
 * @param class_name 类名
 * @return 指针
 */
JNIEXPORT jlong JNICALL Java_com_shoplazza_algorithm_feature_extractorpb_model_JNIExtractor_genInstance
        (JNIEnv * env, jobject obj, jstring class_name){
    const char* name = env->GetStringUTFChars(class_name,0);
    Extractor *e = instanceMap[string(name)];
    return jlong(e);
}

/**
 * 特征抽取接口
 * @param env
 * @param obj
 * @param instance 对象指针
 * @param data raw sample 字节数组
 * @param size 字节数组长度
 * @return 抽取结果，字节数组
 */
JNIEXPORT jbyteArray JNICALL Java_com_shoplazza_algorithm_feature_extractorpb_model_JNIExtractor_extract
        (JNIEnv * env, jobject obj, jlong instance, jbyteArray data, jint size){
    if(env->GetArrayLength(data)!=size){
        cout<<"size not equal";
        size = env->GetArrayLength(data);
    }
    jbyte * olddata = (jbyte*)env->GetByteArrayElements(data, 0);
    byte* bytearr = (byte*)olddata;
    void* cdata = reinterpret_cast<void*>(bytearr);
    Extractor *e = reinterpret_cast<Extractor*>(instance);
    byte* output = e->extract(cdata,size);
    int out_size;
    memcpy(&out_size,output,4);
    byte* res = new byte[out_size];
    memcpy(res,output+4,out_size);
    jbyte *ret = (jbyte*)res;
    jbyteArray jarray = env->NewByteArray(out_size);
    env->SetByteArrayRegion(jarray, 0, out_size, ret);
    delete []output;
    if(res) delete[] res;
    if(bytearr) delete[] bytearr;
    return jarray;
}

/**
 * batch 抽取
 * @param env
 * @param obj
 * @param instance 对象指针
 * @param data raw sample 字节数组
 * @param size 字节数组长度
 * @return sample
 */
JNIEXPORT jbyteArray JNICALL Java_com_shoplazza_algorithm_feature_extractorpb_model_JNIExtractor_extractBatch
        (JNIEnv * env, jobject obj, jlong instance, jbyteArray data, jint size){
    if(env->GetArrayLength(data)!=size){
        cout<<"size not equal";
        size = env->GetArrayLength(data);
    }
    jbyte * olddata = (jbyte*)env->GetByteArrayElements(data, 0);
    byte* bytearr = (byte*)olddata;
    void* cdata = reinterpret_cast<void*>(bytearr);
    Extractor *e = reinterpret_cast<Extractor*>(instance);
    byte* output = e->extractBatch(cdata,size);
    int out_size;
    memcpy(&out_size,output,4);
    byte* res = new byte[out_size];
    memcpy(res,output+4,out_size);
    jbyte *ret = (jbyte*)res;
    jbyteArray jarray = env->NewByteArray(out_size);
    env->SetByteArrayRegion(jarray, 0, out_size, ret);
    delete []output;
    if(res) delete[] res;
    if(bytearr) delete[] bytearr;
    return jarray;
}

/**
 * 删除抽取对象,目前抽取对象是全局的，所以不能调用这个方法
 * @param env
 * @param obj
 * @param instance 对象指针
 */
JNIEXPORT void JNICALL Java_com_shoplazza_algorithm_feature_extractorpb_model_JNIExtractor_delete
        (JNIEnv * env, jobject obj, jlong instance){
    Extractor *e = reinterpret_cast<Extractor*>(instance);
    if(e)
        delete e;
//    google::protobuf::ShutdownProtobufLibrary();
}

/**
 * 释放抽取结果数据
 * @param env
 * @param obj
 * @param data sample
 */
JNIEXPORT void JNICALL Java_com_shoplazza_algorithm_feature_extractorpb_model_JNIExtractor_release
        (JNIEnv * env, jobject obj, jbyteArray data){
    jbyte * olddata = (jbyte*)env->GetByteArrayElements(data, 0);
    env->ReleaseByteArrayElements(data,olddata,0);
}