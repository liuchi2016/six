package com.oseasy.mmc.multirender;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class MyReceiver extends BroadcastReceiver {
    public static final String Tag = "MultiRender";

    public MyReceiver(){

    }

    @Override
    public void onReceive(Context context, Intent intent) {
        // TODO: This method is called when the BroadcastReceiver is receiving
        // an Intent broadcast.
        String  actionType = intent.getAction().toString();
        Log.v(MainActivity.Tag,actionType);
        if (actionType == "com.oseasy.mmc.multirender.OFF"){
            Log.v(Tag,"close MultiRender");
            MainActivity.instance.Quit();
           // android.os.Process.killProcess(android.os.Process.myPid());
        }else if (actionType == "com.oseasy.mmc.multirender.FULLSCREEN"){
            MainActivity.instance.SetFullScreen(true);

        }else if (actionType == "com.oseasy.mmc.multirender.WINDOW"){
            MainActivity.instance.SetFullScreen(false);
        }else  if (actionType == "com.oseasy.mmc.multirender.ON"){
            MainActivity.instance.Start();
        }
    }
}
