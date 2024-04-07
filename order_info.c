#include <stdio.h>
#include "order_info.h"
#include <time.h>

// saveOrderInfo函数，该函数用于保存订单信息
void saveOrderInfo(int tableNumber, int peopleNumber, int orderCount, double totalAmount) {
    // 打开订单信息文件，如果文件不存在，将创建一个新文件
    FILE *orderInfoFile = fopen("order_info.txt", "a");
    if (orderInfoFile != NULL) {
        // 获取当前时间
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char timeStr[64];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm);

        // 将订单信息写入文件
        fprintf(orderInfoFile, "%d %d %d %.2lf %d %s\n", tableNumber, peopleNumber, orderCount, totalAmount, 0, timeStr);
        // 关闭文件
        fclose(orderInfoFile);
    } else {
        // 如果无法打开文件，打印错误信息
        printf("无法打开文件 order_info.txt\n");
    }
}