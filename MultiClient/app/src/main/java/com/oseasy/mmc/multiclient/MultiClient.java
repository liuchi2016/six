package com.oseasy.mmc.multiclient;
import android.app.ActivityManager;
import android.app.IntentService;
import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.IBinder;
import java.util.List;

/**
 * An {@link IntentService} subclass for handling asynchronous task requests in
 * a service on a separate handler thread.
 * <p>
 * TODO: Customize class - update intent actions, extra parameters and static
 * helper methods.
 */

public class MultiClient extends Service {

    public static final String TAG = "MultiClient";
    public static final String START = "com.oseasy.mmc.multiclient.action.COREOTHER";
    public static final String OPENFUNC = "com.oseasy.mmc.multiclient.OPENFUNC";
    public static final String STOPFUNC = "com.oseasy.mmc.multiclient.STOPFUNC";
    public static final String SETFUNC = "com.oseasy.mmc.multiclient.SETFUNC";
    public final static String SCENE = "com.oe.VideoView.scene";
    public final static String SCENE_VIDEO_BROADCAST= "VideoBroadcast";
    public final static String ACTION_PLAYER_QUIT = "com.oe.videoplayer.QUIT";
    public final static String LOG = "com.oseasy.mmc.multiclient.LOG";
    public boolean      inBroadCast = false;
    public boolean      type = false;
    static {
        System.loadLibrary("Core");
    }

    @Override
    public IBinder onBind(Intent intent){
        return  null;
    }

    @Override
    public int onStartCommand(Intent intent,int flags,int startId){
        String  actionType = intent.getAction();
        LogToFile.v(TAG,String.format("action:%s",actionType));
        if (actionType.equals(START)){
            Bundle bundle = intent.getExtras();
            final String teacherIp = bundle.getString("teacher");
            final String filterIp = bundle.getString("filter");
            final String mac = bundle.getString("mac");
            this.type = bundle.getBoolean("type");
           // LogToFile.v(TAG,"message");
            new Thread(new Runnable() {
                @Override
                public void run() {
                    Start(teacherIp,filterIp,mac);
                }
            }).start();

            if (this.type){
                Thread  thread = new Thread(new Runnable() {
                    @Override
                    public void run() {

                        while (true){
                            try {
                                Thread.sleep(3000);
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }

                            if (inBroadCast){
                                if (!checkPackage(getApplicationContext(), "com.oseasy.mmc.multirender")) {
                                    LogToFile.v(TAG, "KillSelf");
                                    android.os.Process.killProcess(android.os.Process.myPid());
                                }
                            }
                        }
                    }
                });
                thread.start();
            }
        }else if (actionType.equals(STOPFUNC)){
            Bundle bundle = intent.getExtras();
            final String type = bundle.getString("type");
            StopFunc(type);
        }
        else  if (actionType.equals(SETFUNC)){
            Bundle bundle = intent.getExtras();
            final String type = bundle.getString("type");
            final int value = bundle.getInt("value");
            boolean ret = (value == 1) ? true : false;
            Set(type,ret);

        }else if (actionType.equals(OPENFUNC)){
            Bundle bundle = intent.getExtras();
            final String name = bundle.getString("type");
            final String host = bundle.getString("host");
            final int port = bundle.getInt("port");
            final int verityPort = bundle.getInt("verityPort");
            StartFunc(name,host,port,verityPort);
        }else if (actionType.equals(LOG)){
            Bundle bundle = intent.getExtras();
            final String message = bundle.getString("message");
            LogToFile.v(TAG,message);
        }
        return START_NOT_STICKY;
    }

    @Override
    public void onCreate(){
        LogToFile.init(getApplicationContext(),TAG);
        super.onCreate();
    }

    @Override
    public void onDestroy(){
        LogToFile.v(TAG,"MultiClient destroy");
        super.onDestroy();
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

    public void  StartFunc(String name,String ip,int port,int verityPort){
        LogToFile.v(TAG,String.format("Excute Func:%s,Params:%s:%d",name,ip,port));
        switch (name){
            case "screen": {
                if (!type){
                    if (!checkPackage(getApplicationContext(), "com.oseasy.mmc.multirender")){
                        Intent intent = new Intent();
                        intent.setAction("com.oseasy.mmc.multirender.OPEN");
                        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                        intent.putExtra("Host", ip);
                        intent.putExtra("Port", port);
                        intent.putExtra("VerityPort",verityPort);
                        startActivity(intent);
                    }
                    else {
                        Intent intentFullScreen = new Intent();
                        intentFullScreen.setComponent(new ComponentName("com.oseasy.mmc.multirender","com.oseasy.mmc.multirender.MyIntentService"));
                        intentFullScreen.setAction("com.oseasy.mmc.multirender.FULLSCREEN");
                        startService(intentFullScreen);

                        Intent intent = new Intent();
                        intent.setComponent(new ComponentName("com.oseasy.mmc.multirender","com.oseasy.mmc.multirender.MyIntentService"));
                        intent.setAction("com.oseasy.mmc.multirender.ON");
                        startService(intent);
                    }
                }else {
                    if (!checkPackage(getApplicationContext(), "com.oseasy.mmc.multirender")) {
                        Intent intent = new Intent();
                        intent.setAction("com.oseasy.mmc.multirender.OPEN");
                        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                        intent.putExtra("Host", ip);
                        intent.putExtra("Port", port);
                        intent.putExtra("VerityPort",verityPort);
                        startActivity(intent);
                    }
                    inBroadCast = true;
                }
                break;
            }
            case "mac": {
                Intent intent = new Intent();
                intent.setAction("com.oseasy.mmc.microaudiorender.OPEN");
                intent.setPackage("com.oseasy.mmc.microaudiorender");
                intent.putExtra("Host", ip);
                intent.putExtra("Port", port);
                intent.putExtra("VerityPort",verityPort);
                startService(intent);
                break;
            }
            case "audio":{
                Intent intent = new Intent();
                intent.setAction("com.oseasy.mmc.localaudiorender.OPEN");
                intent.setPackage("com.oseasy.mmc.localaudiorender");
                intent.putExtra("Host", ip);
                intent.putExtra("Port", port);
                intent.putExtra("VerityPort",verityPort);
                startService(intent);
                break;
            }
            case "techvideo":{
                String uri = String.format("udp://@%s:%d",ip,port);
                Intent intent = new Intent("com.oe.videoplayer.PLAY", Uri.parse(uri));
                //是否全屏：不使用此参数，默认全屏
                intent.putExtra("com.oe.videoplayer.fullscreen", true);
                intent.putExtra(SCENE, SCENE_VIDEO_BROADCAST);
                intent.setFlags(Intent.FLAG_ACTIVITY_MULTIPLE_TASK | Intent.FLAG_ACTIVITY_NEW_TASK);
                startActivity(intent);
                break;
            }
            default:
                break;
        }
    }

    public void  StopFunc(String name){
        LogToFile.v(TAG,String.format("Stop Func:%s",name));
        switch (name){
            case "screen": {
                if (!type){
                    if (checkPackage(getApplicationContext(), "com.oseasy.mmc.multirender")){
                        Intent intent = new Intent();
                        intent.setComponent(new ComponentName("com.oseasy.mmc.multirender","com.oseasy.mmc.multirender.MyIntentService"));
                        intent.setAction("com.oseasy.mmc.multirender.OFF");
                        startService(intent);
                    }
                }else {
                    if (checkPackage(getApplicationContext(),"com.oseasy.mmc.multirender")){
                        inBroadCast = false;
                        Intent intent = new Intent();
                        intent.setComponent(new ComponentName("com.oseasy.mmc.multirender","com.oseasy.mmc.multirender.MyIntentService"));
                        intent.setAction("com.oseasy.mmc.multirender.CLOSE");
                        startService(intent);
                    }
                }
                break;
            }
            case "mac": {
                Intent intent = new Intent("com.oseasy.mmc.microaudiorender.CLOSE");
                sendBroadcast(intent);
                ShellUtil.exec("busybox pkill microaudiorender");
                break;
            }
            case "audio":{
                Intent intent = new Intent("com.oseasy.mmc.localaudiorender.CLOSE");
                sendBroadcast(intent);
                ShellUtil.exec("busybox pkill localaudiorender");
                break;
            }
            case "techvideo":{
                Intent intent = new Intent();
                intent.setAction(ACTION_PLAYER_QUIT);
                sendBroadcast(intent);
                break;
            }
            default:
                break;
        }
    }

    public void Set(String name ,boolean value){
        LogToFile.i(TAG,String.format("Set Value:%s=%b",name,value));
        if (checkPackage(getApplicationContext(),"com.oseasy.mmc.multirender")){
            switch (name){
                case "fullscreen":{
                    if (value){
                        Intent intent = new Intent();
                        intent.setComponent(new ComponentName("com.oseasy.mmc.multirender","com.oseasy.mmc.multirender.MyIntentService"));
                        intent.setAction("com.oseasy.mmc.multirender.FULLSCREEN");
                        startService(intent);

                    }else {
                        Intent intent = new Intent();
                        intent.setComponent(new ComponentName("com.oseasy.mmc.multirender","com.oseasy.mmc.multirender.MyIntentService"));
                        intent.setAction("com.oseasy.mmc.multirender.WINDOW");
                        startService(intent);
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }

    public  native void Start(String teacherIp,String filterIp,String mac);
}