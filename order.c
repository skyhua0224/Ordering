#include <stdio.h>
#include <stdlib.h>
#include "order.h"
#include "dish.h"
#include "color.h"
#include <string.h>

void addOrder() {
    // 在这里实现加单功能
}

void cancelOrder() {
    // 在这里实现撤单功能
}

void checkout(int orderCount, double totalAmount) {
    printf("\e[1;1H\e[2J");
    printf(GRN "已下单！您已点了%d道菜，目前的金额为%.2lf元\n" RESET, orderCount, totalAmount);

    while (1) {
        printf("\n********** 选项 **********\n");
        printf("1. 加菜\n");
        printf("2. 查看餐品状态\n");
        printf("3. 去支付\n");
        printf("请输入选项的编号（输入0退出）：");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                return;
            case 1:
                placeOrder();
                return;
            case 2:
                // 在这里实现查看餐品状态的功能
                break;
            case 3:
                // 在这里实现去支付的功能
                return;
            default:
                printf(RED "无效的选项，请重新选择\n" RESET);
                break;
        }
    }
}

void placeOrder() {
    // 首先，我们需要从dish_info.txt文件中加载菜品信息
    loadDishes("dish_info.txt");

    // 从order_info.txt文件中加载订单信息
    int tableNumber = 0;  // 在这里声明tableNumber变量
    int peopleNumber = 0;  // 在这里声明peopleNumber变量
    int orderCount = 0;  // 在这里声明orderCount变量
    double totalAmount = 0;  // 在这里声明totalAmount变量
    FILE *orderInfoFile = fopen("order_info.txt", "r");
    if (orderInfoFile != NULL) {
        fscanf(orderInfoFile, "%d %d %d %lf", &tableNumber, &peopleNumber, &orderCount, &totalAmount);
        fclose(orderInfoFile);
    }

    int dishIndex = 0;  // 在这里声明dishIndex变量

    while (1) {  // 添加一个循环来让用户继续点菜，直到用户选择退出
        // 显示点了几道菜和目前的金额
        printf("\e[1;1H\e[2J");
        printf(GRN "桌号%d，您已点了%d道菜，目前的金额为%.2lf元\n" RESET, tableNumber, orderCount, totalAmount);

        // 然后，我们可以显示所有的菜品类别，让用户选择一个类别
        printf("\n********** 菜品类别 **********\n");
        for (int i = 0; i < numCategories; i++) {
            printf("%d. %s\n", i + 1, categories[i]);
        }

        printf("请输入要点的菜品类别的编号（输入0下单，输入-1删除已点菜品，输入-2退出点菜）：");
        int categoryChoice;
        scanf("%d", &categoryChoice);

        if (categoryChoice == -2) {
            break;  // 当用户输入-2时，退出点菜
        }

        if (categoryChoice == 0) {
            // 在下单时，将订单信息保存到order_info.txt文件中
            orderInfoFile = fopen("order_info.txt", "w");
            fprintf(orderInfoFile, "%d %d %d %.2lf", tableNumber, peopleNumber, orderCount, totalAmount);
            fclose(orderInfoFile);

            checkout(orderCount, totalAmount);  // 当用户输入0时，调用checkout函数进行下单
            break;
        }

        if (categoryChoice == -1) {
            // 显示所有已点的菜品
            printf("\n********** 已点菜品 **********\n");
            int dishCount = 0;
            for (int i = 0; i < numDishes; i++) {
                if (dishes[i].orderCount > 0) {
                    printf("%d. %s\n", dishCount + 1, dishes[i].name);
                    dishCount++;
                }
            }

            // 让用户选择一个已点的菜品来删除
            printf("请输入要删除的菜品的编号：");
            scanf("%d", &dishIndex);
            dishIndex--;  // 转换为数组索引

            if (dishIndex >= 0 && dishIndex < dishCount) {
                for (int i = 0; i < numDishes; i++) {
                    if (dishIndex < dishes[i].orderCount) {
                        dishes[i].stock++;
                        dishes[i].orderCount--;
                        orderCount--;
                        totalAmount -= dishes[i].price;  // 在这里将totalAmount变量减少
                        // 保存更新后的菜品信息到dish_info.txt文件中
                        saveDishes("dish_info.txt");
                        break;
                    } else {
                        dishIndex -= dishes[i].orderCount;
                    }
                }
            } else {
                printf(RED "无效的选项，请重新选择\n" RESET);
            }
            continue;
        }

        if (categoryChoice < 1 || categoryChoice > numCategories) {
            printf(RED "无效的选项，请重新选择\n" RESET);
            continue;
        }

        // 让用户选择一个菜品
        dishIndex = selectDishByCategory(categories[categoryChoice - 1]);
        if (dishIndex == -1) {
            continue;
        }

        // 在这里，我们添加代码来处理用户的点单请求
        // 减少所选菜品的库存，增加所选菜品的订单数量
        dishes[dishIndex].stock--;
        dishes[dishIndex].orderCount++;
        orderCount++;  // 在这里将orderCount变量加1
        totalAmount += dishes[dishIndex].price;  // 在这里将totalAmount变量增加

        // 保存更新后的菜品信息到dish_info.txt文件中
        saveDishes("dish_info.txt");
    }
}