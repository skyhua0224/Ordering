#ifndef ORDER_H
#define ORDER_H

void addOrder();
void cancelOrder();
void placeOrder(int tableNumber, int peopleNumber);
void checkout(int orderCount, double totalAmount);
void inputTableNumber();  // 在这里声明inputTableNumber函数

#endif