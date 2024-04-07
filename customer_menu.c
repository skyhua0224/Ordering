#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "order.h"
#include "color.h"
#include "order_info.h" 

// customerMenu函数，该函数用于处理顾客的点菜操作
void customerMenu() {
    printf("\033[H\033[J"); // 清屏
    printf("\033[1;36m********** 欢迎光临SkyHua Virtual 餐厅 **********\033[0m\n");
    printf("\033[1;32m请输入您的桌号（范围1-100）：\033[0m");
    int tableNumber;
    scanf("%d", &tableNumber);

    // 检查桌号是否在有效范围内
    if (tableNumber < 1 || tableNumber > 100) {
        printf(RED "无效的桌号，请重新输入\n" RESET);
        return;
    }

    // 从order_info.txt文件中加载订单信息
    int currentTableNumber = 0;
    int currentPeopleNumber = 0;
    int currentOrderCount = 0;
    double currentTotalAmount = 0;
    int paid = 0;
    char orderTime[64];
    FILE *orderInfoFile = fopen("order_info.txt", "r");
    if (orderInfoFile != NULL) {
        while (fscanf(orderInfoFile, "%d %d %d %lf %d %s", &currentTableNumber, &currentPeopleNumber, &currentOrderCount, &currentTotalAmount, &paid, orderTime) == 6) {
            if (currentTableNumber == tableNumber && currentOrderCount > 0 && currentTotalAmount > 0 && paid == 0) {
                // 如果已经有当前桌号的订单信息，并且菜的数量和金额不为0，且未支付，那么跳过点菜过程，直接进入已下单过程
                checkout(currentTableNumber, currentPeopleNumber, currentOrderCount, currentTotalAmount, 0);
                fclose(orderInfoFile);
                return;
            }
        }
        fclose(orderInfoFile);
    }

    printf("\033[H\033[J"); // 清屏
    printf(GRN "桌号%d，您好！\n" RESET, tableNumber);

    printf("\033[1;32m请选择用餐人数：\033[0m\n");
    int peopleNumber = 0;
    if (tableNumber >= 1 && tableNumber <= 60) {
        printf("1. 1人\n");
        printf("2. 2人\n");
        printf("3. 3人\n");
        printf("4. 4人\n");
        scanf("%d", &peopleNumber);
        peopleNumber = peopleNumber <= 4 ? peopleNumber : 4;
    } else if (tableNumber >= 61 && tableNumber <= 90) {
        printf("1. 5人\n");
        printf("2. 6人\n");
        printf("3. 7人\n");
        printf("4. 8人\n");
        scanf("%d", &peopleNumber);
        peopleNumber = peopleNumber <= 4 ? peopleNumber + 4 : 8;
    } else if (tableNumber >= 91 && tableNumber <= 100) {
        printf("1. 9人\n");
        printf("2. 10人\n");
        printf("3. 11人\n");
        printf("4. 12人\n");
        scanf("%d", &peopleNumber);
        peopleNumber = peopleNumber <= 4 ? peopleNumber + 8 : 12;
    }

    // 调用placeOrder函数进行点菜操作
    placeOrder(tableNumber, peopleNumber, 0, 0);
}