#ifndef TABLE_H
#define TABLE_H

#include "dish.h"
#define MAX_TABLES 100

typedef struct {
    int tableNumber;
    int peopleNumber;  // 添加peopleNumber字段
    double totalAmount;
    Dish dishes[MAX_DISHES];
    int numDishes;
} Table;

extern Table tables[MAX_TABLES];
extern int numTables;

#endif