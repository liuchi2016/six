package com.oseasy.mmc.loader;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class AutoReciver extends BroadcastReceiver {

    private static final String TAG = "Loader";

    @Override
    public void onReceive(Context context, Intent i) {
        if (i.getAction().equals("android.intent.action.BOOT_COMPLETED"))
        {
            LogToFile.v(TAG,"Get boot Completed");
            Intent intent = new Intent();
            intent.putExtra("Description","autoboot");
            intent.setComponent(new ComponentName("com.oseasy.mmc.loader","com.oseasy.mmc.loader.Loader"));
            context.startService(intent);
        }
    }
}
