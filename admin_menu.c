#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "dish.h"
#include "receipt.h"
#include "payment.h"
#include <unistd.h>
#include "table.h"
#include "receipt.h"
#include <time.h>

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
    int paid;
    char orderTime[64];
    while (fscanf(file, "%d %d %d %lf %d %s", &tempTableNumber, &peopleNumber, &numDishes, &totalAmount, &paid, orderTime) == 6) {
        if (tempTableNumber == tableNumber && paid == 0) {
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

            // 获取支付时间
            time_t t1 = time(NULL);
            struct tm *tm1 = localtime(&t1);
            char paymentTime1[64];
            strftime(paymentTime1, sizeof(paymentTime1), "%Y-%m-%d %H:%M:%S", tm1);

            askPrintReceipt(tableNumber, totalAmount, NULL, paymentTime1);  // 假设这个函数可以处理NULL作为dishes参数
            break;
        case 2:
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            printf(GRN "应收：" YEL "%.2lf元\n" RESET, totalAmount);
            printf(RED "请使用" BLU "银行卡" RED "支付" YEL "%.2lf元\n" RESET, totalAmount);
            sleep(5);
            printf(GRN "实收：" YEL "%.2lf元\n" RESET, totalAmount);

            // 获取支付时间
            time_t t2 = time(NULL);
            struct tm *tm2 = localtime(&t2);
            char paymentTime2[64];
            strftime(paymentTime2, sizeof(paymentTime2), "%Y-%m-%d %H:%M:%S", tm2);

            askPrintReceipt(tableNumber, totalAmount, NULL, paymentTime2);  // 假设这个函数可以处理NULL作为dishes参数
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
        printf(CYN "3. 支付系统(bug)\n" RESET);//TODO
        printf(MAG "4. 小票打印(bug)\n" RESET);//TODO
        printf(RED "5. 点菜系统(TODO)\n" RESET);//TODO
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
                printf(CYN "请输入桌号：\n" RESET);
                int tableNumber;
                scanf("%d", &tableNumber);

                // 从文件中加载桌号信息
                FILE* file = fopen("order_info.txt", "r");
                if (file == NULL) {
                    printf(RED "无法打开文件\n" RESET);
                    return;
                }

                int found = 0;
                double totalAmount;
                int tempTableNumber, peopleNumber, numDishes;
                int paid;
                char orderTime[64];
                while (fscanf(file, "%d %d %d %lf %d %s", &tempTableNumber, &peopleNumber, &numDishes, &totalAmount, &paid, orderTime) == 6) {
                    if (tempTableNumber == tableNumber && paid == 0) {
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

                printReceipt(tableNumber, totalAmount, NULL, orderTime);  // 假设这个函数可以处理NULL作为dishes参数
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