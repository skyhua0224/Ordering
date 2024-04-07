#ifndef PAYMENT_H  // 如果没有定义 PAYMENT_H
#define PAYMENT_H  // 定义 PAYMENT_H

#include "dish.h"  // 包含 dish.h 文件，用于处理菜品信息

// 声明 processPayment 函数，该函数接受两个参数：桌号和总金额，用于处理支付
void processPayment(int tableNumber, double totalAmount);

// 声明 askPrintReceipt 函数，该函数接受四个参数：桌号、总金额、菜品数组和支付时间，用于询问用户是否打印小票
void askPrintReceipt(int tableNumber, double totalAmount, Dish* dishes, char* paymentTime);

// 声明 clearInputBuffer 函数，该函数用于清空输入缓冲区
void clearInputBuffer();

#endif // 结束 #ifndef 指令