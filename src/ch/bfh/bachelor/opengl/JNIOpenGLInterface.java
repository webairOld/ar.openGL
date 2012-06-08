package ch.bfh.bachelor.opengl;

public class JNIOpenGLInterface {
    static {
        System.loadLibrary("opengl-jni");
    }

    public static native void init(int width, int height);
    public static native void step();
}
