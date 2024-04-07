#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"  // 引入 table.h 文件，用于处理桌子信息

// getTable函数，该函数接受一个参数：桌号，用于获取对应的桌子
Table* getTable(int tableNumber) {
    // tables是一个全局变量，它是一个包含所有Table结构体的数组
    for (int i = 0; i < numTables; i++) {  // 遍历所有的桌子
        if (tables[i].tableNumber == tableNumber) {  // 如果找到了匹配的桌号
            return &tables[i];  // 返回该桌子的指针
        }
    }
    return NULL;  // 如果没有找到匹配的桌号，返回NULL
}