#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dish.h"  // 假设你的菜品定义在 dish.h 文件中
#include "order.h"  // 假设你的订单定义在 order.h 文件中
#include "color.h"  // 假设你的颜色定义在 color.h 文件中   

void printReceiptDetails(FILE* stream, int tableNumber, double totalAmount, char lines[1024][1024], int lineCount) {
      fprintf(stream, "===================================================\n");
      fprintf(stream, "                    SkyHua Virtual                 \n");
      fprintf(stream, "===================================================\n");
      fprintf(stream, "桌号：%d\n", tableNumber);
      fprintf(stream, "---------------------------------------------------\n");
      fprintf(stream, " 菜品                单价    数量    小计\n");
      fprintf(stream, "---------------------------------------------------\n");

      for (int i = 0; i < lineCount; i++) {
            char *line = strdup(lines[i]);
            char *dishName = strtok(line, " ");
            int quantity = atoi(strtok(NULL, " "));
            double price = atof(strtok(NULL, " "));
            double subtotal = atof(strtok(NULL, " "));
            fprintf(stream, " %-20s%7.2f   %-8d%-8.2f\n",
                                    dishName, price, quantity, subtotal);
            free(line);
      }

      fprintf(stream, "---------------------------------------------------\n");
      fprintf(stream, "合计金额 ：              %.2f元\n", totalAmount);
      double paidAmount = totalAmount;  // 直接使用 totalAmount 作为实收金额
      fprintf(stream, "实收：   %.2f元\n", paidAmount);
      fprintf(stream, "找零：   %.2f元\n", totalAmount - paidAmount);
      fprintf(stream, "===================================================\n");
      fprintf(stream, "              请收好小票，钱物及时核对\n");
      fprintf(stream, "===================================================\n");
}

void printReceipt(int tableNumber) {
    char buffer[1024];
      char filename[20];
      sprintf(filename, "table_%d.txt", tableNumber);  // 生成文件名

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

            // 从order_info.txt文件中读取总金额
            FILE *orderFile = fopen("order_info.txt", "r");
            double totalAmount = 0.0;
            if (orderFile != NULL) {
                  while (fgets(buffer, sizeof(buffer), orderFile) != NULL) {
                        int currentTableNumber = atoi(strtok(buffer, " "));
                        if (currentTableNumber == tableNumber) {
                              strtok(NULL, " ");  // 跳过第二个字段
                              strtok(NULL, " ");  // 跳过第三个字段
                              totalAmount = atof(strtok(NULL, " "));  // 获取第四个字段（总金额）
                              break;
                        }
                  }
                  fclose(orderFile);
            }

            sprintf(filename, "receipt_%d.txt", tableNumber);  // 生成文件名

            FILE *receiptFile = fopen(filename, "w");
            if (receiptFile != NULL) {
                  printReceiptDetails(receiptFile, tableNumber, totalAmount, lines, lineCount);
                  fclose(receiptFile);
            } else {
                  printf("无法打开文件 %s\n", filename);
            }

            printReceiptDetails(stdout, tableNumber, totalAmount, lines, lineCount);
      } else {
            printf("无法打开文件 %s\n", filename);
      }

      // 删除订单信息
      FILE *orderFile = NULL;
      orderFile = fopen("order_info.txt", "r");
      FILE *tempFile = fopen("temp.txt", "w");
      if (orderFile != NULL && tempFile != NULL) {
            while (fgets(buffer, sizeof(buffer), orderFile) != NULL) {
                  int currentTableNumber = atoi(strtok(buffer, " "));
                  if (currentTableNumber != tableNumber) {
                        fprintf(tempFile, "%s", buffer);
                  }
            }
            fclose(orderFile);
            fclose(tempFile);
            remove("order_info.txt");
            rename("temp.txt", "order_info.txt");
      }

      // 删除桌号文件
      sprintf(filename, "table_%d.txt", tableNumber);
      remove(filename);

      // 退出程序
      exit(0);
}