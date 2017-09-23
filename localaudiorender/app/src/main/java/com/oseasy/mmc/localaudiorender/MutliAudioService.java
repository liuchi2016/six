package com.oseasy.mmc.localaudiorender;

import android.app.IntentService;
import android.content.Intent;
import android.content.Context;
import android.content.IntentFilter;
import android.media.AudioFormat;
import android.os.Bundle;
import android.util.Log;

/**
 * An {@link IntentService} subclass for handling asynchronous task requests in
 * a service on a separate handler thread.
 * <p>
 * TODO: Customize class - update intent actions, extra parameters and static
 * helper methods.
 */
public class MutliAudioService extends IntentService {
    // TODO: Rename actions, choose action names that describe tasks that this
    private static final String ACTION_CORE = "com.oseasy.mmc.localaudiorender.OPEN";

    private MmcAudioTracker tracker;

    private String host;

    private int    port;

    private int verifyPort;

    MyReceiver myReceiver;

    public static final String Tag = "LocalAudioRender";

    public MutliAudioService() {
        super("MutliAudioService");
    }

    /**
     * Starts this service to perform action Foo with the given parameters. If
     * the service is already performing a task this action will be queued.
     *
     * @see IntentService
     */

    @Override
    protected void onHandleIntent(Intent intent) {
        if (intent != null) {
            final String action = intent.getAction();
            if (ACTION_CORE.equals(action)) {
                Bundle bundle = intent.getExtras();
                host = bundle.getString("Host");
                port = bundle.getInt("Port");
                verifyPort = bundle.getInt("VerityPort");
                Log.v(Tag,String.format("%s,%d,%d",host,port,verifyPort));
                handleActionCore();
            }
        }
    }

    /**
     * Handle action Foo in the provided background thread with the provided
     * parameters.
     */
    private void handleActionCore() {
        // TODO: Handle action Foo.

        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction("com.oseasy.mmc.localaudiorender.CLOSE");

        myReceiver = new MyReceiver();
        registerReceiver(myReceiver,intentFilter);

        tracker = new MmcAudioTracker(44100, AudioFormat.CHANNEL_OUT_STEREO, AudioFormat.ENCODING_PCM_16BIT);

        tracker.init();

        new Thread(new Runnable() {
            @Override
            public void run() {
                AudioRender.NetStart(host,port,verifyPort);
            }
        }).start();


        while (true) {
            byte[] stream = AudioRender.GetAACFrame();
            tracker.playAudioTrack(stream, 0, stream.length);
        }
    }

}
