package com.oseasy.mmc.multiclient;

import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;

import java.util.List;

/**
 * Created by admin on 17-7-28.
 */

public class Core {

    public Context context;
    public static final String TAG = "MultiClient";
    public static final String CONFIG_MEDIA_TEACHER_IP = "oemmdteacherip";
    public static final String CONFIG_MEDIA_STUDENT_IP = "oemmdstudentip";
    public static final String CONFIG_MEDIA_CLIENT_MAC = "oemmdclientmac";
    public final static String SCENE = "com.oe.VideoView.scene";
    public final static String SCENE_VIDEO_BROADCAST= "VideoBroadcast";
    public final static String ACTION_PLAYER_QUIT = "com.oe.videoplayer.QUIT";

    static {
        System.loadLibrary("Core");
    }

    public Core(Context context){
        this.context = context;
    }

    public void Run(){
        Start();
    }

    public  native void Start();

    public String GetConfig(String type){
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
        return result;
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
        Log.i(TAG,String.format("Excute Func:%s,Params:%s:%d",name,ip,port));
        switch (name){
            case "screen": {
                if (!checkPackage(context,"com.oseasy.mmc.multirender")){
                    Intent intent = new Intent();
                    intent.setAction("com.oseasy.mmc.multirender.OPEN");
                    intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                    intent.putExtra("Host", ip);
                    intent.putExtra("Port", port);
                    intent.putExtra("VerityPort",verityPort);
                    context.startActivity(intent);
                }
                else {
                    Intent intent = new Intent("com.oseasy.mmc.multirender.ON");
                    context.sendBroadcast(intent);
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
                context.startService(intent);
                break;
            }
            case "audio":{
                Intent intent = new Intent();
                intent.setAction("com.oseasy.mmc.localaudiorender.OPEN");
                intent.setPackage("com.oseasy.mmc.localaudiorender");
                intent.putExtra("Host", ip);
                intent.putExtra("Port", port);
                intent.putExtra("VerityPort",verityPort);
                context.startService(intent);
                break;
            }
            case "techvideo":{
                String uri = String.format("udp://@%s:%d",ip,port);
                Intent intent = new Intent("com.oe.videoplayer.PLAY", Uri.parse(uri));
                //是否全屏：不使用此参数，默认全屏
                intent.putExtra("com.oe.videoplayer.fullscreen", true);
                intent.putExtra(SCENE, SCENE_VIDEO_BROADCAST);
                intent.setFlags(Intent.FLAG_ACTIVITY_MULTIPLE_TASK | Intent.FLAG_ACTIVITY_NEW_TASK);
                context.startActivity(intent);
                break;
            }
            default:
                break;
        }
    }

    public void  StopFunc(String name){
        Log.i(TAG,String.format("Stop Func:%s",name));
        switch (name){
            case "screen": {
                Intent intent = new Intent("com.oseasy.mmc.multirender.OFF");
                context.sendBroadcast(intent);
                break;
            }
            case "mac": {
                Intent intent = new Intent("com.oseasy.mmc.microaudiorender.CLOSE");
                context.sendBroadcast(intent);
                ShellUtil.exec("busybox pkill microaudiorender");
                break;
            }
            case "audio":{
                Intent intent = new Intent("com.oseasy.mmc.localaudiorender.CLOSE");
                context.sendBroadcast(intent);
                ShellUtil.exec("busybox pkill localaudiorender");
                break;
            }
            case "techvideo":{
                Intent intent = new Intent();
                intent.setAction(ACTION_PLAYER_QUIT);
                context.sendBroadcast(intent);
                break;
            }
            default:
                break;
        }
    }

    public void Set(String name ,boolean value){
        Log.i(TAG,String.format("Set Value:%s=%b",name,value));
        switch (name){
            case "fullscreen":{
                if (value){
                    Intent intent = new Intent("com.oseasy.mmc.multirender.FULLSCREEN");
                    context.sendBroadcast(intent);
                }else {
                    Intent intent = new Intent("com.oseasy.mmc.multirender.WINDOW");
                    context.sendBroadcast(intent);
                }
                break;
            }
            default:
                break;
        }

    }
}
