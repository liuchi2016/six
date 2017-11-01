package com.oseasy.mmc.loader;

import android.app.ActivityManager;
import android.app.Notification;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.IBinder;
import android.util.Log;

import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

public class Loader extends Service {

    public static final String CONFIG_MEDIA_TEACHER_IP = "oemmdteacherip";
    public static final String CONFIG_MEDIA_STUDENT_IP = "oemmdstudentip";
    public static final String CONFIG_MEDIA_CLIENT_MAC = "oemmdclientmac";

    private static final String TAG = "Loader";

    private boolean  isFirst = true;


    public Loader(){
    }

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

    public String GetConfig(Context context,String type){
        String  result = "";
        switch (type){
            case "mac":
                result = PropertyUtil.getProperty(context,CONFIG_MEDIA_CLIENT_MAC);
                break;
            case "connect":
                result = PropertyUtil.getProperty(context,CONFIG_MEDIA_TEACHER_IP);
                break;
            case "filter":
                result = PropertyUtil.getProperty(context,CONFIG_MEDIA_STUDENT_IP);
                break;
            default:
                break;
        }
        if (result == null)
            result = "";

        return result;
    }

    @Override
    public int onStartCommand(final Intent intent, final int flags, int startId){

        if (isFirst){
            isFirst = false;
            String     des = "";
            try {
                des = intent.getExtras().getString("Description");
            }
            catch (NullPointerException e){
                des = " launcher";
            }

            LogToFile.v(TAG,String.format("Started by %s",des));

            Timer timer = new Timer();

            TimerTask task = new TimerTask() {
                @Override
                public void run() {
                    Context context = getApplicationContext();
                    String connectIp = GetConfig(context,"connect");
                    String filterIp = GetConfig(context,"filter");
                    String mac = GetConfig(context,"mac");
                    if (!connectIp.isEmpty()
                            && !filterIp.isEmpty()
                            && !mac.isEmpty()) {
                        if (OemUtil.isOemTongFang()){
                            if (!checkPackage(context, "com.oseasy.mmc.multiclientyh")) {
                                Intent activity = new Intent();
                                activity.putExtra("teacher", connectIp);
                                activity.putExtra("filter", filterIp);
                                activity.putExtra("mac",mac);
                                activity.setAction("com.oseasy.mmc.multiclientyh.action.COREOTHER");
                                startService(activity);
                                LogToFile.v(TAG,String.format("type:yh,teacher:%s,filter:%s,mac:%s",connectIp,filterIp,mac));
                            }
                        }else {
                            if (!checkPackage(context, "com.oseasy.mmc.multiclient")) {
                                Intent activity = new Intent();
                                activity.putExtra("teacher", connectIp);
                                activity.putExtra("filter", filterIp);
                                activity.putExtra("mac",mac);
                                activity.setAction("com.oseasy.mmc.multiclient.action.COREOTHER");
                                startService(activity);
                                LogToFile.v(TAG,String.format("type:normal,teacher:%s,filter:%s,mac:%s",connectIp,filterIp,mac));
                            }
                        }
                    }
                }
            };
            timer.schedule(task,15000,20000);
        }
        return START_STICKY;
    }

    @Override
    public void onCreate(){
        Context  context = getApplicationContext();
        LogToFile.init(context,TAG);
        LogToFile.v(TAG,"Service created");
        super.onCreate();
    }

    @Override
    public void onDestroy(){
        LogToFile.v(TAG,"Service end");
        super.onDestroy();
    }
}
