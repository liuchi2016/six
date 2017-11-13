#include "com_oseasy_mmc_multirender_MultiRender.h"
#include "stdio.h"
#include <string>
#include "udpImageUnPacker.h"
#include "Reciever.h"
#include <boost/bind.hpp>


char* jstringTostr(JNIEnv* env,jstring jstr)
{
    char * pStr = 0;

    jclass jstrObj = env->FindClass("java/lang/String");
    jstring encode = env->NewStringUTF("utf-8");
    jmethodID methodId = env->GetMethodID(jstrObj,"getBytes","(Ljava/lang/String;)[B");
    jbyteArray byteArray=(jbyteArray)env->CallObjectMethod(jstr,methodId,encode);
    jsize strLen  =env->GetArrayLength(byteArray);
    jbyte *jBuf = env->GetByteArrayElements(byteArray,JNI_FALSE);
    if (jBuf != 0){
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

UdpImageUnpacker  udpImageUnpacker;


//net thread running
JNIEXPORT void JNICALL Java_com_oseasy_mmc_multirender_MultiRender_NetStart
        (JNIEnv * env, jclass clazz,jstring host,jint port,jint VerityPort)
{
    try {
        std::string _host(jstringTostr(env,host));
        udpReciever.Initialize(_host,port,VerityPort);
        udpReciever.DataLoop(boost::bind(&UdpImageUnpacker::Flush,&udpImageUnpacker,_1,_2));
    }catch (string_exception& e){
        char  intent[1024] = {0};
        sprintf(intent,"am startservice -n com.oseasy.mmc.multirender/.MyIntentService -a com.oseasy.mmc.multirender.LOG --es message %s",e.get().c_str());
        system(intent);
    }
}

JNIEXPORT jbyteArray JNICALL Java_com_oseasy_mmc_multirender_MultiRender_GetH264Frame
        (JNIEnv * env, jclass clazz)
{
    UdpImageUnpacker::ElementCls  element = udpImageUnpacker.queues.Pop();
    jbyteArray buf = NULL;
    jsize _h264_size = element->size();
    jbyte* _h264_context = (jbyte*)element->get();
    buf = env->NewByteArray(_h264_size);
    env->SetByteArrayRegion(buf, 0, _h264_size, _h264_context);
    delete  element;
    return buf;
}

JNIEXPORT jshortArray JNICALL Java_com_oseasy_mmc_multirender_MultiRender_GetResolution
        (JNIEnv * env, jclass clazz)
{
    jshortArray buf = NULL;
    short info[2];
    info[0] = udpImageUnpacker.width;
    info[1] = udpImageUnpacker.height;
    buf = env->NewShortArray(2);
    env->SetShortArrayRegion(buf, 0, 2, info);
    return buf;
}


JNIEXPORT void JNICALL Java_com_oseasy_mmc_multirender_MultiRender_InsertInviadFrame
        (JNIEnv * env, jclass clazz)
{
    uint8_t * ptrInViadFrame = new uint8_t[4];
    udpImageUnpacker.queues.Push(new UdpImageUnpacker::BufferCls(ptrInViadFrame,4));
    delete[] ptrInViadFrame;
}


JNIEXPORT void JNICALL Java_com_oseasy_mmc_multirender_MultiRender_NetPause
        (JNIEnv * env, jclass clazz,jint value)
{
    udpReciever.SetState(value);
}
