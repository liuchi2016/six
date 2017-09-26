package com.oseasy.mmc.loader;

import android.app.ActivityManager;
import android.app.Notification;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.IBinder;
import android.util.Log;

import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

public class Loader extends Service {

    public Loader() {
    }

    private static final String TAG = "Loader";

    @Override
    public IBinder onBind(Intent intent){
        return  null;
    }

    public boolean  checkPackage(Context context,String packageName){
        if (packageName == null || "".equals(packageName))
            return  false;
        ActivityManager am = (ActivityManager)context.getSystemService(Context.ACTIVITY_SERVICE);
        List<ActivityManager.RunningAppProcessInfo> list = am.getRunningAppProcesses();

        if (list == null)
            return  false;

        for (ActivityManager.RunningAppProcessInfo info : list){
            if (info.processName.equals(packageName)){
                return true;
            }
        }

        return  false;
    }

    @Override
    public int onStartCommand(Intent intent,int flags,int startId){
        Timer timer = new Timer();
        TimerTask task = new TimerTask() {
            @Override
            public void run() {
                Context context = getApplicationContext();
                if (!checkPackage(context,"com.oseasy.mmc.multiclient")){
                    Intent activity = new Intent();
                    activity.setAction("com.oseasy.mmc.multiclient.action.COREOTHER");
                    startService(activity);
                }
            }
        };
        timer.schedule(task,15000,2000);

        return START_STICKY;
    }

    public void OnCreate(){
        Log.d(TAG,"Service created");
        super.onCreate();
    }

    @Override
    public void onDestroy(){
        super.onDestroy();
        Log.v(TAG,"on destroy");
    }
}
