#ifndef PAYMENT_H
#define PAYMENT_H
#include "dish.h"

void processPayment(int tableNumber, double totalAmount);
void askPrintReceipt(int tableNumber, double totalAmount, Dish* dishes, char* paymentTime);
void clearInputBuffer();

#endif // PAYMENT_H