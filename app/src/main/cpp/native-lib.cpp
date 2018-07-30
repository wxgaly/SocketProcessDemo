#include <jni.h>
#include <string>
#include "native_lib.h"


const char *userId;


extern "C"
JNIEXPORT void JNICALL
Java_nova_android_socketprocessdemo_Watcher_createWatcher(JNIEnv *env, jobject instance, jstring userId_) {
    userId = env->GetStringUTFChars(userId_, 0);

    //开双进程
    pid_t pid = fork();

    if (pid < 0) {

    } else if (pid == 0) {
        // 子进程
        child_do_work();
    } else if (pid > 0) {
        //父进程

    }


    env->ReleaseStringUTFChars(userId_, userId);
}

const char *PATH = "/dat/data/nova.android.socketprocessdemo/my.sock";
int m_child;

/**
 * 服务端读取信息
 *
 * @return
 */
int child_create_channel() {

    int listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    unlink(PATH);

    struct sockaddr_un addr;

    //清空内存
    memset(&addr, 0, sizeof(sockaddr_un));

    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, PATH);

    int connfd = 0;

    if (bind(listenfd, reinterpret_cast<const sockaddr *>(&addr), sizeof(sockaddr_un)) < 0) {
        LOGE("绑定错误");
        return 0;
    }

    listen(listenfd, 5);

    // 保证宿主进程连接成功
    while (1) {

        //返回客户端的地址
        if ((connfd = accept(listenfd, NULL, NULL)) < 0) {

            if (errno == EINTR) {
                continue;
            } else {
                LOGE("读取错误");
                return 0;
            }
        }

        m_child = connfd;

        LOGE("apk 父进程连接上了 %d ", m_child);

        break;
    }

    return 1;
}


void child_do_work() {
    // 开启socket 服务端
    if (child_create_channel()) {
        child_listen_msg();
    }

}

/**
 * 创建服务器端的socket
 */
void child_listen_msg() {

    fd_set rfds;

    struct timeval timeout = {3, 0};

    while (1) {

        //清除内容
        FD_ZERO(&rfds);
        FD_SET(m_child, &rfds);

        int r = select(m_child + 1, &rfds, NULL, NULL, &timeout);
        LOGE("读取消息前 %d ", r);

        if (r > 0) {

            //缓冲区
            char pkg[256] = {0};
            // 保证所读到的消息是指定  apk客户端的
            if (FD_ISSET(m_child, &rfds)) {
                //阻塞式函数
                int result = read(m_child, pkg, sizeof(pkg));

                //开启服务
                execlp("am", "am", "startservice", "--user", userId, "nova.android.socketprocessdemo/nova.android"
                        ".socketprocessdemo.ProcessService", (char *) NULL);
                break;
            }
        }

    }

}

extern "C"
JNIEXPORT void JNICALL
Java_nova_android_socketprocessdemo_Watcher_connectMonitor(JNIEnv *env, jobject instance) {

    int socked;
    struct sockaddr_un addr;

    while (1) {

        LOGE("客户端 父进程开始连接");

        socked = socket(AF_LOCAL, SOCK_STREAM, 0);

        if (socked < 0) {
            LOGE("连接失败");
            return;
        }


        //清空内存
        memset(&addr, 0, sizeof(sockaddr_un));

        addr.sun_family = AF_LOCAL;
        strcpy(addr.sun_path, PATH);

        if (connect(socked, reinterpret_cast<const sockaddr *>(&addr), sizeof(sockaddr_un)) < 0) {
            LOGE("连接失败");
            close(socked);
            sleep(1);
            continue;

        }

        LOGE("连接成功");

        break;

    }

}extern "C"
JNIEXPORT jstring JNICALL
Java_nova_android_socketprocessdemo_Watcher_stringFromJNI(JNIEnv *env, jobject instance) {

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}