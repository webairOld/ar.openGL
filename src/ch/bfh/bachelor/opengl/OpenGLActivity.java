package ch.bfh.bachelor.opengl;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;

public class OpenGLActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        View mainView = new OpenGLSurfaceView(getApplication());
        setContentView(mainView);
    }
}