#ifndef _APP_H_2016_10_09_LC_
#define _APP_H_2016_10_09_LC_

#include <jni.h>
#include "tcpclient.h"
#include "DataLogic.h"
#include "logger.h"


inline  char* jstringTostr(JNIEnv* env,jstring jstr)
{
	char * pStr = 0;

	jclass jstrObj = env->FindClass("java/lang/String");
	jstring encode = env->NewStringUTF("utf-8");
	jmethodID methodId = env->GetMethodID(jstrObj,"getBytes","(Ljava/lang/String;)[B");
	jbyteArray byteArray=(jbyteArray)env->CallObjectMethod(jstr,methodId,encode);
	jsize strLen  =env->GetArrayLength(byteArray);
	jbyte *jBuf = env->GetByteArrayElements(byteArray,JNI_FALSE);
	if (!jBuf){
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

inline  jstring strTojstring(JNIEnv* env, const char* str){
	jclass strClass = env->FindClass("java/lang/String");
	jmethodID methodID = env->GetMethodID(strClass,"<init>","([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(strlen(str));
	env->SetByteArrayRegion(bytes,0,strlen(str),(jbyte*)str);
	jstring encoding = env->NewStringUTF("utf-8");
	jstring result = (jstring)env->NewObject(strClass,methodID,bytes,encoding);
    env->DeleteLocalRef(encoding);
    env->DeleteLocalRef(bytes);
    env->DeleteLocalRef(strClass);
    return  result;
}



class  CallerFromJava{
public:
	jmethodID getConfig;
	jmethodID stopFunc;
	jmethodID startFunc;
	jmethodID set;
	JNIEnv*   env;
	jobject   obj;
public:
	void Stop(std::string name) {
		jstring j_name = strTojstring(env,name.c_str());
		env->CallVoidMethod(obj,stopFunc,j_name);
        env->DeleteLocalRef(j_name);
	}
	void Start(std::string name,std::string ip,int port, int verityPort) {
		jstring j_name = strTojstring(env,name.c_str());
		jstring j_ip = strTojstring(env,ip.c_str());
		env->CallVoidMethod(obj,startFunc,j_name,j_ip,port,verityPort);
        env->DeleteLocalRef(j_name);
        env->DeleteLocalRef(j_ip);
	}
	void Set(std::string name, bool value) {
		jstring j_name = strTojstring(env,name.c_str());
		env->CallVoidMethod(obj,set,j_name,value);
        env->DeleteLocalRef(j_name);
	}

	std::string    GetConfig(std::string type){
		jstring j_type = strTojstring(env,type.c_str());
		jstring result = (jstring)env->CallObjectMethod(obj,getConfig,j_type);
		std::string value = jstringTostr(env,result);
        env->DeleteLocalRef(j_type);
        env->DeleteLocalRef(result);
        return  value;
	}
};

class App
{
private:
	tcpclient		client;
	DataLogic		logic;
	CallerFromJava  java;
public:
	App();
	~App();
	DataLogic&  Logic();
	tcpclient&  GetTcpClient();
	CallerFromJava& Java();
	void Run();
};
#endif
