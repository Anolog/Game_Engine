#include "pch.h"
#include "UtilityAndroid.h"

android_app* g_pAndroidApp = 0;

double MyGetSystemTime()
{
    timespec time;
    
    //if( realtime )
    //    clock_gettime(CLOCK_REALTIME, &time);
    //else
        clock_gettime( CLOCK_MONOTONIC, &time );

    return time.tv_sec + ((double)time.tv_nsec / 1000000000);
}

double MyGetSystemTimeSinceGameStart()
{
	static double starttime = -1;

	if( starttime == -1 )
		starttime = MyGetSystemTime();

	return MyGetSystemTime() - starttime;
}

void OutputMessage(const char* message, ...)
{
#define MAX_MESSAGE 1024
    char buffer[MAX_MESSAGE];
    va_list arg;
    va_start( arg, message );
    vsnprintf( buffer, sizeof(buffer), message, arg );
    va_end(arg);

    buffer[MAX_MESSAGE-1] = 0;

    __android_log_print( ANDROID_LOG_INFO, "MyTemplate.NativeActivity", "%s", buffer );
}

char* LoadCompleteFile(const char* filename, long* length)
{
    AAssetManager* pAssetManager = g_pAndroidApp->activity->assetManager;

    char* filecontents = 0;

    char assetsfilename[255];
    sprintf( assetsfilename, "assets/%s", filename );

    AAsset* asset = AAssetManager_open( pAssetManager, filename, AASSET_MODE_STREAMING );
    if( asset == 0 )
        return 0;

    if( asset )
    {
        int size = AAsset_seek( asset, 0, SEEK_END );
        AAsset_seek( asset, 0, SEEK_SET );

        filecontents = new char[size+1];
        AAsset_read( asset, filecontents, size );
        filecontents[size] = 0;

        if( length )
            *length = size;

        AAsset_close( asset );
    }

    return filecontents;
}
