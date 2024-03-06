#ifndef RECEIPT_H
#define RECEIPT_H

#include "dish.h"  // 假设你的菜品定义在 dish.h 文件中
#include "order.h"  // 假设你的订单定义在 order.h 文件中

void printReceipt(int tableNumber, double totalAmount, Dish* dishes);

#endif  // RECEIPT_H