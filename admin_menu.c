#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "dish.h"
#include "receipt.h"

void dishMenu(); 


void manageTable();

void paymentSystem() {
    // 在这里实现支付系统功能
}


void orderSystem() {
    // 在这里实现点菜系统功能
}

void adminMenu() {
    int choice;

    while(1) {
        // 清屏
        printf("\e[1;1H\e[2J");

        printf("\n********** " YEL "管理员菜单" RESET " **********\n");
        printf(BLU "1. 菜品信息管理\n" RESET);
        printf(GRN "2. 桌台管理\n" RESET);
        printf(CYN "3. 支付系统\n" RESET);
        printf(MAG "4. 小票打印\n" RESET);
        printf(RED "5. 点菜系统\n" RESET);
        printf("6. 返回主菜单\n");
        printf("请选择一个选项：");

        scanf("%d", &choice);

        switch(choice) {
            case 1:
                dishMenu();  // 调用dishMenu函数
                break;
            case 2:
                manageTable();
                break;
            case 3:
                paymentSystem();
                break;
            case 4:
                //printReceipt(0, NULL);
                break;
            case 5:
                orderSystem();
                break;
            case 6:
                return;
            default:
                printf(RED "无效的选项，请重新选择\n" RESET);
        }
    }
}