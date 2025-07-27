package com.example.bubbleshooter;

import android.opengl.GLES20;
import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.util.Log;

public class GameRenderer implements GLSurfaceView.Renderer{

    private static final String TAG = "GameRenderer";
    private Context context;

    public GameRenderer(Context context) {
        this.context = context;
    }
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.d(TAG, "Surface created");
        AssetManager assetManager = context.getAssets();
        initializeOpenGL(assetManager);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.d(TAG, "Surface changed: " + width + "x" + height);
        resizeScreen(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        renderFrame();
    }

    // Native methods
    public native void initializeOpenGL(AssetManager assetManager);
    public native void resizeScreen(int width, int height);
    public native void renderFrame();
}
