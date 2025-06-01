package com.example.bubbleshooter;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.opengl.GLSurfaceView;

//import com.example.bubbleshooter.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'bubbleshooter' library on application startup.
    static {
        System.loadLibrary("bubbleshooter");
    }

    private GLSurfaceView glSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        glSurfaceView = findViewById(R.id.gl_surface_view);

        glSurfaceView.setEGLContextClientVersion(2);
        glSurfaceView.setRenderer(new GameRenderer());
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