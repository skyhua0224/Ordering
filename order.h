#ifndef ORDER_H  // 如果没有定义 ORDER_H
#define ORDER_H  // 定义 ORDER_H

// 声明 addOrder 函数，该函数用于添加订单
void addOrder();

// 声明 cancelOrder 函数，该函数用于取消订单
void cancelOrder();

// 声明 placeOrder 函数，该函数接受四个参数：桌号、人数、是否加菜和是否预订单
void placeOrder(int tableNumber, int peopleNumber, int isAddDish, int ifPreOrder);

// 声明 checkout 函数，该函数接受五个参数：桌号、人数、订单数、总金额和是否预订单
void checkout(int tableNumber, int peopleNumber, int orderCount, double totalAmount, int ifPreOrder);

// 声明 inputTableNumber 函数，该函数用于输入桌号
void inputTableNumber();

#endif  // 结束 #ifndef 指令