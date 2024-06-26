#include "dish.h"
#include "color.h"
#include "payment.h"
#include "sleep.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DISHES 100 // 定义最大菜品数量为100
#define MAX_CATEGORIES 10 // 定义最大菜品类别数量为10

Dish dishes[MAX_DISHES]; // 定义一个全局的Dish数组，用于存储所有的菜品
int numDishes = 0; // 定义一个全局的变量，用于存储当前的菜品数量

char categories[MAX_CATEGORIES][50]; // 定义一个全局的二维字符数组，用于存储所有的菜品类别
int numCategories = 0; // 定义一个全局的变量，用于存储当前的菜品类别数量

int dishesLoaded = 0; // 新增的全局变量，用于检查菜品信息是否已经加载

// addCategory函数，该函数用于添加一个新的菜品类别
// 如果该类别已经存在，那么不会重复添加
void addCategory(const char *category) {
    for (int i = 0; i < numCategories; i++) {
        if (strcmp(categories[i], category) == 0) {
            return;
        }
    }

    strcpy(categories[numCategories], category);
    numCategories++;
}

// loadDishes函数，该函数用于从指定的文件中加载菜品信息
void loadDishes(const char *filename) {
    // 清空dishes数组和categories数组
    numDishes = 0;
    numCategories = 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    Dish temp;
    while (fscanf(file, "%d %lf %s %s", &temp.stock, &temp.price, temp.name,
                                temp.category) == 4) {
        dishes[numDishes] = temp;
        addCategory(temp.category);
        numDishes++;
    }

    fclose(file);
}

// saveDishes函数，该函数用于将当前的菜品信息保存到指定的文件中
void saveDishes(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    for (int i = 0; i < numDishes; i++) {
        fprintf(file, "%d %.2lf %s %s\n", dishes[i].stock, dishes[i].price,
                        dishes[i].name, dishes[i].category);
    }

    fclose(file);
}

// modifyPrice函数，该函数用于修改指定菜品的价格
void modifyPrice(int dishIndex) {
    printf(BGRN "请输入新的价格：" RESET);
    double newPrice;
    scanf("%lf", &newPrice);

    dishes[dishIndex].price = newPrice;

    printf(CLEAR_SCREEN_ANSI); // 清屏
    printf(GRN "%s" RESET "的" BYEL "价格" RESET "已经改为" BCYN "%.2lf" RESET
                         "！\n",
                 dishes[dishIndex].name, newPrice);
    sleep(5); // 显示5秒的提示信息
    saveDishes("dish_info.txt");
}

// modifyStock函数，该函数用于修改指定菜品的库存
void modifyStock(int dishIndex) {
    printf(BGRN "请输入新的库存：" RESET);
    int newStock;
    scanf("%d", &newStock);

    dishes[dishIndex].stock = newStock;

    printf(CLEAR_SCREEN_ANSI); // 清屏
    printf(GRN "%s" RESET "的" BYEL "库存" RESET "已经改为" BCYN "%d" RESET
                         "！\n",
                 dishes[dishIndex].name, newStock);
    sleep(5); // 显示5秒的提示信息
    saveDishes("dish_info.txt");
}

// deleteDishFromList函数，该函数用于从菜品列表中删除指定的菜品
void deleteDishFromList(int dishIndex) {
    printf(CLEAR_SCREEN_ANSI); // 清屏
    printf(GRN "%s" RESET "已经" BRED "删除" RESET "！\n",
                 dishes[dishIndex].name);

    // 将数组中的后续元素向前移动一位，覆盖要删除的元素
    for (int i = dishIndex; i < numDishes - 1; i++) {
        dishes[i] = dishes[i + 1];
    }

    numDishes--; // 更新菜品数量

    sleep(5); // 显示5秒的提示信息
    saveDishes("dish_info.txt");
}

// selectDishByCategory函数，该函数用于显示指定类别的菜品，并让用户选择一个菜品
int selectDishByCategory(const char *category) {
        printf("\n" BCYN "********** %s **********\n" RESET, category);

        int dishIndices[MAX_DISHES];
        int numDishesInCategory = 0;

        // 为这个类别的菜品重新生成dishIndices数组
        for (int i = 0; i < numDishes; i++) {
                if (strcmp(dishes[i].category, category) == 0) {
                        int orderCount = 200 - dishes[i].stock; // 计算点菜次数
                        if (dishes[i].stock == 0) {
                                printf(BBLU "%d. [沽清] " RESET "名称：%s，价格：%.2lf，库存：%d，点菜次数：%d\n",
                                                         numDishesInCategory + 1, dishes[i].name, dishes[i].price,
                                                         dishes[i].stock, orderCount);
                        } else if (isHotDish(dishes[i])) {
                                printf(RED "%d. [热销] " RESET "名称：%s，价格：%.2lf，库存：%d，点菜次数：%d\n",
                                                         numDishesInCategory + 1, dishes[i].name, dishes[i].price,
                                                         dishes[i].stock, orderCount);
                        } else {
                                printf("%d. 名称：%s，价格：%.2lf，库存：%d，点菜次数：%d\n", numDishesInCategory + 1,
                                                         dishes[i].name, dishes[i].price, dishes[i].stock, orderCount);
                        }
                        dishIndices[numDishesInCategory] = i;
                        numDishesInCategory++;
                }
        }

    // 让用户选择一个菜品
    printf(BGRN "请选择一个菜品：" RESET);
    int dishChoice;
    scanf("%d", &dishChoice);

    if (dishChoice < 1 || dishChoice > numDishesInCategory) {
        printf(RED "无效的选项，请重新选择\n" RESET);
        return -1;
    }

    return dishIndices[dishChoice - 1];
}

// isHotDish函数，该函数用于判断一个菜品是否是热销菜品
// 在这里，我们假设热销菜品的库存是所有菜品库存的最少的十个
int isHotDish(Dish dish) {
    int hotStockCounts[10] = {INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX,
                                                        INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX};

    for (int i = 0; i < numDishes; i++) {
        if (dishes[i].stock < hotStockCounts[9]) {
            hotStockCounts[9] = dishes[i].stock;
            // 对hotStockCounts数组进行排序，使得最小的库存总是在hotStockCounts[9]中
            for (int j = 8; j >= 0; j--) {
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
    return dish.stock <= hotStockCounts[9];
}

// displayHotDishes函数，该函数用于显示热销菜品
void displayHotDishes() {
        // 从文件中加载菜品数据
        Dish dishes[MAX_DISHES];
        int numDishes = 0;
        FILE *file = fopen("dish_info.txt", "r");
        if (file == NULL) {
                printf("无法打开文件\n");
                return;
        }
        Dish temp;
        while (fscanf(file, "%d %lf %s %s", &temp.stock, &temp.price, temp.name,
                                    temp.category) == 4) {
                dishes[numDishes] = temp;
                numDishes++;
        }
        fclose(file);

        printf("\n" BCYN "********** 热销菜品 **********\n" RESET);

        // 在这里，我们假设热销菜品的库存是所有菜品库存的最少的十个
        int hotStockCounts[10] = {INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX,
                                                            INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX};

        // 遍历所有菜品，找出库存最少的十个菜品
        for (int i = 0; i < numDishes; i++) {
                if (dishes[i].stock < hotStockCounts[9]) {
                        hotStockCounts[9] = dishes[i].stock;
                        // 对hotStockCounts数组进行排序，使得最小的库存总是在hotStockCounts[9]中
                        for (int j = 8; j >= 0; j--) {
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

        // 遍历所有菜品，打印出库存在最少的十个菜品之内的菜品
        int hotDishCount = 0;
        for (int i = 0; i < numDishes; i++) {
                if (dishes[i].stock <= hotStockCounts[9]) {
                        printf(RED "%2d. [热销] " RESET "名称：%s，价格：%.2lf，库存：%d\n",
                                                 hotDishCount + 1, dishes[i].name, dishes[i].price, dishes[i].stock);
                        hotDishCount++;
                }
        }

        if (hotDishCount == 0) {
                printf("当前没有热销菜品。\n");
        }

        printf("按0返回...\n");
        while (getchar() != '0');
}

// handleDishSelectionByCategory函数，该函数处理用户的选择和操作
void handleDishSelectionByCategory(const char *category) {
    int dishIndex = selectDishByCategory(category);
    if (dishIndex == -1) {
        return;
    }

    printf(CLEAR_SCREEN_ANSI); // 清屏

    // 让用户选择一个操作
    printf("\n\033[1;36m请选择要对\033[1;33m%s\033[1;36m的操作：\033[0m\n",
                 dishes[dishIndex].name);
    printf("\033[1;33m1. 修改价格\033[0m\n");
    printf("\033[1;33m2. 修改库存\033[0m\n");
    printf("\033[1;33m3. 删除菜品\033[0m\n");
    printf("\033[1;33m4. 回到菜品管理菜单\033[0m\n");

    int operationChoice;
    scanf("%d", &operationChoice);

    // 根据用户的选择，执行相应的操作
    switch (operationChoice) {
    case 1:
        printf("\033[1;32m请输入\033[1;33m%s\033[1;32m新的价格(原价格：\033[1;33m%."
                     "2lf\033[1;32m)：\033[0m",
                     dishes[dishIndex].name, dishes[dishIndex].price);
        modifyPrice(dishIndex);
        break;
    case 2:
        printf("\033[1;32m请输入\033[1;33m%s\033[1;32m新的库存数量(原库存：\033[1;"
                     "33m%d\033[1;32m)：\033[0m",
                     dishes[dishIndex].name, dishes[dishIndex].stock);
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

// viewDishes函数，该函数用于查看所有的菜品
void viewDishes() {
    printf(CLEAR_SCREEN_ANSI); // 清屏
    printf("\n\033[1;36m********** 查看菜品 **********\033[0m\n");

    // 每次查看菜品信息前，都重新从文件中加载数据
    loadDishes("dish_info.txt");

    // 显示所有的菜品类别
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

    // 处理用户选择的菜品类别
    handleDishSelectionByCategory(categories[categoryChoice - 1]);
}

// addDish函数，该函数用于添加新的菜品
void addDish() {
    printf(CLEAR_SCREEN_ANSI); // 清屏
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

    // 将用户选择的菜品类别复制到新菜品的类别字段
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

    // 将新菜品添加到菜品数组
    dishes[numDishes] = newDish;
    numDishes++;

    // 将新菜品的信息追加到文件中
    FILE *file = fopen("dish_info.txt", "a");
    if (file == NULL) {
        printf(RED "无法打开文件\n" RESET);
        return;
    }

    fprintf(file, "%d %.2lf %s %s\n", newDish.stock, newDish.price, newDish.name,
                    newDish.category);
    fclose(file);

    dishesLoaded = 0; // 设置菜品信息需要重新加载

    printf(GRN "已添加\033[0m\033[1;33m%s\033[0m，价格\033[1;36m%.2lf\033["
                         "0m，库存\033[1;32m%d\033[0m！\n",
                 newDish.name, newDish.price, newDish.stock);
    sleep(5); // 显示5秒的提示信息
}

// deleteDishByCategory函数，该函数用于删除指定类别的菜品
void deleteDishByCategory(const char *category) {
    printf("\n********** %s **********\n", category);
    int dishIndices[MAX_DISHES];
    int numDishesInCategory = 0;

    // 遍历所有菜品，找出指定类别的菜品
    for (int i = 0; i < numDishes; i++) {
        if (strcmp(dishes[i].category, category) == 0) {
            printf("%d. 名称：%s，价格：%.2lf，库存：%d\n", numDishesInCategory + 1,
                         dishes[i].name, dishes[i].price, dishes[i].stock);
            dishIndices[numDishesInCategory] = i;
            numDishesInCategory++;
        }
    }

    printf("\e[1;1H\e[2J");
    printf("请选择一个菜品：");

    int dishChoice;
    scanf("%d", &dishChoice);

    // 检查用户输入的菜品编号是否有效
    if (dishChoice < 1 || dishChoice > numDishesInCategory) {
        printf(RED "无效的选项，请重新选择\n" RESET);
        return;
    }

    // 删除用户选择的菜品
    deleteDishFromList(dishIndices[dishChoice - 1]);
}

// deleteDish函数，该函数用于删除菜品
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

    // 检查用户输入的菜品类别编号是否有效
    if (categoryIndex < 1 || categoryIndex > numCategories) {
        printf(RED "无效的选项，请重新选择\n" RESET);
        return;
    }

    // 显示用户选择的类别下的所有菜品
    int dishIndices[MAX_DISHES];
    int numDishesInCategory = 0;
    for (int i = 0; i < numDishes; i++) {
        if (strcmp(dishes[i].category, categories[categoryIndex - 1]) == 0) {
            printf("\033[1;34m%d. %s\033[0m\n", numDishesInCategory + 1,
                         dishes[i].name);
            dishIndices[numDishesInCategory] = i;
            numDishesInCategory++;
        }
    }

    printf("\033[1;32m请输入要删除的菜品的编号：\033[0m");
    int dishChoice;
    scanf("%d", &dishChoice);
    clearInputBuffer();

    // 检查用户输入的菜品编号是否有效
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
    FILE *file = fopen("dish_info.txt", "w");
    if (file == NULL) {
        printf(RED "无法打开文件\n" RESET);
        return;
    }

    for (int i = 0; i < numDishes; i++) {
        fprintf(file, "%d %.2lf %s %s\n", dishes[i].stock, dishes[i].price,
                        dishes[i].name, dishes[i].category);
    }
    fclose(file);

    dishesLoaded = 0; // 设置菜品信息需要重新加载

    printf(GRN "\033[1;33m%s\033[0m已经\033[1;36m删除\033[0m！\n", dishName);
    sleep(5); // 显示5秒的提示信息
}

// dishMenu函数，该函数用于显示菜品管理菜单，并处理用户的选择
void dishMenu() {
    int choice;

    while (1) {
        printf(CLEAR_SCREEN_ANSI); // 清屏
        printf("\n********** " YEL "菜品管理" RESET " **********\n");
        printf(BLU "1. 查看现有菜品\n" RESET);
        printf(YEL "2. 查看热销菜品\n" RESET);
        printf(GRN "3. 添加菜品\n" RESET);
        printf(RED "4. 删除菜品\n" RESET);
        printf(CYN "5. 返回管理员菜单\n" RESET);
        printf(MAG "6. 退出系统\n" RESET);
        printf("请选择一个选项：");

        scanf("%d", &choice);

        // 根据用户的选择，执行相应的操作
        switch (choice) {
        case 1:
            // 查看现有菜品
            viewDishes();
            break;
        case 2:
            // 查看热销菜品
            displayHotDishes();
            break;
        case 3:
            // 添加菜品
            addDish();
            break;
        case 4:
            // 删除菜品
            deleteDish();
            break;
        case 5:
            // 返回管理员菜单
            return;
        case 6:
            // 退出系统
            printf(MAG "退出系统\n" RESET);
            exit(0);
        default:
            // 如果用户输入的选项无效，显示错误信息
            printf(RED "无效的选项，请重新选择\n" RESET);
        }
    }
}