#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "dish.h"
#include "receipt.h"
#include "payment.h"
#include <unistd.h>
#include "table.h"

void dishMenu(); 

void manageTable();

void paymentSystem() {
    printf(CYN "请输入桌号：\n" RESET);
    int tableNumber;
    scanf("%d", &tableNumber);

    // 从文件中加载桌号信息
    FILE* file = fopen("order_info.txt", "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    int found = 0;
    double totalAmount;
    int tempTableNumber, peopleNumber, numDishes;
    while (fscanf(file, "%d %d %d %lf", &tempTableNumber, &peopleNumber, &numDishes, &totalAmount) == 4) {
        if (tempTableNumber == tableNumber) {
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf(RED "无效的桌号，请重新输入\n" RESET);
        sleep(5);
        return;
    }

    printf(CLEAR_SCREEN_ANSI);  // 清屏

    printf(GRN "桌号：%d\n" RESET, tableNumber);
    printf(GRN "总金额：%.2lf元\n" RESET, totalAmount);

    printf(CYN "请选择该桌的支付方式：\n" RESET);
    printf("1. " RED "现金\n" RESET);
    printf("2. " BLU "银行卡\n" RESET);
    printf("3. " GRA "回到上级菜单\n" RESET);

    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            printf(GRN "应收：" YEL "%.2lf元\n" RESET, totalAmount);
            sleep(5);
            printf(GRN "实收：" YEL "%.2lf元\n" RESET, (double)((int)totalAmount));
            askPrintReceipt(tableNumber, totalAmount, NULL);  // 假设这个函数可以处理NULL作为dishes参数
            break;
        case 2:
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            printf(GRN "应收：" YEL "%.2lf元\n" RESET, totalAmount);
            printf(RED "请使用" BLU "银行卡" RED "支付" YEL "%.2lf元\n" RESET, totalAmount);
            sleep(5);
            printf(GRN "实收：" YEL "%.2lf元\n" RESET, totalAmount);
            askPrintReceipt(tableNumber, totalAmount, NULL);  // 假设这个函数可以处理NULL作为dishes参数
            break;
        case 3:
            // 返回上级菜单
            return;
        default:
            printf(RED "无效的选项，请重新选择\n" RESET);
            break;
    }
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