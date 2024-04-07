#ifndef DISH_H  // 防止头文件被重复包含
#define DISH_H

#define MAX_DISHES 100  // 定义最大菜品数量
#define MAX_CATEGORIES 10  // 定义最大菜品类别数量

// 定义菜品结构体
typedef struct {
    int stock;  // 库存
    double price;  // 价格
    int orderCount;  // 订单数量
    char name[50];  // 菜品名称
    char category[50];  // 菜品类别
    double totalPrice;  // 总价格
} Dish;

extern Dish dishes[MAX_DISHES];  // 声明全局菜品数组
extern int numDishes;  // 声明全局菜品数量
extern int numOrders;  // 声明全局订单数量

extern char categories[MAX_CATEGORIES][50];  // 声明全局菜品类别数组
extern int numCategories;  // 声明全局菜品类别数量

// 声明函数
void loadDishes(const char* filename);  // 从文件加载菜品
void saveDishes(const char* filename);  // 保存菜品到文件
int selectDishByCategory(const char* category);  // 根据类别选择菜品
int isHotDish(Dish dish);  // 判断是否为热销菜品
void handleDishSelectionByCategory(const char* category);  // 处理类别下的菜品选择
void displayHotDishes();  // 显示热销菜品

#endif  // 结束防止头文件被重复包含