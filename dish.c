#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sleep.h"
#include "dish.h"
#include "color.h"
#include "payment.h"

#define MAX_DISHES 100
#define MAX_CATEGORIES 10

Dish dishes[MAX_DISHES];
int numDishes = 0;

char categories[MAX_CATEGORIES][50];
int numCategories = 0;

int dishesLoaded = 0;  // 新增的全局变量，用于检查菜品信息是否已经加载


void addCategory(const char* category) {
    for (int i = 0; i < numCategories; i++) {
        if (strcmp(categories[i], category) == 0) {
            return;
        }
    }

    strcpy(categories[numCategories], category);
    numCategories++;
}

void loadDishes(const char* filename) {
    // 清空dishes数组和categories数组
    numDishes = 0;
    numCategories = 0;

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    Dish temp;
    while (fscanf(file, "%d %lf %s %s", &temp.stock, &temp.price, temp.name, temp.category) == 4) {
        dishes[numDishes] = temp;
        addCategory(temp.category);
        numDishes++;
    }

    fclose(file);
}

void saveDishes(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    for (int i = 0; i < numDishes; i++) {
        fprintf(file, "%d %.2lf %s %s\n", dishes[i].stock, dishes[i].price, dishes[i].name, dishes[i].category);
    }

    fclose(file);
}

void modifyPrice(int dishIndex) {
    printf("\033[1;32m请输入新的价格：\033[0m");
    double newPrice;
    scanf("%lf", &newPrice);

    dishes[dishIndex].price = newPrice;

    printf("\033[H\033[J"); // 清屏
    printf(GRN "%s\033[0m的\033[1;33m价格\033[0m已经改为\033[1;36m%.2lf\033[0m！\n", dishes[dishIndex].name, newPrice);
    sleep(5); // 显示5秒的提示信息
    saveDishes("dish_info.txt");
}

void modifyStock(int dishIndex) {
    printf("\033[1;32m请输入新的库存：\033[0m");
    int newStock;
    scanf("%d", &newStock);

    dishes[dishIndex].stock = newStock;

    printf("\033[H\033[J"); // 清屏
    printf(GRN "%s\033[0m的\033[1;33m库存\033[0m已经改为\033[1;36m%d\033[0m！\n", dishes[dishIndex].name, newStock);
    sleep(5); // 显示5秒的提示信息
    saveDishes("dish_info.txt");
}

void deleteDishFromList(int dishIndex) {
    printf("\033[H\033[J"); // 清屏
    printf(GRN "%s\033[0m已经\033[1;36m删除\033[0m！\n", dishes[dishIndex].name);

    // 将数组中的后续元素向前移动一位，覆盖要删除的元素
    for (int i = dishIndex; i < numDishes - 1; i++) {
        dishes[i] = dishes[i + 1];
    }

    numDishes--;  // 更新菜品数量

    sleep(5); // 显示5秒的提示信息
    saveDishes("dish_info.txt");
}

// 这个函数只负责显示指定类别的菜品，并让用户选择一个菜品
int selectDishByCategory(const char* category) {
    printf("\n\033[1;36m********** %s **********\033[0m\n", category);

    int dishIndices[MAX_DISHES];
    int numDishesInCategory = 0;

    // 为这个类别的菜品重新生成dishIndices数组
    for (int i = 0; i < numDishes; i++) {
        if (strcmp(dishes[i].category, category) == 0) {
            if (dishes[i].stock == 0) {
                printf("\033[1;34m%d. " BLU "[沽清] " RESET "名称：%s，价格：%.2lf，库存：%d\033[0m\n", numDishesInCategory + 1, dishes[i].name, dishes[i].price, dishes[i].stock);
            } else if (isHotDish(dishes[i])) {
                printf("\033[1;34m%d. " RED "[热销] " RESET "名称：%s，价格：%.2lf，库存：%d\033[0m\n", numDishesInCategory + 1, dishes[i].name, dishes[i].price, dishes[i].stock);
            } else {
                printf("\033[1;34m%d. 名称：%s，价格：%.2lf，库存：%d\033[0m\n", numDishesInCategory + 1, dishes[i].name, dishes[i].price, dishes[i].stock);
            }
            dishIndices[numDishesInCategory] = i;
            numDishesInCategory++;
        }
    }

    // 让用户选择一个菜品
    printf("\033[1;32m请选择一个菜品：\033[0m");
    int dishChoice;
    scanf("%d", &dishChoice);

    if (dishChoice < 1 || dishChoice > numDishesInCategory) {
        printf(RED "无效的选项，请重新选择\n" RESET);
        return -1;
    }

    return dishIndices[dishChoice - 1];
}

int isHotDish(Dish dish) {
    // 在这里，我们假设热销菜品的库存是所有菜品库存的最少的五个
    int hotStockCounts[5] = {INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX};

    for (int i = 0; i < numDishes; i++) {
        if (dishes[i].stock < hotStockCounts[4]) {
            hotStockCounts[4] = dishes[i].stock;
            // 对hotStockCounts数组进行排序，使得最小的库存总是在hotStockCounts[4]中
            for (int j = 3; j >= 0; j--) {
                if (hotStockCounts[j] > hotStockCounts[j + 1]) {
                    int temp = hotStockCounts[j];
                    hotStockCounts[j] = hotStockCounts[j + 1];
                    hotStockCounts[j + 1] = temp;
                } else {
                    break;
                }
            }
        }
    }
    return dish.stock <= hotStockCounts[4];
}

// 这个函数处理用户的选择和操作
void handleDishSelectionByCategory(const char* category) {
    int dishIndex = selectDishByCategory(category);
    if (dishIndex == -1) {
        return;
    }

    // 清屏
    printf(CLEAR_SCREEN_ANSI); // 清屏

    // 让用户选择一个操作
    printf("\n\033[1;36m请选择要对\033[1;33m%s\033[1;36m的操作：\033[0m\n", dishes[dishIndex].name);
    printf("\033[1;33m1. 修改价格\033[0m\n");
    printf("\033[1;33m2. 修改库存\033[0m\n");
    printf("\033[1;33m3. 删除菜品\033[0m\n");
    printf("\033[1;33m4. 回到菜品管理菜单\033[0m\n");

    int operationChoice;
    scanf("%d", &operationChoice);

    switch (operationChoice) {
        case 1:
            printf("\033[1;32m请输入\033[1;33m%s\033[1;32m新的价格(原价格：\033[1;33m%.2lf\033[1;32m)：\033[0m", dishes[dishIndex].name, dishes[dishIndex].price);
            modifyPrice(dishIndex);
            break;
        case 2:
            printf("\033[1;32m请输入\033[1;33m%s\033[1;32m新的库存数量(原库存：\033[1;33m%d\033[1;32m)：\033[0m", dishes[dishIndex].name, dishes[dishIndex].stock);
            modifyStock(dishIndex);
            break;
        case 3:
            deleteDishFromList(dishIndex);
            return;
        case 4:
            return;
        default:
            printf(RED "无效的选项，请重新选择\n" RESET);
    }
}

void viewDishes() {
    printf(CLEAR_SCREEN_ANSI);  // 清屏
    printf("\n\033[1;36m********** 查看菜品 **********\033[0m\n");

    // 每次查看菜品信息前，都重新从文件中加载数据
    loadDishes("dish_info.txt");

    for (int i = 0; i < numCategories; i++) {
        printf("\033[1;33m%d. %s\033[0m\n", i + 1, categories[i]);
    }

    printf("\033[1;32m请选择一个菜品类别：\033[0m");
    int categoryChoice;
    scanf("%d", &categoryChoice);

    if (categoryChoice < 1 || categoryChoice > numCategories) {
        printf(RED "无效的选项，请重新选择\n" RESET);
        return;
    }

    handleDishSelectionByCategory(categories[categoryChoice - 1]);  // 修改这里
}

void addDish() {
    printf(CLEAR_SCREEN_ANSI);  // 清屏
    printf("\n\033[1;36m********** 添加菜品 **********\033[0m\n");

    // 每次添加菜品前，都重新从文件中加载数据
    loadDishes("dish_info.txt");

    // 显示所有的菜品类别
    for (int i = 0; i < numCategories; i++) {
        printf("\033[1;33m%d. %s\033[0m\n", i + 1, categories[i]);
    }

    Dish newDish;

    printf("\033[1;32m请输入菜品类别的编号：\033[0m");
    int categoryIndex;
    scanf("%d", &categoryIndex);
    clearInputBuffer();

    if (categoryIndex < 1 || categoryIndex > numCategories) {
        printf(RED "无效的选项，请重新选择\n" RESET);
        return;
    }

    strcpy(newDish.category, categories[categoryIndex - 1]);

    printf("\033[1;32m请输入菜品名称：\033[0m");
    scanf("%s", newDish.name);
    clearInputBuffer();

    printf("\033[1;32m请输入菜品价格：\033[0m");
    scanf("%lf", &newDish.price);
    clearInputBuffer();

    printf("\033[1;32m请输入菜品库存：\033[0m");
    scanf("%d", &newDish.stock);
    clearInputBuffer();

    dishes[numDishes] = newDish;
    numDishes++;

    FILE* file = fopen("dish_info.txt", "a");
    if (file == NULL) {
        printf(RED "无法打开文件\n" RESET);
        return;
    }

    fprintf(file, "%d %.2lf %s %s\n", newDish.stock, newDish.price, newDish.name, newDish.category);
    fclose(file);

    dishesLoaded = 0;  // 设置菜品信息需要重新加载

    printf(GRN "已添加\033[0m\033[1;33m%s\033[0m，价格\033[1;36m%.2lf\033[0m，库存\033[1;32m%d\033[0m！\n", newDish.name, newDish.price, newDish.stock);
    sleep(5); // 显示5秒的提示信息
}

void deleteDishByCategory(const char* category) {
    printf("\n********** %s **********\n", category);
    int dishIndices[MAX_DISHES];
    int numDishesInCategory = 0;
    for (int i = 0; i < numDishes; i++) {
        if (strcmp(dishes[i].category, category) == 0) {
            printf("%d. 名称：%s，价格：%.2lf，库存：%d\n", numDishesInCategory + 1, dishes[i].name, dishes[i].price, dishes[i].stock);
            dishIndices[numDishesInCategory] = i;
            numDishesInCategory++;
        }
    }
    printf("\e[1;1H\e[2J");
    printf("请选择一个菜品：");

    int dishChoice;
    scanf("%d", &dishChoice);

    if (dishChoice < 1 || dishChoice > numDishesInCategory) {
        printf(RED "无效的选项，请重新选择\n" RESET);
        return;
    }

    deleteDishFromList(dishIndices[dishChoice - 1]);
}

void deleteDish() {
    printf("\e[1;1H\e[2J");
    printf("\n\033[1;36m********** 删除菜品 **********\033[0m\n");

    // 每次删除菜品前，都重新从文件中加载数据
    loadDishes("dish_info.txt");

    // 显示所有的菜品类别
    for (int i = 0; i < numCategories; i++) {
        printf("\033[1;33m%d. %s\033[0m\n", i + 1, categories[i]);
    }

    printf("\033[1;32m请输入菜品类别的编号：\033[0m");
    int categoryIndex;
    scanf("%d", &categoryIndex);
    clearInputBuffer();

    if (categoryIndex < 1 || categoryIndex > numCategories) {
        printf(RED "无效的选项，请重新选择\n" RESET);
        return;
    }

    // 显示用户选择的类别下的所有菜品
    int dishIndices[MAX_DISHES];
    int numDishesInCategory = 0;
    for (int i = 0; i < numDishes; i++) {
        if (strcmp(dishes[i].category, categories[categoryIndex - 1]) == 0) {
            printf("\033[1;34m%d. %s\033[0m\n", numDishesInCategory + 1, dishes[i].name);
            dishIndices[numDishesInCategory] = i;
            numDishesInCategory++;
        }
    }

    printf("\033[1;32m请输入要删除的菜品的编号：\033[0m");
    int dishChoice;
    scanf("%d", &dishChoice);
    clearInputBuffer();

    if (dishChoice < 1 || dishChoice > numDishesInCategory) {
        printf(RED "无效的选项，请重新选择\n" RESET);
        return;
    }

    // 删除用户选择的菜品
    int dishIndex = dishIndices[dishChoice - 1];
    char dishName[50];
    strcpy(dishName, dishes[dishIndex].name);
    for (int i = dishIndex; i < numDishes - 1; i++) {
        dishes[i] = dishes[i + 1];
    }
    numDishes--;

    // 将更新后的菜品信息写回文件
    FILE* file = fopen("dish_info.txt", "w");
    if (file == NULL) {
        printf(RED "无法打开文件\n" RESET);
        return;
    }

    for (int i = 0; i < numDishes; i++) {
        fprintf(file, "%d %.2lf %s %s\n", dishes[i].stock, dishes[i].price, dishes[i].name, dishes[i].category);
    }
    fclose(file);

    dishesLoaded = 0;  // 设置菜品信息需要重新加载

    printf(GRN "\033[1;33m%s\033[0m已经\033[1;36m删除\033[0m！\n", dishName);
    sleep(5); // 显示5秒的提示信息
}

void dishMenu() {
    int choice;

    while(1) {
        printf(CLEAR_SCREEN_ANSI);  // 清屏
        printf("\n********** " YEL "菜品管理" RESET " **********\n");
        printf(BLU "1. 查看现有菜品\n" RESET);
        printf(GRN "2. 添加菜品\n" RESET);
        printf(RED "3. 删除菜品\n" RESET);
        printf(CYN "4. 返回管理员菜单\n" RESET);
        printf(MAG "5. 退出系统\n" RESET);
        printf("请选择一个选项：");

        scanf("%d", &choice);

        switch(choice) {
            case 1:
                viewDishes();
                break;
            case 2:
                addDish();
                break;
            case 3:
                deleteDish();
                break;
            case 4:
                return;
            case 5:
                printf(MAG "退出系统\n" RESET);
                exit(0);
            default:
                printf(RED "无效的选项，请重新选择\n" RESET);
        }
    }
}