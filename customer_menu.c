#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "order.h"
#include "color.h"
#include "order_info.h" 


void inputTableNumber() {
    // 在这里实现输入桌台人数功能
}

void payment() {
    // 在这里实现支付功能
}

void customerMenu() {
    printf("\e[1;1H\e[2J");
    printf("欢迎光临SkyHua Virtual 餐厅，请输入您的桌号（范围1-100）：");
    int tableNumber;
    scanf("%d", &tableNumber);

    if (tableNumber < 1 || tableNumber > 100) {
        printf(RED "无效的桌号，请重新输入\n" RESET);
        return;
    }

    printf("\e[1;1H\e[2J");
    printf(GRN "桌号%d，您好！\n" RESET, tableNumber);

    printf("请选择用餐人数：\n");
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

    placeOrder(tableNumber, peopleNumber);
}