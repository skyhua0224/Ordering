#ifndef RECEIPT_H  // 如果没有定义 RECEIPT_H
#define RECEIPT_H  // 定义 RECEIPT_H

#include "dish.h"  // 包含 dish.h 文件，用于处理菜品信息
#include <stdio.h>  // 包含 stdio.h 文件，用于处理输入输出

// 声明 printReceiptDetails 函数，该函数接受七个参数：文件流、桌号、总金额、行数组、行数、下单时间和支付时间，用于打印小票详情
void printReceiptDetails(FILE* stream, int tableNumber, double totalAmount, char lines[1024][1024], int lineCount, char* orderTime, char* paymentTime);

// 声明 printReceipt 函数，该函数接受四个参数：桌号、总金额、菜品数组和支付时间，用于打印小票
void printReceipt(int tableNumber, double totalAmount, Dish* dishes, char* paymentTime);

#endif // 结束 #ifndef 指令