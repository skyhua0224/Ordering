#include <stdio.h>
#include "payment.h"
#include "color.h"
#include "receipt.h"
#include "sleep.h"
#include "time.h"
#include <stdlib.h>
#include <string.h>

// clearInputBuffer函数，该函数用于清空输入缓冲区
void clearInputBuffer() {
    while ((getchar()) != '\n');
}

// askPrintReceipt函数，该函数用于询问用户是否打印小票
void askPrintReceipt(int tableNumber, double totalAmount, Dish* dishes, char* paymentTime) {
    printf("是否打印小票？（1=是，0=否）：");
    int printReceiptChoice;
    scanf("%d", &printReceiptChoice);
    clearInputBuffer();  // 清空输入缓冲区

    // 如果用户选择打印小票
    if (printReceiptChoice == 1) {
        // 从 order_info.txt 文件中读取下单时间
        FILE *orderFile = fopen("order_info.txt", "r");
        char orderTime[64] = "";
        if (orderFile != NULL) {
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), orderFile) != NULL) {
                char* token = strtok(buffer, " ");
                if (token == NULL) continue;
                int currentTableNumber = atoi(token);
                if (currentTableNumber == tableNumber) {
                    for (int i = 0; i < 4; i++) {
                        token = strtok(NULL, " ");
                        if (token == NULL) break;
                    }
                    if (token != NULL) {
                        strcpy(orderTime, token);
                    }
                    break;
                }
            }
            fclose(orderFile);
        }
        // 打印小票
        printReceipt(tableNumber, totalAmount, dishes, paymentTime);
        sleep(10);

        // 在打印完小票之后，将order_info.txt文件中的对应订单信息，修改为已支付状态
        FILE *orderInfoFile = fopen("order_info.txt", "r");
        FILE *tempFile = fopen("temp.txt", "w");
        if (orderInfoFile != NULL && tempFile != NULL) {
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), orderInfoFile) != NULL) {
                char* token = strtok(buffer, " ");
                if (token == NULL) continue;
                int currentTableNumber = atoi(token);

                token = strtok(NULL, " ");
                if (token == NULL) continue;
                int currentPeopleNumber = atoi(token);

                token = strtok(NULL, " ");
                if (token == NULL) continue;
                int currentOrderCount = atoi(token);

                token = strtok(NULL, " ");
                if (token == NULL) continue;
                double currentTotalAmount = atof(token);

                token = strtok(NULL, " ");
                if (token == NULL) continue;
                int paid = atoi(token);

                token = strtok(NULL, " ");
                if (token == NULL) continue;
                char orderTime[64];
                strcpy(orderTime, token);

                // 如果当前订单是用户的订单，并且未支付，将其标记为已支付
                if (currentTableNumber == tableNumber && currentOrderCount > 0 && currentTotalAmount > 0 && paid == 0) {
                    fprintf(tempFile, "%d %d %d %.2lf %d %s", currentTableNumber, currentPeopleNumber, currentOrderCount, currentTotalAmount, 1, orderTime);
                } else {
                    fprintf(tempFile, "%d %d %d %.2lf %d %s", currentTableNumber, currentPeopleNumber, currentOrderCount, currentTotalAmount, paid, orderTime);
                }
            }
            fclose(orderInfoFile);
            fclose(tempFile);
            remove("order_info.txt");  // 删除原订单信息文件
            rename("temp.txt", "order_info.txt");  // 将临时文件重命名为订单信息文件
        } else {
            printf("无法打开文件 order_info.txt 或 temp.txt\n");
        }

        // 删除对应的 table_桌号.txt 文件
        char tableFilename[20];
        sprintf(tableFilename, "table_%d.txt", tableNumber);
        remove(tableFilename);
    }
    exit(0);  // 结束程序
}

// processPayment函数，该函数用于处理支付
void processPayment(int tableNumber, double totalAmount) {
    printf(CLEAR_SCREEN_ANSI);  // 清屏

    printf(GRN "桌号：%d\n" RESET, tableNumber);
    printf(GRN "总金额：%.2lf元\n" RESET, totalAmount);

    printf(CYN "请选择支付方式：\n" RESET);
    printf("1. " RED "现金\n" RESET);
    printf("2. " BLU "支付宝\n" RESET);
    printf("3. " YEL "微信支付\n" RESET);
    printf("4. " MAG "云闪付\n" RESET);
    printf("5. " GRA "我要吃霸王餐！\n" RESET);
    printf("6. " GRA "回到上级菜单\n" RESET);

    int choice;
    scanf("%d", &choice);
    clearInputBuffer();  // 清空输入缓冲区

    // 根据用户的选择，执行相应的操作
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
            strftime(paymentTime1, sizeof(paymentTime1), "%Y-%m-%d_%H_%M_%S", tm1);

            askPrintReceipt(tableNumber, totalAmount, dishes, paymentTime1);
            printf(RED "感谢您光临" GRN "SkyHua" YEL " Virtual" BLU " 餐厅" MAG "，期待您下次光临！\n" RESET);
            sleep(5);
            break;
        case 2:
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            printf(GRN "应收：" YEL "%.2lf元\n" RESET, totalAmount);
            printf(RED "请使用" BLU "支付宝" RED "支付" YEL "%.2lf元\n" RESET, totalAmount);
            sleep(5);
            printf(GRN "实收：" YEL "%.2lf元\n" RESET, totalAmount);

            // 获取支付时间
            time_t t2 = time(NULL);
            struct tm *tm2 = localtime(&t2);
            char paymentTime2[64];
            strftime(paymentTime2, sizeof(paymentTime2), "%Y-%m-%d_%H_%M_%S", tm2);

            askPrintReceipt(tableNumber, totalAmount, dishes, paymentTime2);
            printf(RED "感谢您光临" GRN "SkyHua" YEL " Virtual" BLU " 餐厅" MAG "，期待您下次光临！\n" RESET);
            sleep(5);
            break;
        case 3:
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            printf(GRN "应收：" YEL "%.2lf元\n" RESET, totalAmount);
            printf(RED "请使用" GRN "微信" RED "支付" YEL "%.2lf元\n" RESET, totalAmount);
            sleep(5);
            printf(GRN "实收：" YEL "%.2lf元\n" RESET, totalAmount);

            // 获取支付时间
            time_t t3 = time(NULL);
            struct tm *tm3 = localtime(&t3);
            char paymentTime3[64];
            strftime(paymentTime3, sizeof(paymentTime3), "%Y-%m-%d_%H_%M_%S", tm3);

            askPrintReceipt(tableNumber, totalAmount, dishes, paymentTime3);
            printf(RED "感谢您光临" GRN "SkyHua" YEL " Virtual" BLU " 餐厅" MAG "，期待您下次光临！\n" RESET);
            sleep(5);
            break;
        case 4:
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            printf(GRN "应收：" YEL "%.2lf元\n" RESET, totalAmount);
            printf(RED "请使用" MAG "云闪付" RED "支付" YEL "%.2lf元\n" RESET, totalAmount);
            sleep(5);
            printf(GRN "实收：" YEL "%.2lf元\n" RESET, totalAmount);

            // 获取支付时间
            time_t t4 = time(NULL);
            struct tm *tm4 = localtime(&t4);
            char paymentTime4[64];
            strftime(paymentTime4, sizeof(paymentTime4), "%Y-%m-%d_%H_%M_%S", tm4);

            askPrintReceipt(tableNumber, totalAmount, dishes, paymentTime4);
            printf(RED "感谢您光临" GRN "SkyHua" YEL " Virtual" BLU " 餐厅" MAG "，期待您下次光临！\n" RESET);
            sleep(5);
            break;
        case 5:
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            printf(RED "你想吃霸王餐？！\n" RESET);
            sleep(1);
            printf(RED "这是不可能的！\n" RESET);
            sleep(1);
            printf(RED "我们的保安正在赶来...\n" RESET);
            sleep(1);
            printf(RED "你被赶出了餐厅！\n" RESET);
            sleep(1);
            // 获取支付时间
            time_t t5 = time(NULL);
            struct tm *tm5 = localtime(&t5);
            char paymentTime5[64];
            strftime(paymentTime5, sizeof(paymentTime5), "%Y-%m-%d_%H_%M_%S", tm5);

            askPrintReceipt(tableNumber, totalAmount, dishes, paymentTime5);
            printf(RED "感谢您光临" GRN "SkyHua" YEL " Virtual" BLU " 餐厅" MAG "，期待您下次光临！\n" RESET);
            sleep(5);
            break;
            
        case 6:
            // 返回上级菜单
            return;
        default:
            printf(RED "无效的选项，请重新选择\n" RESET);
            break;
    }
}