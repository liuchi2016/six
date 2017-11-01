package com.oseasy.mmc.multirender;
import android.app.Service;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;


public class MyIntentService extends Service {
    MyReceiver     myReceiver;

    public MyIntentService() {

    }

    @Override
    public IBinder onBind(Intent intent){
        return  null;
    }


    @Override
    public int onStartCommand(Intent intent,int flags,int startId){
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction("com.oseasy.mmc.multirender.OFF");
        intentFilter.addAction("com.oseasy.mmc.multirender.ON");
        intentFilter.addAction("com.oseasy.mmc.multirender.FULLSCREEN");
        intentFilter.addAction("com.oseasy.mmc.multirender.WINDOW");
        myReceiver = new MyReceiver();
        registerReceiver(myReceiver,intentFilter);
        return START_STICKY;
    }

    public void OnCreate(){
        super.onCreate();
    }

    @Override
    public void onDestroy(){
        super.onDestroy();
        unregisterReceiver(myReceiver);
    }
}
