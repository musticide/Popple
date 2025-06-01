package com.example.bubbleshooter;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GameRenderer implements GLSurfaceView.Renderer{
    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig){
        initializeOpenGL();
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int width, int height){
        resizeScreen(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl10){
        renderFrame();
    }

    public native void initializeOpenGL();
    public native void resizeScreen(int width, int height);
    public native void renderFrame();
}
