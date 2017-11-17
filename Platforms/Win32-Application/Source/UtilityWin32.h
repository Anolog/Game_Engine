#ifndef __UtilityWin32_H__
#define __UtilityWin32_H__

double MyGetSystemTime();
double MyGetSystemTimeSinceGameStart();
void OutputMessage(const char* message, ...);
char* LoadCompleteFile(const char* filename, long* length);

#endif //__UtilityWin32_H__
