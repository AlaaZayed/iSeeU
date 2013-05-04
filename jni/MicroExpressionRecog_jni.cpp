#include "MicroExpressionRecog_jni.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/detection_based_tracker.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv/cxcore.h>
#include <opencv/cv.h>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
//#include<iostream>

#include <android/log.h>

#define LOG_TAG "iSeeU"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

using namespace std;
using namespace cv;



JNIEXPORT jint JNICALL Java_com_eaaa_iseeu_MicroExpRecogNative_recognizeExpression(
		JNIEnv * jenv, jclass, jlong image) {
	LOGD("Starting expression recognition");
	jint result = 0;

	LOGD("Ending expression recognition");
	return result;
}

JNIEXPORT void JNICALL Java_com_eaaa_iseeu_MicroExpRecogNative_train(
		JNIEnv * jenv, jclass) {
	LOGD("Starting loading training");


	LOGD("Ending loading training");
}

