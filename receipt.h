#ifndef RECEIPT_H
#define RECEIPT_H
#include "dish.h"
#include <stdio.h>

void printReceiptDetails(FILE* stream, int tableNumber, double totalAmount, char lines[1024][1024], int lineCount, char* orderTime, char* paymentTime);
void printReceipt(int tableNumber, double totalAmount, Dish* dishes, char* paymentTime);

#endif // RECEIPT_H