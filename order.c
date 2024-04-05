#include "order.h"
#include "color.h"
#include "dish.h"
#include "main_menu.h"
#include "order_info.h"
#include "payment.h"
#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void addOrder(){

}
void cancelOrder(){

}
void placeOrder(int tableNumber, int peopleNumber, int isAddDish,int ifpreorder) {
  // 加载菜品信息
  loadDishes("dish_info.txt");

  // 从order_info.txt文件中加载订单信息
  int orderCount = 0;
  double totalAmount = 0;
  int currentTableNumber = 0;
  int currentPeopleNumber = 0;
  int currentOrderCount = 0;
  double currentTotalAmount = 0;
  int paid = 0;
  char orderTime[64];
  FILE *orderInfoFile = fopen("order_info.txt", "r");
  if (orderInfoFile != NULL) {
    while (fscanf(orderInfoFile, "%d %d %d %lf %d %s", &currentTableNumber,
                  &currentPeopleNumber, &currentOrderCount, &currentTotalAmount,
                  &paid, orderTime) == 6) {
      if (currentTableNumber == tableNumber && currentOrderCount > 0 &&
          currentTotalAmount > 0) {
        if (!isAddDish && paid == 0) {
          // 如果已经有当前桌号的订单信息，并且菜的数量和金额不为0，且未支付，那么跳过点菜过程，直接进入已下单过程
          checkout(currentTableNumber, currentPeopleNumber, currentOrderCount,
                   currentTotalAmount,0);
          fclose(orderInfoFile);
          return;
        } else if (isAddDish) {
          // 如果是加菜操作，设置orderCount和totalAmount的值
          orderCount = currentOrderCount;
          totalAmount = currentTotalAmount;
        }
      }
    }
    fclose(orderInfoFile);
  }

  int dishIndex = 0;

  while (1) {
    printf(CLEAR_SCREEN_ANSI);
    printf(BWHT "********** 桌号%d，您已点了%d道菜，目前的金额为%.2lf元 "
                "**********\n" RESET,
           tableNumber, orderCount, totalAmount);

    printf("\n" BYEL "********** 菜品类别 **********\n" RESET);
    for (int i = 0; i < numCategories; i++) {
      printf(BGRN "%d. %s\n" RESET, i + 1, categories[i]);
    }

    printf(BBLU "请输入要点的菜品类别的编号（输入0下单，输入-"
                "1删除已点菜品，输入-2退出点菜）：" RESET);
    int categoryChoice;
    scanf("%d", &categoryChoice);

    if (categoryChoice == -2) {
      // 当用户输入-2时，退出点菜
      char filename[20];
      sprintf(filename, "table_%d.txt", tableNumber);
      FILE *tableFile = fopen(filename, "r");
      if (tableFile != NULL) {
        // 如果文件存在，关闭文件并删除它
        fclose(tableFile);
        remove(filename);
      }
      break;
    }

    if (categoryChoice == 0) {
      // 在下单时，先从order_info.txt文件中删除旧的订单信息
      FILE *tempFile = fopen("temp.txt", "w");
      orderInfoFile = fopen("order_info.txt", "r");
      if (orderInfoFile != NULL && tempFile != NULL) {
        while (fscanf(orderInfoFile, "%d %d %d %lf %d %s", &currentTableNumber,
                      &currentPeopleNumber, &currentOrderCount, &currentTotalAmount,
                      &paid, orderTime) == 6) {
          if (currentTableNumber != tableNumber || paid != 0) {
            // 如果不是当前桌号的订单，或者已经支付，那么将这一行写入新的文件中
            fprintf(tempFile, "%d %d %d %.2lf %d %s\n", currentTableNumber,
                    currentPeopleNumber, currentOrderCount, currentTotalAmount, paid,
                    orderTime);
          }
        }
        fclose(orderInfoFile);
        fclose(tempFile);

        // 删除旧的文件，并将新的文件重命名为order_info.txt
        remove("order_info.txt");
        rename("temp.txt", "order_info.txt");
      }

      // 然后，将新的订单信息保存到order_info.txt文件中
      orderInfoFile =
          fopen("order_info.txt",
                "a"); // 使用"a"模式打开文件，以便在文件末尾添加新的订单信息

      // 获取当前时间
      time_t t = time(NULL);
      struct tm *tm = localtime(&t);
      char timeStr[64];
      strftime(timeStr, sizeof(timeStr), "%Y-%m-%d_%H:%M:%S", tm);

      fprintf(orderInfoFile, "\n%d %d %d %.2lf %d %s", tableNumber,
              peopleNumber, orderCount, totalAmount, 0,
              timeStr); // 在新的一行中写入订单信息，其中0表示未支付
      fclose(orderInfoFile);

      checkout(tableNumber, peopleNumber, orderCount, totalAmount,ifpreorder);
    }

    if (categoryChoice == -1) {
      // 从文件中读取已点的菜品
      char filename[20];
      sprintf(filename, "table_%d.txt", tableNumber);
      FILE *tableFile = fopen(filename, "r");
      if (tableFile != NULL) {
        char buffer[1024];
        char lines[1024][1024];
        int lineCount = 0;
        while (fgets(buffer, sizeof(buffer), tableFile) != NULL) {
          buffer[strcspn(buffer, "\n")] = 0; // 去掉换行符
          strcpy(lines[lineCount], buffer);
          lineCount++;
        }
        fclose(tableFile);

        // 显示所有已点的菜品
        printf("\n" BYEL "********** 已点菜品 **********\n" RESET);
        for (int i = 0; i < lineCount; i++) {
          char *line = strdup(lines[i]);
          if (line == NULL) {
            printf(RED "内存不足，无法删除菜品\n" RESET);
            continue;
          }
          char *dishName = strtok(line, " ");
          int quantity = atoi(strtok(NULL, " "));
          printf(BGRN "%d. %s x%d\n" RESET, i + 1, dishName, quantity);
          free(line);
        }

        // 让用户选择一个已点的菜品来删除
        printf(BBLU "请输入要删除的菜品的编号（按0返回）：" RESET);
        int dishChoice;
        scanf("%d", &dishChoice);
        dishChoice--; // 转换为数组索引

        if (dishChoice >= 0 && dishChoice < lineCount) {
          // 找到要删除的菜品名称
          char *line = strdup(lines[dishChoice]);
          if (line == NULL) {
            printf(RED "内存不足，无法删除菜品\n" RESET);
            continue;
          }
          char *dishToDelete = strdup(strtok(line, " "));
          int quantity = atoi(strtok(NULL, " "));
          double price = atof(strtok(NULL, " "));
          double total = atof(strtok(NULL, " "));
          free(line);

          if (quantity > 1) {
            // 如果菜品数量大于1，只删除一个
            quantity--;
            total -= price;
            sprintf(lines[dishChoice], "%s %d %.2lf %.2lf", dishToDelete,
                    quantity, price, total);
          } else {
            // 如果菜品数量等于1，删除整行
            for (int i = dishChoice; i < lineCount - 1; i++) {
              strcpy(lines[i], lines[i + 1]);
            }
            lineCount--;
          }
          free(dishToDelete);

          // 将更新后的内容写回到文件中
          tableFile = fopen(filename, "w");
          if (tableFile != NULL) {
            for (int i = 0; i < lineCount; i++) {
              // 在写入文件之前，先检查你要写入的数据是否正确
              if (strstr(lines[i], "�") == NULL) {
                fprintf(tableFile, "%s\n", lines[i]); // 添加换行符
              }
            }
            fclose(tableFile);
          }

          // 找到对应的菜品价格，并从总金额中减去该价格
          totalAmount -= price;
          orderCount--; // 在这里将orderCount变量减1

          printf(GRN "已删除菜品：%s\n" RESET, dishToDelete);
        } else {
          printf(RED "无效的选项，请重新选择\n" RESET);
        }
      }
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
        while (fgets(lines[lineCount], sizeof(lines[lineCount]), tableFile) !=
               NULL) {
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
          sprintf(lines[i], "%s %d %.2lf %.2lf", dishName, quantity, price,
                  total);
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
        fprintf(tableFile, "%s 1 %.2lf %.2lf\n", dishes[dishIndex].name,
                dishes[dishIndex].price, dishes[dishIndex].price);
        fclose(tableFile);
      }
    }
  }
}

void checkout(int tableNumber, int peopleNumber, int orderCount,
              double totalAmount,int ifpreorder) {
  printf(CLEAR_SCREEN_ANSI);
  printf(BWHT "********** 已下单！您已点了%d道菜，目前的金额为%.2lf元 "
              "**********\n" RESET,
         orderCount, totalAmount);

  if(ifpreorder==1)
  return 0;

  while (1) {
    printf("\n" BYEL "********** 选项 **********\n" RESET);
    printf(BGRN "1. 加菜\n" RESET);
    printf(BGRN "2. 查看餐品状态(TODO)\n" RESET); // TODO
    printf(BGRN "3. 去支付\n" RESET);
    printf(BBLU "请输入选项的编号（输入0退出）：" RESET);

    int choice;
    scanf("%d", &choice);

    switch (choice) {
    case 0:
      mainMenu();
      return;
    case 1:
      placeOrder(tableNumber, peopleNumber, 1,0);
      return;
    case 2:
      // 在这里实现查看餐品状态的功能
      break;
    case 3:
      processPayment(tableNumber, totalAmount);
      return;
    default:
      printf(RED "无效的选项，请重新选择\n" RESET);
      break;
    }
  }
}