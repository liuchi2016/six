package com.oseasy.mmc.microaudiorender;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class MyReceiver extends BroadcastReceiver {

    public static final String Tag = "MicroAudioRender";
    @Override
    public void onReceive(Context context, Intent intent) {
        // TODO: This method is called when the BroadcastReceiver is receiving
        // an Intent broadcast.
        Log.v(Tag,"Closing MicroAudioRender");
        android.os.Process.killProcess(android.os.Process.myPid());
    }
}
