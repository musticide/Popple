package com.example.bubbleshooter;

import androidx.appcompat.app.AppCompatActivity;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.os.Bundle;
import android.opengl.GLSurfaceView;
import android.widget.Toast;
import android.util.Log;

//import com.example.bubbleshooter.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "GameRenderer";
    private GLSurfaceView glSurfaceView;
    // Used to load the 'bubbleshooter' library on application startup.
    static {
        System.loadLibrary("bubbleshooter");
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if(!checkGLES30Support())
        {
            Log.e(TAG, "OpenGL ES 3.0 not supported on this device");
            Toast.makeText(this, "OpenGL ES 3.0 Not supported", Toast.LENGTH_LONG).show();
            finish();
            return;
        }

        glSurfaceView = new GLSurfaceView(this);
        glSurfaceView.setEGLContextClientVersion(3);

        GameRenderer renderer = new GameRenderer(this);
        glSurfaceView.setRenderer(renderer);

        setContentView(glSurfaceView);
    }

    private boolean checkGLES30Support(){
        ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();

        return configurationInfo.reqGlEsVersion >= 0x30000;
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