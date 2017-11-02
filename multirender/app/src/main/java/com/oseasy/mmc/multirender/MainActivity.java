package com.oseasy.mmc.multirender;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.media.MediaCodec;
import android.media.MediaFormat;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import java.lang.Thread;
import java.nio.ByteBuffer;

/**
 * An example full-screen activity that shows and hides the system UI (i.e.
 * status bar and navigation/system bar) with user interaction.
 */
public class MainActivity extends Activity {
    static  public  MySurfaceView  videoView;
    private String host;
    private int    port;
    private int    verifyPort;
    private WindowManager mWindowManager;
    private WindowManager.LayoutParams mLayout;
    public static final String Tag = "MultiRender";
    static  public MainActivity instance;
    boolean  bWindow = false;
    boolean  bEnable;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LogToFile.init(getApplicationContext(),Tag);
        Bundle bundle = this.getIntent().getExtras();
        host = bundle.getString("Host");
        port = bundle.getInt("Port");
        verifyPort = bundle.getInt("VerityPort");
        createWindowManager();
        videoView = new MySurfaceView(this);
        videoView.setOnTouchListener(new View.OnTouchListener() {
                float mTouchStartX;
                float mTouchStartY;
                boolean bDrag = false;
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    if (!bWindow) return  false;
                    Log.v(Tag,String.format("%d",event.getAction()));
                    switch (event.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            bDrag = true;
                            mTouchStartX = event.getX();
                            mTouchStartY = event.getY();
                            break;
                        case MotionEvent.ACTION_MOVE:
                            break;
                        case MotionEvent.ACTION_UP:
                            // 更新浮动窗口位置参数
                            if (bDrag){
                                mLayout.x += (int) (event.getX() - mTouchStartX);
                                mLayout.y += (int) (event.getY() - mTouchStartY);
                                mWindowManager.updateViewLayout(v, mLayout);
                                bDrag = false;
                            }
                            break;
                    }
                    return true;
                }
        });

        LogToFile.v(Tag,String.format("%s,%d",host,port));

        requestWindowFeature(Window.FEATURE_NO_TITLE);

        setContentView(R.layout.activity_main);

        instance = this;

        new Thread(new Runnable() {
            @Override
            public void run() {
                MultiRender.NetStart(host,port,verifyPort);
            }
        }).start();

        new Handler().postDelayed(new Runnable(){

            public void run() {
                showDesk();
            }
        }, 100);

      /*  Intent intent = new Intent(this,MyIntentService.class);

        startService(intent);*/

        Intent intent = new Intent();
        intent.setComponent(new ComponentName("com.oseasy.mmc.multirender","com.oseasy.mmc.multirender.MyIntentService"));
        intent.setAction("com.oseasy.mmc.multirender.SERVER");
        startService(intent);

        bEnable = true;

      //  videoView.StartHandler();
    }

    private void showDesk() {
        mWindowManager.addView(videoView, mLayout);
        finish();
    }

    public void Quit(){
        if (bEnable){
            videoView.bdecodeStop = true;
            MultiRender.InsertInviadFrame();
            try {
                videoView.decodeThread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            if (videoView.mCodec !=null) {
                videoView.mCodec.stop();
                videoView.mCodec = null;
            }
            videoView.setVisibility(View.GONE);
            bEnable = false;
        }
    }

    public void Start(){
        if (!bEnable){
            bEnable = true;
            videoView.bdecodeStop = false;
            videoView.setVisibility(View.VISIBLE);
        }
    }

    public void SetFullScreen(boolean flag){
        if (flag == true){
            mLayout.width = ViewGroup.LayoutParams.FILL_PARENT;
            mLayout.height = ViewGroup.LayoutParams.FILL_PARENT;
            mWindowManager.updateViewLayout(videoView,mLayout);
            mLayout.y = 0;
            mLayout.x = 0;
            bWindow  = false;
        }else {
            mLayout.width = 800;
            mLayout.height = 600;
            mLayout.y = 0;
            mLayout.x = 0;
            bWindow = true;
            mWindowManager.updateViewLayout(videoView,mLayout);
        }
    }

    private void createWindowManager() {
        // 取得系统窗体
        mWindowManager = (WindowManager) getApplicationContext()
                .getSystemService(WINDOW_SERVICE);
        mLayout = new WindowManager.LayoutParams();
        mLayout.type = WindowManager.LayoutParams.TYPE_SYSTEM_ERROR;
        mLayout.flags = WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL|
                WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM;
        mLayout.format = PixelFormat.TRANSPARENT;
        mLayout.width = ViewGroup.LayoutParams.FILL_PARENT;
        mLayout.height = ViewGroup.LayoutParams.FILL_PARENT;
        mLayout.x = 0;
        mLayout.y = 0;
        bWindow = false;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    class MySurfaceView extends SurfaceView implements SurfaceHolder.Callback{


        private final static String MIME_TYPE = "video/avc";// H.264 Advanced Video

        public int video_width;

        public int video_height;

        public  MediaCodec     mCodec;

        public  Thread   decodeThread;

        public  boolean  bdecodeStop;


        public MySurfaceView(Context context){
            super(context);

            getHolder().addCallback(this);

            bdecodeStop = false;

        }

        public void Decode(){

            try {
                MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();
                ByteBuffer[] inputBuffers = mCodec.getInputBuffers();
                while (!bdecodeStop) {
                    int inIndex = mCodec.dequeueInputBuffer(0);
                    if (inIndex >= 0) {
                        ByteBuffer byteBuffer = inputBuffers[inIndex];
                        byteBuffer.clear();
                        byte[] streamBuffer = MultiRender.GetH264Frame();
                        if (bdecodeStop)
                            break;
                        byteBuffer.put(streamBuffer, 0, streamBuffer.length);
                        mCodec.queueInputBuffer(inIndex, 0, streamBuffer.length, 0, 0);
                    }
                    int outIndex = mCodec.dequeueOutputBuffer(info,1);
                    if (outIndex >= 0) {
                        mCodec.releaseOutputBuffer(outIndex, true);
                    }
                }
            }
            catch (Exception e){
                LogToFile.v("Error","exception");
            }

        }

        public void InitDecoder(){
            try {
                mCodec = MediaCodec.createDecoderByType(MIME_TYPE);
            }
            catch (java.io.IOException e){
                e.printStackTrace();
            }

            MediaFormat mediaFormat = MediaFormat.createVideoFormat(MIME_TYPE,
                    video_width, video_height);

            mCodec.configure(mediaFormat,getHolder().getSurface(),
                    null, 0);

            mCodec.start();
        }



        public void StartHandler(){
            new Handler().postDelayed(new Runnable(){

                public void run() {
                    short[] resolution = MultiRender.GetResolution();

                    if (resolution[0] != 0 && resolution[1] != 0 ) {
                        video_width = resolution[0];
                        video_height = resolution[1];
                        InitDecoder();
                        decodeThread = new Thread(new Runnable() {
                            @Override
                            public void run() {
                                Decode();
                            }
                        });
                        decodeThread.start();
                    }
                    else
                        StartHandler();
                }
            }, 10);
        }

        @Override
        public void surfaceCreated(SurfaceHolder holder) {
            StartHandler();
        }

        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

        }

        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {

        }
    }
}


