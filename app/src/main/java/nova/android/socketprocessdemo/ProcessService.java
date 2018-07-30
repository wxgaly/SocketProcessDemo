package nova.android.socketprocessdemo;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Process;
import android.support.annotation.Nullable;
import android.util.Log;

import java.util.Timer;
import java.util.TimerTask;

/**
 * nova.android.socketprocessdemo.
 *
 * @author Created by WXG on 2018/7/30 030 14:58.
 * @version V1.0
 */
public class ProcessService extends Service {


    private static final String TAG = ProcessService.class.getSimpleName();

    private int i = 0;

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Watcher watcher = new Watcher();
        watcher.createWatcher(String.valueOf(Process.myUid()));
        watcher.connectMonitor();

        Timer timer = new Timer();

        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                Log.d(TAG, "run: 服务开启中。。。。" + i);
                i++;
            }
        }, 0, 3 * 1000);

    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return super.onStartCommand(intent, flags, startId);
    }
}
