#include <stdio.h>
#include <stdlib.h>
#include "dish.h"  // 假设你的菜品定义在 dish.h 文件中
#include "order.h"  // 假设你的订单定义在 order.h 文件中

void printReceiptDetails(FILE* stream, int tableNumber, double totalAmount, Dish* dishes) {
      fprintf(stream, "             SkyHua Virtual           \n");
      fprintf(stream, " 菜品            价格         数量        小计\n");

      for (int i = 0; i < numDishes; i++) {  // 假设 numDishes 是你的菜品数量
            double subtotal = dishes[i].price * dishes[i].orderCount;
            fprintf(stream, " %s            %.2f         %.2d        %.2f\n",
                        dishes[i].name, dishes[i].price, dishes[i].orderCount, subtotal);
      }

      fprintf(stream, "合计金额 ：              %.2f元\n", totalAmount);
      double paidAmount = totalAmount;  // 直接使用 totalAmount 作为实收金额
      fprintf(stream, "实收：   %.2f元\n", paidAmount);
      fprintf(stream, "找零：   %.2f元\n", totalAmount - paidAmount);
      fprintf(stream, "              请收好小票，钱物及时核对");
}

void printReceipt(int tableNumber, double totalAmount, Dish* dishes) {
      char filename[20];
      sprintf(filename, "receipt_%d.txt", tableNumber);  // 生成文件名

      FILE *receiptFILE = fopen(filename, "w");
      if (receiptFILE != NULL) {
            printReceiptDetails(receiptFILE, tableNumber, totalAmount, dishes);
            fclose(receiptFILE);
      } else {
            printf("无法打开文件 %s\n", filename);
      }

      printReceiptDetails(stdout, tableNumber, totalAmount, dishes);
}