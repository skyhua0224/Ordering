#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "dish.h"
#include "color.h"

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
    printf("请输入新的价格：");
    double newPrice;
    scanf("%lf", &newPrice);

    dishes[dishIndex].price = newPrice;

    printf(GRN "%s的价格已经被修改为%.2lf！\n" RESET, dishes[dishIndex].name, newPrice);
    saveDishes("dish_info.txt");
}

void modifyStock(int dishIndex) {
    printf("请输入新的库存：");
    int newStock;
    scanf("%d", &newStock);

    dishes[dishIndex].stock = newStock;

    printf(GRN "%s的库存已经被修改为%d！\n" RESET, dishes[dishIndex].name, newStock);

    saveDishes("dish_info.txt");
}

void deleteDishFromList(int dishIndex) {
    printf(RED "%s已经被删除了！\n" RESET, dishes[dishIndex].name);

    // 将数组中的后续元素向前移动一位，覆盖要删除的元素
    for (int i = dishIndex; i < numDishes - 1; i++) {
        dishes[i] = dishes[i + 1];
    }

    numDishes--;  // 更新菜品数量

    saveDishes("dish_info.txt");
}

// 这个函数只负责显示指定类别的菜品，并让用户选择一个菜品
int selectDishByCategory(const char* category) {
    printf("\n********** %s **********\n", category);

    int dishIndices[MAX_DISHES];
    int numDishesInCategory = 0;

    // 为这个类别的菜品重新生成dishIndices数组
    for (int i = 0; i < numDishes; i++) {
        if (strcmp(dishes[i].category, category) == 0) {
            if (dishes[i].stock == 0) {
                printf("%d. " BLU "[沽清] " RESET "名称：%s，价格：%.2lf，库存：%d\n", numDishesInCategory + 1, dishes[i].name, dishes[i].price, dishes[i].stock);
            } else if (isHotDish(dishes[i])) {
                printf("%d. " RED "[热销] " RESET "名称：%s，价格：%.2lf，库存：%d\n", numDishesInCategory + 1, dishes[i].name, dishes[i].price, dishes[i].stock);
            } else {
                printf("%d. 名称：%s，价格：%.2lf，库存：%d\n", numDishesInCategory + 1, dishes[i].name, dishes[i].price, dishes[i].stock);
            }
            dishIndices[numDishesInCategory] = i;
            numDishesInCategory++;
        }
    }

    // 让用户选择一个菜品
    printf("请选择一个菜品：");
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

    // 让用户选择一个操作
    printf("\n请选择一个操作：\n");
    printf("1. 修改价格\n");
    printf("2. 修改库存\n");
    printf("3. 删除菜品\n");
    printf("4. 回到菜品管理菜单\n");

    int operationChoice;
    scanf("%d", &operationChoice);

    switch (operationChoice) {
        case 1:
            modifyPrice(dishIndex);
            break;
        case 2:
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
    printf("\n********** 查看菜品 **********\n");

    // 每次查看菜品信息前，都重新从文件中加载数据
    loadDishes("dish_info.txt");

    for (int i = 0; i < numCategories; i++) {
        printf("%d. %s\n", i + 1, categories[i]);
    }

    printf("请选择一个菜品类别：");
    int categoryChoice;
    scanf("%d", &categoryChoice);

    if (categoryChoice < 1 || categoryChoice > numCategories) {
        printf(RED "无效的选项，请重新选择\n" RESET);
        return;
    }

    handleDishSelectionByCategory(categories[categoryChoice - 1]);  // 修改这里
}

void addDish() {
    printf("\e[1;1H\e[2J");
    printf("\n********** 添加菜品 **********\n");

    // 每次添加菜品前，都重新从文件中加载数据
    loadDishes("dish_info.txt");

    // 显示所有的菜品类别
    for (int i = 0; i < numCategories; i++) {
        printf("%d. %s\n", i + 1, categories[i]);
    }

    Dish newDish;

    printf("请输入菜品类别的编号：");
    int categoryIndex;
    scanf("%d", &categoryIndex);

    if (categoryIndex < 1 || categoryIndex > numCategories) {
        printf(RED "无效的选项，请重新选择\n" RESET);
        return;
    }

    strcpy(newDish.category, categories[categoryIndex - 1]);

    printf("请输入菜品名称：");
    scanf("%s", newDish.name);

    printf("请输入菜品价格：");
    scanf("%lf", &newDish.price);
    printf("请输入菜品库存：");
    scanf("%d", &newDish.stock);

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

    printf(GRN "菜品已成功添加\n" RESET);
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
    loadDishes("dish_info.txt");

    printf("\n********** 删除菜品 **********\n");
    for (int i = 0; i < numCategories; i++) {
        printf("%d. %s\n", i + 1, categories[i]);
    }
    printf("\e[1;1H\e[2J");
    printf("请选择一个菜品类别：");

    int choice;
    scanf("%d", &choice);

    if (choice < 1 || choice > numCategories) {
        printf("无效的选项，请重新选择\n");
        return;
    }

    deleteDishByCategory(categories[choice - 1]);
}

void dishMenu() {
    int choice;

    while(1) {

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