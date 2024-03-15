#include "order.h"
#include "color.h"
#include "dish.h"
#include "main_menu.h"
#include "order_info.h"
#include "payment.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "payment.h"
#include "table.h"

void addOrder() {
  // 在这里实现加单功能
}

void cancelOrder() {
  // 在这里实现撤单功能
}
void placeOrder(int tableNumber, int peopleNumber) {
  // 首先，我们需要从dish_info.txt文件中加载菜品信息
  loadDishes("dish_info.txt");

  // 从order_info.txt文件中加载订单信息
  int orderCount = 0;            // 在这里声明orderCount变量
  double totalAmount = 0;        // 在这里声明totalAmount变量
  int currentTableNumber = 0;    // 在这里声明currentTableNumber变量
  int currentPeopleNumber = 0;   // 在这里声明currentPeopleNumber变量
  int currentOrderCount = 0;     // 在这里声明currentOrderCount变量
  double currentTotalAmount = 0; // 在这里声明currentTotalAmount变量
  FILE *orderInfoFile = fopen("order_info.txt", "r");
  if (orderInfoFile != NULL) {
    while (fscanf(orderInfoFile, "%d %d %d %lf", &currentTableNumber,
                  &currentPeopleNumber, &currentOrderCount,
                  &currentTotalAmount) == 4) {
      if (currentTableNumber == tableNumber && currentOrderCount > 0 &&
          currentTotalAmount > 0) {
        // 如果已经有当前桌号的订单信息，并且菜的数量和金额不为0，那么跳过点菜过程，直接进入已下单过程
        checkout(currentTableNumber, currentPeopleNumber, currentOrderCount,
                 currentTotalAmount);
        fclose(orderInfoFile);
        return;
      }
    }
    fclose(orderInfoFile);
  }

  int dishIndex = 0; // 在这里声明dishIndex变量

  while (1) { // 添加一个循环来让用户继续点菜，直到用户选择退出
    // 显示点了几道菜和目前的金额
    printf("\e[1;1H\e[2J");
    printf(GRN "桌号%d，您已点了%d道菜，目前的金额为%.2lf元\n" RESET,
           tableNumber, orderCount, totalAmount);

    // 然后，我们可以显示所有的菜品类别，让用户选择一个类别
    printf("\n********** 菜品类别 **********\n");
    for (int i = 0; i < numCategories; i++) {
      printf("%d. %s\n", i + 1, categories[i]);
    }

    printf("请输入要点的菜品类别的编号（输入0下单，输入-1删除已点菜品，输入-"
           "2退出点菜）：");
    int categoryChoice;
    scanf("%d", &categoryChoice);

    if (categoryChoice == -2) {
      break; // 当用户输入-2时，退出点菜
    }

    if (categoryChoice == 0) {
      // 在下单时，将订单信息保存到order_info.txt文件中
      orderInfoFile =
          fopen("order_info.txt",
                "a"); // 使用"a"模式打开文件，以便在文件末尾添加新的订单信息
      fprintf(orderInfoFile, "\n%d %d %d %.2lf", tableNumber, peopleNumber,
              orderCount, totalAmount); // 在新的一行中写入订单信息
      fclose(orderInfoFile);

      checkout(tableNumber, peopleNumber, orderCount, totalAmount);
    }

  if (categoryChoice == -1) {
    // 从文件中读取已点的菜品
    char filename[20];
    sprintf(filename, "table_%d.txt", tableNumber);
    FILE *tableFile = fopen(filename, "r");
    if (tableFile != NULL) {
      char buffer[1024];
      fgets(buffer, sizeof(buffer), tableFile);
      fclose(tableFile);

      // 分割字符串并显示所有已点的菜品
      printf("\n********** 已点菜品 **********\n");
      char *dishName = strtok(buffer, " ");
      int dishIndex = 0;
      while (dishName != NULL) {
        if (strlen(dishName) > 0) {  // 只有当菜品名称不为空时，才显示
          printf("%d. %s\n", dishIndex + 1, dishName);
          dishIndex++;
        }
        dishName = strtok(NULL, " ");
        if (dishIndex >= orderCount) {
          break;
        }
      }

      // 让用户选择一个已点的菜品来删除
      printf("请输入要删除的菜品的编号（按0返回）：");
      int dishChoice;
      scanf("%d", &dishChoice);
      dishChoice--; // 转换为数组索引

      if (dishChoice >= 0 && dishChoice < orderCount) {
        // 重新从文件中读取已点的菜品
        tableFile = fopen(filename, "r");
        fgets(buffer, sizeof(buffer), tableFile);
        fclose(tableFile);

        // 找到要删除的菜品名称
        dishName = strtok(buffer, " ");
        char *dishToDelete = NULL;
        for (int i = 0; i <= dishChoice; i++) {
          dishToDelete = dishName;
          dishName = strtok(NULL, " ");
        }

        // 删除table_桌号.txt文件中的要删除的上述要删除的菜品
        FILE *tempFile = fopen("temp.txt", "w");
        tableFile = fopen(filename, "r");
        int deleted = 0; // 将标志的声明移出循环
        if (tableFile != NULL && tempFile != NULL) {
          while (fgets(buffer, sizeof(buffer), tableFile) != NULL) {
            char *line = strtok(buffer, " ");
            while (line != NULL) {
              if (strcmp(line, dishToDelete) != 0 || deleted) {
                fprintf(tempFile, "%s ", line);
              } else {
                int quantity = atoi(strtok(NULL, " "));
                double price = atof(strtok(NULL, " "));
                double total = atof(strtok(NULL, " "));
                quantity--;
                total -= price;
                if (quantity > 0) {
                  fprintf(tempFile, "%s %d %.2lf %.2lf\n", line, quantity, price, total);
                }
                deleted = 1; // 当删除一个菜品后，将标志设为1
              }
              line = strtok(NULL, " ");
            }
            fprintf(tempFile, "\n");
          }
          fclose(tableFile);
          fclose(tempFile);
          remove(filename);
          rename("temp.txt", filename);
          orderCount--;
        }

        // 找到对应的菜品价格，并从总金额中减去该价格
        FILE *dishInfoFile = fopen("dish_info.txt", "r");
        if (dishInfoFile != NULL) {
          char dishBuffer[1024];
          while (fgets(dishBuffer, sizeof(dishBuffer), dishInfoFile) != NULL) {
            char *dishNameFromFile = strtok(dishBuffer, " ");
            if (strcmp(dishNameFromFile, dishToDelete) == 0) {
              strtok(NULL, " "); // 跳过菜品的库存
              double dishPrice = atof(strtok(NULL, " ")); // 获取菜品的价格
              totalAmount -= dishPrice;
              break;
            }
          }
          fclose(dishInfoFile);
        }

        printf(GRN "已删除菜品：%s\n" RESET, dishToDelete);
      } else {
        printf(RED "无效的选项，请重新选择\n" RESET);
      }
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
    orderCount++;                           // 在这里将orderCount变量加1
    totalAmount += dishes[dishIndex].price; // 在这里将totalAmount变量增加

    // 新建文件并写入菜品信息
    char filename[20];
    sprintf(filename, "table_%d.txt", tableNumber);
    FILE *tableFile = fopen(filename, "a+");
    if (tableFile != NULL) {
      fclose(tableFile);
    }

    tableFile = fopen(filename, "r");
    char buffer[1024];
    int found = 0;
    if (tableFile != NULL) {
      while (fgets(buffer, sizeof(buffer), tableFile) != NULL) {
        char *dishName = strtok(buffer, " ");
        if (strcmp(dishName, dishes[dishIndex].name) == 0) {
          int quantity = atoi(strtok(NULL, " "));
          double price = atof(strtok(NULL, " "));
          double total = atof(strtok(NULL, " "));
          quantity++;
          total += price;
          found = 1;
          break;
        }
      }
      fclose(tableFile);
    }

    if (found) {
      // 如果找到了菜品，我们需要更新这个菜品的数量和总价
      // 首先，我们需要将文件中的所有内容读取到一个字符串数组中
      char lines[1024][1024];
      int lineCount = 0;
      tableFile = fopen(filename, "r");
      if (tableFile != NULL) {
        while (fgets(lines[lineCount], sizeof(lines[lineCount]), tableFile) != NULL) {
          lines[lineCount][strlen(lines[lineCount]) - 1] = '\0'; // 去掉换行符
          lineCount++;
        }
        fclose(tableFile);
      }

      // 然后，我们需要更新找到的菜品的数量和总价
      for (int i = 0; i < lineCount; i++) {
        char *line = strdup(lines[i]); // 复制当前行，以便我们可以使用strtok函数
        char *dishName = strtok(line, " ");
        if (strcmp(dishName, dishes[dishIndex].name) == 0) {
          int quantity = atoi(strtok(NULL, " "));
          double price = atof(strtok(NULL, " "));
          double total = atof(strtok(NULL, " "));
          quantity++;
          total += price;
          sprintf(lines[i], "%s %d %.2lf %.2lf", dishName, quantity, price, total);
          free(line);
          break;
        }
        free(line);
      }

      // 最后，我们需要将更新后的内容写回到文件中
      tableFile = fopen(filename, "w");
      if (tableFile != NULL) {
        for (int i = 0; i < lineCount; i++) {
          fprintf(tableFile, "%s\n", lines[i]); // 添加换行符
        }
        fclose(tableFile);
      }
    } else {
      // 如果没有找到菜品，我们需要在文件末尾添加新的一行
      tableFile = fopen(filename, "a");
      if (tableFile != NULL) {
        fprintf(tableFile, "%s 1 %.2lf %.2lf\n", dishes[dishIndex].name, dishes[dishIndex].price, dishes[dishIndex].price);
        fclose(tableFile);
      }
    }

    // 保存更新后的菜品信息到dish_info.txt文件中
    // saveOrderInfo(tableNumber, peopleNumber, orderCount, totalAmount);
  }
}

void checkout(int tableNumber, int peopleNumber, int orderCount,
              double totalAmount) {
    printf("\e[1;1H\e[2J");
    printf(GRN "已下单！您已点了%d道菜，目前的金额为%.2lf元\n" RESET, orderCount,
           totalAmount);

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
            mainMenu();
            return;
        case 1:
            placeOrder(tableNumber, peopleNumber);
            return;
        case 2:
            // 在这里实现查看餐品状态的功能
            break;
        case 3:
            processPayment(tableNumber, totalAmount);
            askPrintReceipt(tableNumber, totalAmount, dishes);  // 假设 dishes 是你的菜品数组
            return;
        default:
            printf(RED "无效的选项，请重新选择\n" RESET);
            break;
        }
    }
}