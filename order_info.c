#include <stdio.h>
#include "order_info.h"

void saveOrderInfo(int tableNumber, int peopleNumber, int orderCount, double totalAmount) {
    FILE *orderInfoFile = fopen("order_info.txt", "a");
    if (orderInfoFile != NULL) {
        fprintf(orderInfoFile, "%d %d %d %.2lf\n", tableNumber, peopleNumber, orderCount, totalAmount);
        fclose(orderInfoFile);
    } else {
        printf("无法打开文件 order_info.txt\n");
    }
}