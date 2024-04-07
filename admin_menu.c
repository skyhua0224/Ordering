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
#include "adminOrder.h"

// dishMenu函数的声明，该函数用于显示菜品管理菜单
void dishMenu(); 

// manageTable函数的声明，该函数用于显示桌台管理菜单
void manageTable();

// paymentSystem函数，该函数用于处理支付操作
void paymentSystem() {
    // 提示用户输入桌号
    printf(CYN "请输入桌号：\n" RESET);
    int tableNumber;
    scanf("%d", &tableNumber);

    // 从文件中加载桌号信息
    FILE* file = fopen("order_info.txt", "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    // 搜索指定的桌号
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

    // 如果没有找到指定的桌号，提示错误信息并返回
    if (!found) {
        printf(RED "无效的桌号，请重新输入\n" RESET);
        sleep(5);
        return;
    }

    // 清屏
    printf(CLEAR_SCREEN_ANSI);  

    // 显示桌号和总金额
    printf(GRN "桌号：%d\n" RESET, tableNumber);
    printf(GRN "总金额：%.2lf元\n" RESET, totalAmount);

    // 提示用户选择支付方式
    printf(CYN "请选择该桌的支付方式：\n" RESET);
    printf("1. " RED "现金\n" RESET);
    printf("2. " BLU "银行卡\n" RESET);
    printf("3. " GRA "回到上级菜单\n" RESET);

    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            // 如果用户选择现金支付，显示应收和实收金额，并打印小票
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
            // 如果用户选择银行卡支付，显示应收和实收金额，并打印小票
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
            // 如果用户选择返回上级菜单，直接返回
            return;
        default:
            // 如果用户选择的不是1、2或3，显示错误消息
            printf(RED "无效的选项，请重新选择\n" RESET);
            break;
    }
}

// adminMenu函数，该函数用于显示管理员菜单，并根据用户的选择调用相应的函数
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
                // 如果用户选择1，显示菜品管理菜单
                dishMenu();  
                break;
            case 2:
                // 如果用户选择2，显示桌台管理菜单
                manageTable();
                break;
            case 3:
                // 如果用户选择3，处理支付操作
                paymentSystem();
                break;
            case 4:
                // 如果用户选择4，打印小票
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
                // 如果用户选择5，处理点菜操作
                adminOrderSystem();
                break;
            case 6:
                // 如果用户选择6，返回主菜单
                return;
            default:
                // 如果用户选择的不是1、2、3、4、5或6，显示错误消息
                printf(RED "无效的选项，请重新选择\n" RESET);
        }
    }
}