#include <stdio.h>
#include <unistd.h>

void printProgress(int percent) {
    int length = 50;  // 进度条的长度

    printf("\r[");  // \r将光标移动到行首

    // 打印进度条
    for (int i = 0; i < length; i++) {
        if (i < (percent/2)) {
            printf("=");  // 已完成的部分
        } else if (i == (percent/2)) {
            printf(">");  // 当前位置
        } else {
            printf(" ");  // 未完成的部分
        }
    }

    printf("] %d%%", percent);  // 打印百分比

    fflush(stdout);  // 刷新输出缓冲区，使进度条能立即显示
}

int main() {
    for (int i = 0; i <= 100; i++) {
        printProgress(i);
        usleep(50000);  // 暂停50毫秒
    }

    printf("\n");
    return 0;
}