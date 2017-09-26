package com.oseasy.mmc.multiclient;
import android.app.IntentService;
import android.app.Notification;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

import java.util.Timer;
import java.util.TimerTask;

/**
 * An {@link IntentService} subclass for handling asynchronous task requests in
 * a service on a separate handler thread.
 * <p>
 * TODO: Customize class - update intent actions, extra parameters and static
 * helper methods.
 */

public class MultiClient extends Service {

    private static final String TAG = "MultiClient";

    @Override
    public IBinder onBind(Intent intent){
        return  null;
    }

    @Override
    public int onStartCommand(Intent intent,int flags,int startId){
        new Thread(new Runnable() {
            @Override
            public void run() {
                Core core = new Core(getApplicationContext());
                core.Run();
            }
        }).start();
        return START_STICKY;
    }

    public void OnCreate(){
        Log.v(TAG,"Service created");
        super.onCreate();
    }

    @Override
    public void onDestroy(){
        super.onDestroy();
        Log.v(TAG,"on destroy");
    }
}