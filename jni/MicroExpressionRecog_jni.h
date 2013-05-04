#include <jni.h>
#include <vector>
/* Header for class com_eaaa_iseeu_MicroExpRecogNative */

#ifndef _Included_com_eaaa_iseeu_MicroExpRecogNative
#define _Included_com_eaaa_iseeu_MicroExpRecogNative
#ifdef __cplusplus
extern "C" {
#endif

using namespace std;
/*
 * Class:     com_eaaa_iseeu_MicroExpRecogNative
 * Method:    nativeCreateObject
 */JNIEXPORT jint JNICALL Java_com_eaaa_iseeu_MicroExpRecogNative_recognizeExpression(
		JNIEnv *, jclass, jlong);

JNIEXPORT void JNICALL Java_com_eaaa_iseeu_MicroExpRecogNative_train(
		JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
