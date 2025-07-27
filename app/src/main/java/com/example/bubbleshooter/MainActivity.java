package com.example.bubbleshooter;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.opengl.GLSurfaceView;

//import com.example.bubbleshooter.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    private GLSurfaceView glSurfaceView;
    // Used to load the 'bubbleshooter' library on application startup.
    static {
        System.loadLibrary("bubbleshooter");
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        glSurfaceView = new GLSurfaceView(this);
        glSurfaceView.setEGLContextClientVersion(2);

        GameRenderer renderer = new GameRenderer(this);
        glSurfaceView.setRenderer(renderer);

        setContentView(glSurfaceView);
    }

    @Override
    protected  void onResume() {
        super.onResume();
        glSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        glSurfaceView.onPause();
    }
}