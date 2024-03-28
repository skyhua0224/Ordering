#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "sleep.h"
#include <string.h>

#define MAX_TABLES 100

typedef enum {
    UNPAID,
    PAID,
    RESERVED
} TableStatus;

typedef struct {
    int number;
    int people;
    TableStatus status;
    double price;
} Table;

Table tables[MAX_TABLES];
int numTables = 0;

void reserveTable() {
    // 在这里实现预订桌台功能
}

void addOrder();

void cancelOrder();

void viewTableStatusAndPrice() {
    printf(CLEAR_SCREEN_ANSI);  // 清屏

    int tableNumber;
    printf(BLU "请输入要查看的桌号（按0返回）：" RESET);
    scanf("%d", &tableNumber);

    if (tableNumber == 0) {
        return;
    }

    FILE *file = fopen("order_info.txt", "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    int currentTableNumber, currentPeopleNumber, currentOrderCount, paid;
    double currentTotalAmount;
    char orderTime[20];

    int orderCount = 0;
    int found = 0;
    while (fscanf(file, "%d %d %d %lf %d %s", &currentTableNumber, &currentPeopleNumber, &currentOrderCount, &currentTotalAmount, &paid, orderTime) != EOF) {
        if (currentTableNumber == tableNumber) {
            printf("%d. %s桌号：%d, 下单时间：%s\n", ++orderCount, paid ? RED "[已支付]" RESET : BLU "[未支付]" RESET, currentTableNumber, orderTime);
            found = 1;
        }
    }

    fclose(file);

    if (!found) {
        printf(RED "未找到对应桌号，请重新输入\n" RESET);
        sleep(5);
        viewTableStatusAndPrice();
        return;
    }

    printf("请输入要查看的订单的编号（按0返回）：");
    int orderChoice;
    scanf("%d", &orderChoice);

    if (orderChoice == 0) {
        return;
    }

    file = fopen("order_info.txt", "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    orderCount = 0;
    while (fscanf(file, "%d %d %d %lf %d %s", &currentTableNumber, &currentPeopleNumber, &currentOrderCount, &currentTotalAmount, &paid, orderTime) != EOF) {
        if (currentTableNumber == tableNumber) {
            orderCount++;
            if (orderCount == orderChoice) {
                printf("桌号：%d, 下单时间：%s\n", currentTableNumber, orderTime);
                break;
            }
        }
    }

    fclose(file);

    printf("请选择一个选项：\n");
    printf(BLU "1. 查看金额\n" RESET);
    printf(GRN "2. 查看订单数\n" RESET);
    printf(CYN "3. 查看用餐人数\n" RESET);
    printf(YEL "4. 查看点单菜品\n" RESET);
    printf(MAG "5. 查看支付小票\n" RESET);
    printf(RED "6. 查看总览\n" RESET);

    int choice;
    scanf("%d", &choice);
        switch(choice) {
            case 1:
                printf("总金额：%.2lf\n", currentTotalAmount);
                sleep(5);
                printf(CLEAR_SCREEN_ANSI);  // 清屏
                break;
            case 2:
                printf("订单数：%d\n", currentOrderCount);
                sleep(5);
                printf(CLEAR_SCREEN_ANSI);  // 清屏
                break;
            case 3:
                printf("用餐人数：%d\n", currentPeopleNumber);
                sleep(5);
                printf(CLEAR_SCREEN_ANSI);  // 清屏
                break;
        case 4:
            if (paid) {
                // 已支付，从小票文件中读取菜品信息
                char receiptFileName[50];
                sprintf(receiptFileName, "receipt_%d_%s.txt", currentTableNumber, orderTime);
                FILE *receiptFile = fopen(receiptFileName, "r");
                if (receiptFile == NULL) {
                    printf("无法打开小票文件\n");
                    return;
                }
                char line[100];
                while (fgets(line, sizeof(line), receiptFile)) {
                    if (strstr(line, "菜品") != NULL) {
                        while (fgets(line, sizeof(line), receiptFile)) {
                            if (strstr(line, "合计金额") != NULL) {
                                break;
                            }
                            printf("%s", line);
                        }
                        break;
                    }
                }
                fclose(receiptFile);
            } else {
                // 未支付，从桌号文件中读取菜品信息
                char tableFileName[20];
                sprintf(tableFileName, "table_%d.txt", currentTableNumber);
                FILE *tableFile = fopen(tableFileName, "r");
                if (tableFile == NULL) {
                    printf("无法打开桌号文件\n");
                    return;
                }
                char line[100];
                while (fgets(line, sizeof(line), tableFile)) {
                    printf("%s", line);
                }
                fclose(tableFile);
            }
            sleep(5);
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            break;
            case 5: {
                // 查看支付小票
                if (!paid) {
                    printf(RED "桌台未支付，无法打印支付小票！\n" RESET);
                    sleep(5);
                    printf(CLEAR_SCREEN_ANSI);  // 清屏
                    break;
                }
                char receiptFileName[50];  // 在这里声明 receiptFileName 变量
                sprintf(receiptFileName, "receipt_%d_%s.txt", currentTableNumber, orderTime);
                FILE *receiptFile = fopen(receiptFileName, "r");
                if (receiptFile == NULL) {
                    printf("无法打开小票文件\n");
                    return;
                }
                char line[100];
                while (fgets(line, sizeof(line), receiptFile)) {
                    printf("%s", line);
                }
                fclose(receiptFile);
                sleep(5);
                printf(CLEAR_SCREEN_ANSI);  // 清屏
                break;
            }
            case 6:
                printf("桌号：%d, 人数：%d, 订单数：%d, 总金额：%.2lf, 状态：%s, 时间：%s\n", currentTableNumber, currentPeopleNumber, currentOrderCount, currentTotalAmount, paid ? "已支付" : "未支付", orderTime);
                sleep(5);
                printf(CLEAR_SCREEN_ANSI);  // 清屏
                break;
            default:
                printf("无效的选项，请重新选择\n");
        }
        viewTableStatusAndPrice();
    }

void manageTable() {
    int choice;

    while(1) {
        printf(CLEAR_SCREEN_ANSI);
        printf("\n********** " YEL "桌台管理" RESET " **********\n");
        printf(BLU "1. 预订桌台(TODO)\n" RESET);//TODO
        printf(GRN "2. 加单(TODO)\n" RESET);//TODO
        printf(CYN "3. 撤单(TODO)\n" RESET);//TODO
        printf(YEL "4. 查看桌台状态\n" RESET);
        printf(RED "5. 返回管理员菜单\n" RESET);
        printf(MAG "8. 退出系统\n" RESET);

        printf("请选择一个选项：");

        scanf("%d", &choice);

        switch(choice) {
            case 1:
                reserveTable();
                break;
            case 2:
                addOrder();
                break;
            case 3:
                cancelOrder();
                break;
            case 4:
                viewTableStatusAndPrice();
                break;
            case 5:
                return;
            case 6:
                printf(MAG "退出系统\n" RESET);
                exit(0);
            default:
                printf(RED "无效的选项，请重新选择\n" RESET);
        }
    }
}