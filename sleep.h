// sleep.h 文件，提供了跨平台的 sleep 函数
#ifndef SLEEP_H  // 如果没有定义 SLEEP_H
#define SLEEP_H  // 定义 SLEEP_H

#ifdef _WIN32  // 如果是在 Windows 平台
    #include <windows.h>  // 包含 windows.h 文件，用于调用 Windows API
    #define sleep(x) Sleep(1000 * (x))  // 定义 sleep 函数，使用 Windows API 的 Sleep 函数实现，参数 x 表示要暂停的秒数
#elif _POSIX_C_SOURCE >= 199309L  // 如果是在支持 POSIX 的平台，并且支持高精度的 sleep
    #include <time.h>   // 包含 time.h 文件，用于调用 nanosleep 函数
    void sleep(int sec);  // 只在这里声明 sleep 函数，具体的实现在 sleep.c 文件中
#else  // 如果是在其他平台
    #include <unistd.h> // 包含 unistd.h 文件，用于调用 sleep 函数
#endif

#endif // 结束 #ifndef 指令