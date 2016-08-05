#ifndef __BASIC_LOG_UTILS_H__
#define __BASIC_LOG_UTILS_H__

#ifdef BASIC_TIZEN

#include <dlog.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "TIZEN_SAMPLE"

#ifdef LOGI
#undef LOGI
#endif
#ifdef LOGE
#undef LOGE
#endif
#define LOGI(fmt, arg...) \
    ({ do { \
        dlog_print(DLOG_INFO, LOG_TAG, "%s(%d)>> " fmt " |%s", \
                __func__, __LINE__, ##arg, __FILE__); \
    } while (0); })
#define LOGE(fmt, arg...) \
    ({ do { \
        dlog_print(DLOG_ERROR, LOG_TAG, "%s(%d)>> " fmt " |%s", \
                __func__, __LINE__, ##arg, __FILE__); \
    } while (0); })

#define LOGI_ENTRY LOGI("func entry\n")
#define LOGI_EXIT LOGI("func exit")

#endif //BASIC_TIZEN

#ifdef BASIC_ANDROID
#define  LOG_TAG    "ES20NATIVE"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif //BASIC_ANDROID

#ifdef BASIC_WIN //Win32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define strtok_r strtok_s

#define LOGI(...)					\
	printf("Iml info: ");			\
	printf(__VA_ARGS__);
#define LOGE(...)					\
	printf("Iml ERROR OCCUR! - ");	\
	printf(__VA_ARGS__);

#endif //BASIC_WIN

#define ASSERT(x, s)                                \
    if (!(x)) {                                        \
        LOGE("Assertion failed at %s:%i\n%s\n",        \
                            __FILE__, __LINE__, s); \
        exit(1);                                    \
    }

char *util_strtok(char *str, const char *delim, char **nextp);

void print_gl_string(const char *name, unsigned int s);

void check_gl_error(const char *op);

void log_string(const char *source);

#endif //__BASIC_LOG_UTILS_H__


