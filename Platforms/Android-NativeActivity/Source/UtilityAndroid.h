#ifndef __UtilityAndroid_H__
#define __UtilityAndroid_H__

extern android_app* g_pAndroidApp;

double MyGetSystemTime();
double MyGetSystemTimeSinceGameStart();
void OutputMessage(const char* message, ...);
char* LoadCompleteFile(const char* filename, long* length);

#endif //__UtilityAndroid_H__
