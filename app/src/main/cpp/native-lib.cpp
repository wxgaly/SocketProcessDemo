#include <jni.h>
#include <string>
#include "native_lib.h"


extern "C" JNIEXPORT jstring JNICALL
Java_nova_android_socketprocessdemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_nova_android_socketprocessdemo_Watcher_createWatcher(JNIEnv *env, jobject instance, jstring userId_) {
    const char *userId = env->GetStringUTFChars(userId_, 0);

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

            char pkg[256] = {0};
            // 保证所读到的消息是指定  apk客户端的
            if (FD_ISSET(m_child, &rfds)) {

            }
        }

    }

}