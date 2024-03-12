#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"  

Table* getTable(int tableNumber) {
    // 假设tables是一个全局变量，它是一个包含所有Table结构体的数组
    for (int i = 0; i < numTables; i++) {
        if (tables[i].tableNumber == tableNumber) {
            return &tables[i];
        }
    }
    return NULL;  // 如果没有找到对应的桌号，返回NULL
}