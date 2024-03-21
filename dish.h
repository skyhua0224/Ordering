#ifndef DISH_H
#define DISH_H

#define MAX_DISHES 100
#define MAX_CATEGORIES 10

typedef struct {
    int stock;
    double price;
    int orderCount;
    char name[50];
    char category[50];
    double totalPrice;
} Dish;

extern Dish dishes[MAX_DISHES];
extern int numDishes;
extern int numOrders;

extern char categories[MAX_CATEGORIES][50];
extern int numCategories;

void loadDishes(const char* filename);
void saveDishes(const char* filename);
int selectDishByCategory(const char* category);  // 新增函数声明
int isHotDish(Dish dish);
void handleDishSelectionByCategory(const char* category);  // 新增函数声明

#endif