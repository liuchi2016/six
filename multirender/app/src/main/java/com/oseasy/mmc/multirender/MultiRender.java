package com.oseasy.mmc.multirender;

/**
 * Created by Jonesx on 2016/3/12.
 */
public class MultiRender {

    static {
        System.loadLibrary("MultiRender");
    }

    public static native void NetStart(String host,int port,int VerityPort);

    public static native byte[] GetH264Frame();

    public static native short[] GetResolution();
}
