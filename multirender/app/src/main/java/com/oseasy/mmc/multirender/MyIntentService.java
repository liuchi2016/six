package com.oseasy.mmc.multirender;

import android.app.IntentService;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.MediaCodec;

import java.nio.ByteBuffer;

public class MyIntentService extends IntentService {
    MyReceiver     myReceiver;

    public MyIntentService() {
        super("MyIntentService");
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction("com.oseasy.mmc.multirender.CLOSE");
        intentFilter.addAction("com.oseasy.mmc.multirender.FULLSCREEN");
        intentFilter.addAction("com.oseasy.mmc.multirender.WINDOW");
        myReceiver = new MyReceiver();
        registerReceiver(myReceiver,intentFilter);
        ByteBuffer[] inputBuffers = MainActivity.videoView.mCodec.getInputBuffers();
        MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();
        while (true) {
            int inIndex = MainActivity.videoView.mCodec.dequeueInputBuffer(-1);
            if (inIndex >= 0) {
                ByteBuffer byteBuffer = inputBuffers[inIndex];
                byteBuffer.clear();
                byte[] streamBuffer = MultiRender.GetH264Frame();
                byteBuffer.put(streamBuffer, 0, streamBuffer.length);
                MainActivity.videoView.mCodec.queueInputBuffer(inIndex, 0, streamBuffer.length, 0, 0);
            }
            int outIndex = MainActivity.videoView.mCodec.dequeueOutputBuffer(info, 0);
            if (outIndex >= 0)
                MainActivity.videoView.mCodec.releaseOutputBuffer(outIndex,true);
        }
    }

    @Override
    public void onDestroy(){
        super.onDestroy();
        unregisterReceiver(myReceiver);
    }

}
