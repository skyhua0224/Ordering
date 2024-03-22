#include <stdio.h>
#include "order_info.h"
#include <time.h>

void saveOrderInfo(int tableNumber, int peopleNumber, int orderCount, double totalAmount) {
    FILE *orderInfoFile = fopen("order_info.txt", "a");
    if (orderInfoFile != NULL) {
        // 获取当前时间
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char timeStr[64];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm);

        fprintf(orderInfoFile, "%d %d %d %.2lf %d %s\n", tableNumber, peopleNumber, orderCount, totalAmount, 0, timeStr);
        fclose(orderInfoFile);
    } else {
        printf("无法打开文件 order_info.txt\n");
    }
}