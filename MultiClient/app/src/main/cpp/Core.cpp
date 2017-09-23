// AudioRender.cpp : Defines the entry point for the console application.
//


#include "App.h"
#include "com_oseasy_mmc_multiclient_CORE.h"
#include "logger.h"

//net thread running
JNIEXPORT void JNICALL Java_com_oseasy_mmc_multiclient_Core_Start
        (JNIEnv * env, jobject obj)
{
    App app;
    jclass jni = env->GetObjectClass(obj);
    app.Java().getConfig = env->GetMethodID(jni,"GetConfig","(Ljava/lang/String;)Ljava/lang/String;");
    app.Java().startFunc = env->GetMethodID(jni,"StartFunc","(Ljava/lang/String;Ljava/lang/String;II)V");
    app.Java().stopFunc = env->GetMethodID(jni,"StopFunc","(Ljava/lang/String;)V");
    app.Java().set = env->GetMethodID(jni,"Set","(Ljava/lang/String;Z)V");
    app.Java().obj = obj;
    app.Java().env = env;
    app.Run();
}
