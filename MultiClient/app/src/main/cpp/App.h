#ifndef _APP_H_2016_10_09_LC_
#define _APP_H_2016_10_09_LC_

#include <jni.h>
#include "tcpclient.h"
#include "DataLogic.h"

inline  char* jstringTostr(JNIEnv* env,jstring jstr)
{
	char * pStr = 0;

	jclass jstrObj = env->FindClass("java/lang/String");
	jstring encode = env->NewStringUTF("utf-8");
	jmethodID methodId = env->GetMethodID(jstrObj,"getBytes","(Ljava/lang/String;)[B");
	jbyteArray byteArray=(jbyteArray)env->CallObjectMethod(jstr,methodId,encode);
	jsize strLen  =env->GetArrayLength(byteArray);
	jbyte *jBuf = env->GetByteArrayElements(byteArray,JNI_FALSE);
	if (jBuf){
		pStr = (char*)malloc(strLen+1);
		if (!pStr){
			return 0;
		}
		memcpy(pStr,jBuf,strLen);
		pStr[strLen]=0;
	}
	env->ReleaseByteArrayElements(byteArray,jBuf,0);
    env->DeleteLocalRef(encode);
    env->DeleteLocalRef(byteArray);
    env->DeleteLocalRef(jstrObj);
	return pStr;
}

class  CallerFromJava{

public:

    static  void Stop(std::string name) {
        char  intent[1024] = {0};
        sprintf(intent,"su -c \"am startservice -n com.oseasy.mmc.multiclient/.MultiClient -a com.oseasy.mmc.multiclient.STOPFUNC --es type %s\"",name.c_str());
        system(intent);
    }
    static void Start(std::string name,std::string ip,int port, int verityPort){
        char  intent[1024] = {0};
        sprintf(intent,"su -c \"am startservice -n com.oseasy.mmc.multiclient/.MultiClient -a com.oseasy.mmc.multiclient.OPENFUNC --es type %s --es host %s --ei port %d --ei verityPort %d\"",name.c_str(),ip.c_str(),port,verityPort);
        system(intent);
    }
    static void Set(std::string name, bool value) {
        char  intent[1024] = {0};
        sprintf(intent,"su -c \"am startservice -n com.oseasy.mmc.multiclient/.MultiClient -a com.oseasy.mmc.multiclient.SETFUNC --es type %s --ei value %d\"",name.c_str(),(int)value);
        system(intent);
    }

    static void Log(std::string message){
        char  intent[1024] = {0};
        sprintf(intent,"su -c \"am startservice -n com.oseasy.mmc.multiclient/.MultiClient -a com.oseasy.mmc.multiclient.LOG --es message %s\"",message.c_str());
        system(intent);
    }
};

class App
{
private:
	tcpclient		client;
	DataLogic		logic;
	CallerFromJava  java;
	std::string 	_teachetIp;
	std::string 	_studentIp;
	std::string 	_smac;
public:
	App(std::string teacherIp,std::string studentIp,std::string smac);
	~App();
	DataLogic&  Logic();
	tcpclient&  GetTcpClient();
	CallerFromJava& Java();
	void Run();
};
#endif
