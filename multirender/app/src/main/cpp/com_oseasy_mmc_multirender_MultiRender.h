/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class jonesx_videoplayer_VideoPlayer */

#ifndef _Included_com_oseasy_mmc_multirender_MultiRender
#define _Included_com_oseasy_mmc_multirender_MultiRender
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     jonesx_videoplayer_VideoPlayer
 * Method:    play
 * Signature: (Ljava/lang/Object;)I
 */

JNIEXPORT void JNICALL Java_com_oseasy_mmc_multirender_MultiRender_NetStart
  (JNIEnv *, jclass,jstring,jint,jint);

JNIEXPORT jbyteArray JNICALL Java_com_oseasy_mmc_multirender_MultiRender_GetH264Frame
        (JNIEnv*,jclass);

JNIEXPORT jshortArray JNICALL Java_com_oseasy_mmc_multirender_MultiRender_GetResolution
        (JNIEnv * env, jclass clazz);

JNIEXPORT void JNICALL Java_com_oseasy_mmc_multirender_MultiRender_InsertInviadFrame
        (JNIEnv * env, jclass );

#ifdef __cplusplus
}
#endif

#endif
