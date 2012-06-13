#include <android/log.h>

#define  LOG_TAG    "AndroidOpenGL"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

/*
//TODO find out how to forward vargs
class Logger {
public:
	void static LOGI(const char *,...);
	void static LOGE(const char *,...);
};
*/
