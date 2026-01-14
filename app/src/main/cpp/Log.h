#pragma once
#include <android/log.h>

#ifdef NDEBUG
#define RELEASE_BUILD
#else
#define DEBUG_BUILD
#endif

#define LOG_TAG "Popple"

#ifdef DEBUG_BUILD

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#else

#define LOGV(...)
#define LOGI(...)
#define LOGW(...)
#define LOGE(...)

#endif
