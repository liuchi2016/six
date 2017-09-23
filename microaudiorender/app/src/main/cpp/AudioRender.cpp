// AudioRender.cpp : Defines the entry point for the console application.
//

#include "Reciever.h"
#include <common/WaitWrapper.h>
#include "udpAudioUnPacker.h"
#include "com_oseasy_mmc_microaudiorender_AudioRender.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/program_options.hpp>
#include "logger.h"

char* jstringTostr(JNIEnv* env,jstring jstr)
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
    return pStr;
}

Reciever udpReciever;

UdpAudioUnpacker  udpAudioUnpacker;

//net thread running
JNIEXPORT void JNICALL Java_com_oseasy_mmc_microaudiorender_AudioRender_NetStart
        (JNIEnv * env, jclass clazz,jstring host,jint port,jint verifyPort)
{
    std::string _host(jstringTostr(env,host));
    LOGV("host:%s,port:%d,verifyPort:%d",_host.c_str(),port,verifyPort);
    try {
        udpReciever.Initialize(_host,port,verifyPort);
        udpReciever.DataLoop(boost::bind(&UdpAudioUnpacker::Flush,&udpAudioUnpacker,_1,_2));
    }
    catch (string_exception& e)
    {
        LOGV("%s",e.get().c_str());
    }
}


uint8_t  outArray[AACDecoder::max_audio_buffer_size];

JNIEXPORT jbyteArray JNICALL Java_com_oseasy_mmc_microaudiorender_AudioRender_GetAACFrame
        (JNIEnv * env, jclass clazz)
{
    LOGV("GetAACFrame  start");
    int  length ;
    uint8_t * out = outArray;
    udpAudioUnpacker.Get(out,length);
    LOGV("get %d bytes",length);
    jbyteArray buf = NULL;
    jsize _aac_size = length;
    jbyte* _aac_context = (jbyte*)outArray;
    buf = env->NewByteArray(_aac_size);
    env->SetByteArrayRegion(buf, 0, _aac_size, _aac_context);
    return buf;
}
