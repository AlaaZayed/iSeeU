LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#OPENCV_CAMERA_MODULES:=off
#OPENCV_INSTALL_MODULES:=off
#OPENCV_LIB_TYPE:=SHARED
include D:/CSED/vourth_year/Graduation_project/opencv-Binaries/OpenCV-2.4.2-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES  := MicroExpressionRecog_jni.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -llog -ldl

LOCAL_MODULE     := micro_expression_detection

include $(BUILD_SHARED_LIBRARY)