//
// Created by Bryan Zhou on 2022/10/31.
//

/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_algorithm_feature_extractorpb_model_JNIExtractor */

#ifndef _Included_com_algorithm_feature_extractorpb_model_JNIExtractor
#define _Included_com_algorithm_feature_extractorpb_model_JNIExtractor
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_algorithm_feature_extractorpb_model_JNIExtractor
 * Method:    genInstance
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_algorithm_feature_extractorpb_model_JNIExtractor_genInstance
        (JNIEnv *, jobject, jstring);

/*
 * Class:     com_algorithm_feature_extractorpb_model_JNIExtractor
 * Method:    extract
 * Signature: (J[BI)[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_algorithm_feature_extractorpb_model_JNIExtractor_extract
        (JNIEnv *, jobject, jlong, jbyteArray, jint);


/*
 * Class:     com_algorithm_feature_extractorpb_model_JNIExtractor
 * Method:    extract
 * Signature: (J[BI)[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_algorithm_feature_extractorpb_model_JNIExtractor_extractBatch
        (JNIEnv *, jobject, jlong, jbyteArray, jint);

/*
 * Class:     com_algorithm_feature_extractorpb_model_JNIExtractor
 * Method:    delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_algorithm_feature_extractorpb_model_JNIExtractor_delete
(JNIEnv *, jobject, jlong);


/*
 * Class:     com_algorithm_feature_extractorpb_model_JNIExtractor
 * Method:    release
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_algorithm_feature_extractorpb_model_JNIExtractor_release
        (JNIEnv * , jobject , jbyteArray );

#ifdef __cplusplus
}
#endif
#endif
