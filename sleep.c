#include "sleep.h"  // 包含 sleep.h 文件，用于声明 sleep 函数

// 如果定义了 _POSIX_C_SOURCE 并且其值大于或等于 199309L，那么就定义 sleep 函数
#ifdef _POSIX_C_SOURCE //>= 199309L
// sleep 函数，该函数接受一个参数 sec，表示要暂停的秒数
void sleep(int sec) {
    struct timespec ts;  // 定义一个 timespec 结构体变量 ts，用于表示时间
    ts.tv_sec = sec;  // 设置 ts 的秒数为 sec
    ts.tv_nsec = 0;  // 设置 ts 的纳秒数为 0
    nanosleep(&ts, NULL);  // 调用 nanosleep 函数，使程序暂停 sec 秒
}
#endif