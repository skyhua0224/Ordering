#ifndef TABLE_H  // 如果没有定义 TABLE_H
#define TABLE_H  // 定义 TABLE_H

#include "dish.h"  // 包含 dish.h 文件，用于处理菜品信息
#define MAX_TABLES 100  // 定义最大桌子数量为 100

// 定义 Table 结构体，用于表示餐桌
typedef struct {
    int tableNumber;  // 桌号
    int peopleNumber;  // 人数
    double totalAmount;  // 总金额
    Dish dishes[MAX_DISHES];  // 菜品数组
    int numDishes;  // 菜品数量
} Table;

extern Table tables[MAX_TABLES];  // 声明全局变量 tables，表示所有的桌子
extern int numTables;  // 声明全局变量 numTables，表示桌子的数量

#endif // 结束 #ifndef 指令