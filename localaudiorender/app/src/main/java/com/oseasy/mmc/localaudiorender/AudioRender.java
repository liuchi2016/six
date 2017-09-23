package com.oseasy.mmc.localaudiorender;

/**
 * Created by Jonesx on 2016/3/12.
 */
public class AudioRender {

    static {
        System.loadLibrary("AudioRender");
    }

    public static native void NetStart(String host,int port,int verifyPort);

    public static native byte[] GetAACFrame();

}
