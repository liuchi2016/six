// AudioRender.cpp : Defines the entry point for the console application.
//


#include "App.h"
#include "com_oseasy_mmc_multiclient_MultiClient.h"

//net thread running
JNIEXPORT void JNICALL Java_com_oseasy_mmc_multiclient_MultiClient_Start
        (JNIEnv * env, jobject obj,jstring teacherIp,jstring studentIp,jstring mac)
{
    std::string  _teacherIp = jstringTostr(env,teacherIp);

    std::string  _studentIp = jstringTostr(env,studentIp);

    std::string  _mac = jstringTostr(env,mac);

    App app(_teacherIp,_studentIp,_mac);

    app.Run();
}
