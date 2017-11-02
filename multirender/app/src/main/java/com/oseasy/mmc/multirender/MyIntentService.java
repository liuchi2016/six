package com.oseasy.mmc.multirender;
import android.app.Service;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;


public class MyIntentService extends Service {

    public static final String TAG = "MultiRender";
    public final static String LOG = "com.oseasy.mmc.multirender.LOG";

    public MyIntentService() {
    }

    @Override
    public IBinder onBind(Intent intent){
        return  null;
    }

    @Override
    public int onStartCommand(Intent intent,int flags,int startId){
        String  actionType = intent.getAction().toString();
        LogToFile.v(TAG,actionType);
        if (actionType.equals("com.oseasy.mmc.multirender.OFF")){
            LogToFile.v(TAG,"OFF");
            MainActivity.instance.Quit();
        }else if (actionType.equals("com.oseasy.mmc.multirender.FULLSCREEN")){
            LogToFile.v(TAG,"FullScreen");
            MainActivity.instance.SetFullScreen(true);
        }else if (actionType.equals("com.oseasy.mmc.multirender.WINDOW")){
            LogToFile.v(TAG,"Window");
            MainActivity.instance.SetFullScreen(false);
        }else  if (actionType.equals("com.oseasy.mmc.multirender.ON")){
            LogToFile.v(TAG,"On");
            MainActivity.instance.Start();
        }else if (actionType.equals(LOG)){
            Bundle bundle = intent.getExtras();
            final String message = bundle.getString("message");
            LogToFile.v(TAG,message);
        }
        return START_NOT_STICKY;
    }

    @Override
    public void onCreate(){
        super.onCreate();
    }

    @Override
    public void onDestroy(){
        super.onDestroy();
    }
}
