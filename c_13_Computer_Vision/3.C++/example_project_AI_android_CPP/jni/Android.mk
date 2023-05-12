
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
# inlcude libopencv-android-armv7a
LOCAL_C_INCLUDES := $(LOCAL_PATH)/libopencv-4.5.0-android-armv7a-prebuilt/sdk/native/jni/include

# include ffmpeg
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libffmpeg/include/armeabi-v7a

# include snpe
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libSNPE-prebuilt/include/zdl

# include paddle-lite
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libpaddle-lite-2.9/include

#include source
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src

LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/AiCore
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/AiCore/snpe
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/AiCore/sort-tracker
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/AiCore/utils
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/AiCore/snpe/detector

LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/AiMajor
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/AiMajor/Anpr
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/AiMajor/Anpr/ocr

LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/AiMajor/Traffic

LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/InputFrame

# share lib
LOCAL_SHARED_LIBRARIES := libopencv_world-prebuilt
LOCAL_SHARED_LIBRARIES += libffmpeg
LOCAL_SHARED_LIBRARIES += libsnpe-prebuilt
LOCAL_SHARED_LIBRARIES += libpaddle-lite-prebuilt

LOCAL_MODULE := devCam
LOCAL_SRC_FILES := \
			\
			src/AiCore/ObjectDetector.cpp \
			src/AiCore/utils/Util.cpp \
			\
			src/AiCore/snpe/SnpeCommLib.cpp \
			src/AiCore/snpe/detector/SnpeDetector.cpp \
			src/AiCore/snpe/detector/SnpeMobilenetSSD.cpp \
			\
			src/AiCore/sort-tracker/Hungarian.cpp \
			src/AiCore/sort-tracker/KalmanTracker.cpp \
			src/AiCore/sort-tracker/ObjectTracker.cpp \
			src/AiCore/sort-tracker/ObjectTracking.cpp \
			src/AiCore/sort-tracker/RoadLine.cpp \
			src/AiCore/sort-tracker/TrackerManager.cpp \
			\
			src/AiMajor/Anpr/ocr/clipper.cpp \
			src/AiMajor/Anpr/ocr/crnn_process.cpp \
			src/AiMajor/Anpr/ocr/db_post_process.cpp \
			src/AiMajor/Anpr/ocr/ocr_db_crnn.cpp \
			\
			src/AiMajor/Anpr/VehicleDetector.cpp \
			src/AiMajor/Anpr/AnprDetector.cpp \
			src/AiMajor/Anpr/AnprRecognizer.cpp \
			\
			src/AiMajor/Traffic/Oppose.cpp \
			src/AiMajor/Traffic/BlowRedLight.cpp \
			src/AiMajor/Traffic/Speed.cpp \
			\
			src/AiMajor/Litter.cpp \
			\
			src/InputFrame/DecodeFrame.cpp \
			src/Main/main_SP.cpp


LOCAL_CFLAGS += -fopenmp -fPIE -fPIC 
LOCAL_CPPFLAGS += -fopenmp -fPIE -fPIC 
LOCAL_LDFLAGS += -fopenmp  -fPIE -fPIC -Wl,--gc-sections 
LOCAL_LDLIBS := -lz -llog -ljnigraphics -landroid -lGLESv2 -lEGL -lvulkan

include $(BUILD_EXECUTABLE)
#include $(BUILD_SHARED_LIBRARY)


#add lib opencv prebuilt
include $(CLEAR_VARS)
LOCAL_MODULE := libopencv_world-prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/libopencv-4.5.0-android-armv7a-prebuilt/sdk/native/libs/armeabi-v7a/libopencv_world.so
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/libopencv-4.5.0-android-armv7a-prebuilt/sdk/native/jni/include
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

#add lib libffmpeg
include $(CLEAR_VARS)
LOCAL_MODULE := libffmpeg
LOCAL_SRC_FILES := $(LOCAL_PATH)/libffmpeg/lib/armeabi-v7a/libffmpeg.so
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/libffmpeg/include/armeabi-v7a
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

#add lib snpe prebuilt
include $(CLEAR_VARS)
LOCAL_MODULE := libsnpe-prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/libSNPE-prebuilt/lib/arm-android-clang6.0/libSNPE.so
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/jni/libSNPE-prebuilt/include/zdl
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)

#add lib paddle-lite prebuilt
include $(CLEAR_VARS)
LOCAL_MODULE := libpaddle-lite-prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/libpaddle-lite-2.9/lib/libpaddle_light_api_shared.so
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/libpaddle-lite-2.9/include
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)
