package nova.android.socketprocessdemo;

/**
 * nova.android.socketprocessdemo.
 *
 * @author Created by WXG on 2018/7/30 030 14:57.
 * @version V1.0
 */
public class Watcher {

    static {
        System.loadLibrary("native-lib");
    }

    public native String stringFromJNI();

    public native void createWatcher(String userId);

    public native void connectMonitor();

}
