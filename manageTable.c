#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "sleep.h"
#include <string.h>
#include "dish.h"
#include "order.h"
#include <time.h>
#define MAX_TABLES 100

// 定义桌台状态枚举
typedef enum {
    Unpaid,
    Paid,
    Reserved
} TableStatus;

// 定义桌台结构体
typedef struct {
    int number;
    int people;
    TableStatus status;
    double price;
} Table;

Table tables[MAX_TABLES];  // 定义全局桌台数组
int numTables = 0;  // 定义全局桌台数量

// reserveTable函数，该函数用于预订桌台
void reserveTable() {
    printf("\e[1;1H\e[2J");
    printf("请输入您的桌号（范围1-100）：");
    int tableNumber;
    scanf("%d", &tableNumber);

    // 检查用户输入的桌号是否有效
    if (tableNumber < 1 || tableNumber > 100) {
        printf(RED "无效的桌号，请重新输入\n" RESET);
        return;
    }

    // 从order_info.txt文件中加载订单信息
    int currentTableNumber = 0;
    int currentPeopleNumber = 0;
    int currentOrderCount = 0;
    double currentTotalAmount = 0;
    int isAddDish=0;
    int paid=0;
    char orderTime[64];
    FILE *orderInfoFile = fopen("order_info.txt", "r");
    if (orderInfoFile != NULL) {
        while (fscanf(orderInfoFile, "%d %d %d %lf %d %s", &currentTableNumber,
                  &currentPeopleNumber, &currentOrderCount, &currentTotalAmount,
                  &paid, orderTime) == 6) {
            if (currentTableNumber == tableNumber && currentOrderCount > 0 && currentTotalAmount > 0) 
            {
                if (!isAddDish && paid == 0)
                { 
                // 如果已经有当前桌号的订单信息，并且菜的数量和金额不为0，那么跳过点菜过程，直接进入已下单过程
                checkout(currentTableNumber, currentPeopleNumber, currentOrderCount, currentTotalAmount, 0);
                fclose(orderInfoFile);
                return;
                }
            }
        }
        fclose(orderInfoFile);
    }

    printf("\e[1;1H\e[2J");
    printf(GRN "桌号%d，您好！\n" RESET, tableNumber);

    printf("请选择用餐人数：\n");
    int peopleNumber = 0;
    if (tableNumber >= 1 && tableNumber <= 60) {
        printf("1. 1人\n");
        printf("2. 2人\n");
        printf("3. 3人\n");
        printf("4. 4人\n");
        scanf("%d", &peopleNumber);
        peopleNumber = peopleNumber <= 4 ? peopleNumber : 4;
    } else if (tableNumber >= 61 && tableNumber <= 90) {
        printf("1. 5人\n");
        printf("2. 6人\n");
        printf("3. 7人\n");
        printf("4. 8人\n");
        scanf("%d", &peopleNumber);
        peopleNumber = peopleNumber <= 4 ? peopleNumber + 4 : 8;
    } else if (tableNumber >= 91 && tableNumber <= 100) {
        printf("1. 9人\n");
        printf("2. 10人\n");
        printf("3. 11人\n");
        printf("4. 12人\n");
        scanf("%d", &peopleNumber);
        peopleNumber = peopleNumber <= 4 ? peopleNumber + 8 : 12;
    }

    placeOrder(tableNumber, peopleNumber,0,1);
}

// addOrder函数，该函数用于添加订单
void addOrder(){
    int tablenum;
    int latablenum;
    int peoplenum;
    int dishnum;
    double totalprice;
    int status;
    char ordertime[100];
    printf("请输入桌台号：");
    scanf("%d",&tablenum);



    //检查桌台是否支付，是则返回到上一步
    FILE *orderInfoFile = fopen("order_info.txt", "r");
    int found = 0; 

    if(orderInfoFile != NULL)
    {
        //找到桌台
        while(1)
        {
            fscanf(orderInfoFile,"%d %d %d %lf %d %s",
            &latablenum,&peoplenum,&dishnum,&totalprice,&status,ordertime);
            
            if(feof(orderInfoFile)) // 判断是否读到文件末尾
            {
                break; // 跳出循环
            }

            if(latablenum == tablenum && status == 0)
            {
                found = 1; // 标记找到符合条件的桌号
                break;
            }
        }    

        fclose(orderInfoFile);

        if(!found) // 如果没有找到符合条件的桌号
        {
            return; // 返回上一步
        }
    }

    fopen("order_info.txt", "r");
    if (orderInfoFile != NULL) 
    {
        //找到桌台
        while(1)
        {
            fscanf(orderInfoFile,"%d %d %d %lf %d %s",
                &latablenum, &peoplenum, &dishnum, &totalprice, &status, ordertime);
            if(latablenum == tablenum && status == 0)
            break;
            else
            continue;
        }    
    }
    fclose(orderInfoFile);

    int dishIndex = 0;
    loadDishes("dish_info.txt");

    while (1)
    {
        printf(CLEAR_SCREEN_ANSI);
            printf(BWHT "********** 桌号%d，您已点了%d道菜，目前的金额为%.2lf元 "
            "**********\n" RESET,tablenum, dishnum, totalprice);
            printf("\n" BYEL "********** 菜品类别 **********\n" RESET);
            for (int i = 0; i < numCategories; i++) 
            {
                printf(BGRN "%d. %s\n" RESET, i + 1, categories[i]);
            }
            printf(BBLU "请输入要点的菜品类别的编号（输入0下单，输入-"
            "1删除已点菜品，输入-2退出点菜）：" RESET);
            int categoryChoice;
            scanf("%d", &categoryChoice);
                
            // 当用户输入-2时，退出点菜
            if (categoryChoice == -2) 
            {
                char filename[20];
                sprintf(filename, "table_%d.txt", tablenum);
                FILE *tableFile = fopen(filename, "r");
                if (tableFile != NULL) 
                {
                    // 如果文件存在，关闭文件并删除它
                    fclose(tableFile);
                    remove(filename);
                }
                break;
            }
                
            //当用户输入0时开始点菜
            if (categoryChoice == 0) 
            {
                // 在下单时，先从order_info.txt文件中删除旧的订单信息
                FILE *tempFile = fopen("temp.txt", "w");
                orderInfoFile = fopen("order_info.txt", "r");
                if (orderInfoFile != NULL && tempFile != NULL) 
                {
                    while (fscanf(orderInfoFile, "%d %d %d %lf %d %s", &latablenum,
                    &peoplenum, &dishnum, &totalprice,
                    &status, ordertime) == 6) 
                    {
                        if (latablenum != tablenum || status != 0) 
                        {
                            // 如果不是当前桌号的订单，或者已经支付，那么将这一行写入新的文件中
                            fprintf(tempFile, "%d %d %d %.2lf %d %s\n", latablenum,
                            peoplenum, dishnum, totalprice, status,ordertime);
                        }
                    }
                    fclose(orderInfoFile);
                    fclose(tempFile);

                    // 删除旧的文件，并将新的文件重命名为order_info.txt
                    remove("order_info.txt");
                    rename("temp.txt", "order_info.txt");
                }
                // 然后，将新的订单信息保存到order_info.txt文件中
                orderInfoFile =fopen("order_info.txt","a"); 
                // 使用"a"模式打开文件，以便在文件末尾添加新的订单信息

                // 获取当前时间
                time_t t = time(NULL);
                struct tm *tm = localtime(&t);
                char timeStr[64];
                strftime(timeStr, sizeof(timeStr), "%Y-%m-%d_%H_%M_%S", tm);

                fprintf(orderInfoFile, "\n%d %d %d %lf %d %s", tablenum,
                        peoplenum, dishnum, totalprice, 0,
                        timeStr); // 在新的一行中写入订单信息，其中0表示未支付
                fclose(orderInfoFile);

                checkout(tablenum, peoplenum, dishnum, totalprice, 0);
            }
            if (categoryChoice == -1) 
            {
            // 从文件中读取已点的菜品
            char filename[20];
            sprintf(filename, "table_%d.txt", tablenum);
            FILE *tableFile = fopen(filename, "r");
            if (tableFile != NULL) 
            {
                char buffer[1024];
                char lines[1024][1024];
                int lineCount = 0;
                while (fgets(buffer, sizeof(buffer), tableFile) != NULL) 
                {
                buffer[strcspn(buffer, "\n")] = 0; // 去掉换行符
                strcpy(lines[lineCount], buffer);
                lineCount++;
                }
                fclose(tableFile);

                // 显示所有已点的菜品
                printf("\n" BYEL "********** 已点菜品 **********\n" RESET);
                for (int i = 0; i < lineCount; i++) 
                {
                char *line = strdup(lines[i]);
                if (line == NULL) 
                {
                    printf(RED "内存不足，无法删除菜品\n" RESET);
                    continue;
                }
                char *dishName = strtok(line, " ");
                int quantity = atoi(strtok(NULL, " "));
                printf(BGRN "%d. %s x%d\n" RESET, i + 1, dishName, quantity);
                free(line);
                }

                // 让用户选择一个已点的菜品来删除
                printf(BBLU "请输入要删除的菜品的编号（按0返回）：" RESET);
                int dishChoice;
                scanf("%d", &dishChoice);
                dishChoice--; // 转换为数组索引

                if (dishChoice >= 0 && dishChoice < lineCount) 
                {
                // 找到要删除的菜品名称
                char *line = strdup(lines[dishChoice]);
                if (line == NULL) 
                {
                    printf(RED "内存不足，无法删除菜品\n" RESET);
                    continue;
                }
                char *dishToDelete = strdup(strtok(line, " "));
                int quantity = atoi(strtok(NULL, " "));
                double price = atof(strtok(NULL, " "));
                double total = atof(strtok(NULL, " "));
                free(line);

                if (quantity > 1) 
                {
                    // 如果菜品数量大于1，只删除一个
                    quantity--;
                    total -= price;
                    sprintf(lines[dishChoice], "%s %d %.2lf %.2lf", dishToDelete,
                            quantity, price, total);
                } 
                else 
                {
                    // 如果菜品数量等于1，删除整行
                    for (int i = dishChoice; i < lineCount - 1; i++) 
                    {
                    strcpy(lines[i], lines[i + 1]);
                    }
                    lineCount--;
                }
                free(dishToDelete);

                // 将更新后的内容写回到文件中
                tableFile = fopen(filename, "w");
                if (tableFile != NULL) 
                {
                    for (int i = 0; i < lineCount; i++) 
                    {
                    // 在写入文件之前，先检查你要写入的数据是否正确
                    if (strstr(lines[i], "�") == NULL) 
                    {
                        fprintf(tableFile, "%s\n", lines[i]); // 添加换行符
                    }
                    }
                    fclose(tableFile);
                }

                // 找到对应的菜品价格，并从总金额中减去该价格
                totalprice -= price;
                dishnum--; // 在这里将orderCount变量减1

                printf(GRN "已删除菜品：%s\n" RESET, dishToDelete);
                } 
                else 
                {
                printf(RED "无效的选项，请重新选择\n" RESET);
                }
            }
            continue;
            }
            // 让用户选择一个菜品
            dishIndex = selectDishByCategory(categories[categoryChoice - 1]);
            if (dishIndex == -1) 
            {
                continue;
            }

            // 在这里，我们添加代码来处理用户的点单请求
            // 减少所选菜品的库存，增加所选菜品的订单数量
            dishes[dishIndex].stock--;
            dishes[dishIndex].orderCount++;
            dishnum++;                           
            totalprice += dishes[dishIndex].price; 

            // 新建文件并写入菜品信息
            char filename[20];
            sprintf(filename, "table_%d.txt", tablenum);
            FILE *tableFile = fopen(filename, "a+");
            if (tableFile != NULL) 
            {
                fclose(tableFile);
            }

            tableFile = fopen(filename, "r");
            char buffer[1024];
            int found = 0;
            if (tableFile != NULL) 
            {
                while (fgets(buffer, sizeof(buffer), tableFile) != NULL) 
                {
                    char *dishName = strtok(buffer, " ");
                    if (strcmp(dishName, dishes[dishIndex].name) == 0) 
                    {
                        int quantity = atoi(strtok(NULL, " "));
                        double price = atof(strtok(NULL, " "));
                        double total = atof(strtok(NULL, " "));
                        quantity++;
                        total += price;
                        found = 1;
                        break;
                    }
                }
                fclose(tableFile);
            }

                if (found) 
                {
                    // 如果找到了菜品，我们需要更新这个菜品的数量和总价
                    // 首先，我们需要将文件中的所有内容读取到一个字符串数组中
                    char lines[1024][1024];
                    int lineCount = 0;
                    tableFile = fopen(filename, "r");
                    if (tableFile != NULL) 
                    {
                        while (fgets(lines[lineCount], sizeof(lines[lineCount]), tableFile) !=
                            NULL) 
                        {
                            lines[lineCount][strlen(lines[lineCount]) - 1] = '\0'; // 去掉换行符
                            lineCount++;
                        }
                        fclose(tableFile);
                    }

                    // 然后，我们需要更新找到的菜品的数量和总价
                    for (int i = 0; i < lineCount; i++) 
                    {
                        char *line = strdup(lines[i]); // 复制当前行，以便我们可以使用strtok函数
                        char *dishName = strtok(line, " ");
                        if (strcmp(dishName, dishes[dishIndex].name) == 0) 
                        {
                            int quantity = atoi(strtok(NULL, " "));
                            double price = atof(strtok(NULL, " "));
                            double total = atof(strtok(NULL, " "));
                            quantity++;
                            total += price;
                            sprintf(lines[i], "%s %d %.2lf %.2lf", dishName, quantity, price,
                                    total);
                            free(line);
                        break;
                        }
                        free(line);
                    }

                    // 最后，我们需要将更新后的内容写回到文件中
                    tableFile = fopen(filename, "w");
                    if (tableFile != NULL) 
                    {
                        for (int i = 0; i < lineCount; i++) 
                        {
                            fprintf(tableFile, "%s\n", lines[i]); // 添加换行符
                        }
                        fclose(tableFile);
                    }
                } 
                else 
                {
                    // 如果没有找到菜品，我们需要在文件末尾添加新的一行
                    tableFile = fopen(filename, "a");
                    if (tableFile != NULL) 
                    {
                        fprintf(tableFile, "%s 1 %.2lf %.2lf\n", dishes[dishIndex].name,
                                dishes[dishIndex].price, dishes[dishIndex].price);
                        fclose(tableFile);
                    }
                }
    }        
}

// cancelOrder函数，该函数用于取消订单
void cancelOrder() {
    int tableNum;
    printf("请输入桌台号：");
    scanf("%d", &tableNum);
    FILE *orderInfoFile = fopen("order_info.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    
    if(orderInfoFile != NULL && tempFile != NULL) {
        char line[100]; // 假设每行最多100个字符
        int found = 0; // 标记是否找到桌台号对应的行
        
        // 读取每一行，查找匹配的桌台号
        while(fgets(line, sizeof(line), orderInfoFile) != NULL) {
            int currentTableNum;
            // 使用 sscanf 从一行中读取桌台号
            if(sscanf(line, "%d", &currentTableNum) == 1) {
                if(currentTableNum == tableNum) {
                    found = 1;
                    continue; // 跳过这一行，相当于删除这一行
                }
            }
            // 将不是要删除的行写入临时文件
            fprintf(tempFile, "%s", line);
        }
        
        fclose(orderInfoFile);
        fclose(tempFile);
        
        if(found) { // 删除原文件并将临时文件重命名为原文件
            remove("order_info.txt"); 
            rename("temp.txt", "order_info.txt"); 
            char filename[20];
            sprintf(filename, "table_%d.txt", tableNum);
            FILE *tableFile = fopen(filename, "r");
            if (tableFile != NULL) {
                // 如果文件存在，关闭文件并删除它
                fclose(tableFile);
                remove(filename);
            }
            printf("成功撤单\n");
            sleep(5);
        } else {
            remove("temp.txt"); // 删除临时文件
            printf("未找到对应桌台号的订单\n");
        }
    } else {
        printf("无法打开文件\n");
    }
}

// viewTableStatusAndPrice函数，该函数用于查看桌台的状态和价格
void viewTableStatusAndPrice() {
    printf(CLEAR_SCREEN_ANSI);  // 清屏

    int tableNumber;
    printf(BLU "请输入要查看的桌号（按0返回）：" RESET);
    scanf("%d", &tableNumber);

    // 如果输入的桌号为0，返回上一级菜单
    if (tableNumber == 0) {
        return;
    }

    // 打开订单信息文件
    FILE *file = fopen("order_info.txt", "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    // 定义一些变量来存储订单信息
    int currentTableNumber, currentPeopleNumber, currentOrderCount, paid;
    double currentTotalAmount;
    char orderTime[20];

    int orderCount = 0;
    int found = 0;

    // 读取订单信息文件，查找匹配的桌号
    while (fscanf(file, "%d %d %d %lf %d %s", &currentTableNumber, &currentPeopleNumber, &currentOrderCount, &currentTotalAmount, &paid, orderTime) != EOF) {
        if (currentTableNumber == tableNumber) {
            printf("%d. %s桌号：%d, 下单时间：%s\n", ++orderCount, paid ? RED "[已支付]" RESET : BLU "[未支付]" RESET, currentTableNumber, orderTime);
            found = 1;
        }
    }

    fclose(file);

    // 如果没有找到匹配的桌号，提示用户重新输入
    if (!found) {
        printf(RED "未找到对应桌号，请重新输入\n" RESET);
        sleep(5);
        viewTableStatusAndPrice();
        return;
    }

    // 提示用户输入要查看的订单编号
    printf("请输入要查看的订单的编号（按0返回）：");
    int orderChoice;
    scanf("%d", &orderChoice);

    // 如果输入的订单编号为0，返回上一级菜单
    if (orderChoice == 0) {
        return;
    }

    // 重新打开订单信息文件，查找匹配的订单编号
    file = fopen("order_info.txt", "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    orderCount = 0;
    while (fscanf(file, "%d %d %d %lf %d %s", &currentTableNumber, &currentPeopleNumber, &currentOrderCount, &currentTotalAmount, &paid, orderTime) != EOF) {
        if (currentTableNumber == tableNumber) {
            orderCount++;
            if (orderCount == orderChoice) {
                printf("桌号：%d, 下单时间：%s\n", currentTableNumber, orderTime);
                break;
            }
        }
    }

    fclose(file);

    // 提示用户选择一个选项
    printf("请选择一个选项：\n");
    printf(BLU "1. 查看金额\n" RESET);
    printf(GRN "2. 查看订单数\n" RESET);
    printf(CYN "3. 查看用餐人数\n" RESET);
    printf(YEL "4. 查看点单菜品\n" RESET);
    printf(MAG "5. 查看支付小票\n" RESET);
    printf(RED "6. 查看总览\n" RESET);

    int choice;
    scanf("%d", &choice);

    // 根据用户的选择，执行相应的操作
    switch(choice) {
        case 1:
            // 用户选择查看总金额
            printf("总金额：%.2lf\n", currentTotalAmount);
            sleep(5);
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            break;
        case 2:
            // 用户选择查看订单数
            printf("订单数：%d\n", currentOrderCount);
            sleep(5);
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            break;
        case 3:
            // 用户选择查看用餐人数
            printf("用餐人数：%d\n", currentPeopleNumber);
            sleep(5);
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            break;
        case 4:
            // 用户选择查看点单菜品
            if (paid) {
                // 如果已支付，从小票文件中读取菜品信息
                char receiptFileName[50];
                sprintf(receiptFileName, "receipt_%d_%s.txt", currentTableNumber, orderTime);
                FILE *receiptFile = fopen(receiptFileName, "r");
                if (receiptFile == NULL) {
                    printf("无法打开小票文件\n");
                    return;
                }
                char line[100];
                while (fgets(line, sizeof(line), receiptFile)) {
                    if (strstr(line, "菜品") != NULL) {
                        while (fgets(line, sizeof(line), receiptFile)) {
                            if (strstr(line, "合计金额") != NULL) {
                                break;
                            }
                            printf("%s", line);
                        }
                        break;
                    }
                }
                fclose(receiptFile);
            } else {
                // 如果未支付，从桌号文件中读取菜品信息
                char tableFileName[20];
                sprintf(tableFileName, "table_%d.txt", currentTableNumber);
                FILE *tableFile = fopen(tableFileName, "r");
                if (tableFile == NULL) {
                    printf("无法打开桌号文件\n");
                    return;
                }
                char line[100];
                while (fgets(line, sizeof(line), tableFile)) {
                    printf("%s", line);
                }
                fclose(tableFile);
            }
            sleep(5);
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            break;
        case 5:
            // 用户选择查看支付小票
            if (!paid) {
                printf(RED "桌台未支付，无法打印支付小票！\n" RESET);
                sleep(5);
                printf(CLEAR_SCREEN_ANSI);  // 清屏
                break;
            }
            char receiptFileName[50];  // 在这里声明 receiptFileName 变量
            sprintf(receiptFileName, "receipt_%d_%s.txt", currentTableNumber, orderTime);
            FILE *receiptFile = fopen(receiptFileName, "r");
            if (receiptFile == NULL) {
                printf("无法打开小票文件\n");
                return;
            }
            char line[100];
            while (fgets(line, sizeof(line), receiptFile)) {
                printf("%s", line);
            }
            fclose(receiptFile);
            sleep(5);
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            break;
        case 6:
            // 用户选择查看总览
            printf("桌号：%d, 人数：%d, 订单数：%d, 总金额：%.2lf, 状态：%s, 时间：%s\n", currentTableNumber, currentPeopleNumber, currentOrderCount, currentTotalAmount, paid ? "已支付" : "未支付", orderTime);
            sleep(5);
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            break;
        default:
            // 用户输入了无效的选项
            printf("无效的选项，请重新选择\n");
    }
    // 在执行完用户选择的操作后，重新调用自身，以便用户可以继续查看其他信息或进行其他操作
    viewTableStatusAndPrice();
}

// manageTable函数，该函数用于管理桌台
void manageTable() {
    int choice;

    // 使用一个无限循环，让用户可以反复选择操作
    while(1) {
        printf(CLEAR_SCREEN_ANSI);  // 清屏
        printf("\n********** " YEL "桌台管理" RESET " **********\n");
        printf(BLU "1. 预订桌台\n" RESET);  // 提示用户可以选择预订桌台
        printf(GRN "2. 加单\n" RESET);  // 提示用户可以选择加单
        printf(CYN "3. 撤单\n" RESET);  // 提示用户可以选择撤单
        printf(YEL "4. 查看桌台状态\n" RESET);  // 提示用户可以选择查看桌台状态
        printf(RED "5. 返回管理员菜单\n" RESET);  // 提示用户可以选择返回管理员菜单
        printf(MAG "6. 退出系统\n" RESET);  // 提示用户可以选择退出系统

        printf("请选择一个选项：");

        scanf("%d", &choice);  // 读取用户的选择

        // 根据用户的选择，执行相应的操作
        switch(choice) {
            case 1:
                reserveTable();  // 用户选择预订桌台
                break;
            case 2:
                addOrder();  // 用户选择加单
                break;
            case 3:
                cancelOrder();  // 用户选择撤单
                break;
            case 4:
                viewTableStatusAndPrice();  // 用户选择查看桌台状态
                break;
            case 5:
                return;  // 用户选择返回管理员菜单，结束当前函数
            case 6:
                printf(MAG "退出系统\n" RESET);
                exit(0);  // 用户选择退出系统，结束程序
            default:
                printf(RED "无效的选项，请重新选择\n" RESET);  // 用户输入了无效的选项，提示用户重新选择
        }
    }
}#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "sleep.h"
#include <string.h>
#include "dish.h"
#include "order.h"
#include <time.h>
#define MAX_TABLES 100

// 定义桌台状态枚举
typedef enum {
    Unpaid,
    Paid,
    Reserved
} TableStatus;

// 定义桌台结构体
typedef struct {
    int number;
    int people;
    TableStatus status;
    double price;
} Table;

Table tables[MAX_TABLES];  // 定义全局桌台数组
int numTables = 0;  // 定义全局桌台数量

// reserveTable函数，该函数用于预订桌台
void reserveTable() {
    printf("\e[1;1H\e[2J");
    printf("请输入您的桌号（范围1-100）：");
    int tableNumber;
    scanf("%d", &tableNumber);

    // 检查用户输入的桌号是否有效
    if (tableNumber < 1 || tableNumber > 100) {
        printf(RED "无效的桌号，请重新输入\n" RESET);
        return;
    }

    // 从order_info.txt文件中加载订单信息
    int currentTableNumber = 0;
    int currentPeopleNumber = 0;
    int currentOrderCount = 0;
    double currentTotalAmount = 0;
    int isAddDish=0;
    int paid=0;
    char orderTime[64];
    FILE *orderInfoFile = fopen("order_info.txt", "r");
    if (orderInfoFile != NULL) {
        while (fscanf(orderInfoFile, "%d %d %d %lf %d %s", &currentTableNumber,
                  &currentPeopleNumber, &currentOrderCount, &currentTotalAmount,
                  &paid, orderTime) == 6) {
            if (currentTableNumber == tableNumber && currentOrderCount > 0 && currentTotalAmount > 0) 
            {
                if (!isAddDish && paid == 0)
                { 
                // 如果已经有当前桌号的订单信息，并且菜的数量和金额不为0，那么跳过点菜过程，直接进入已下单过程
                checkout(currentTableNumber, currentPeopleNumber, currentOrderCount, currentTotalAmount, 0);
                fclose(orderInfoFile);
                return;
                }
            }
        }
        fclose(orderInfoFile);
    }

    printf("\e[1;1H\e[2J");
    printf(GRN "桌号%d，您好！\n" RESET, tableNumber);

    printf("请选择用餐人数：\n");
    int peopleNumber = 0;
    if (tableNumber >= 1 && tableNumber <= 60) {
        printf("1. 1人\n");
        printf("2. 2人\n");
        printf("3. 3人\n");
        printf("4. 4人\n");
        scanf("%d", &peopleNumber);
        peopleNumber = peopleNumber <= 4 ? peopleNumber : 4;
    } else if (tableNumber >= 61 && tableNumber <= 90) {
        printf("1. 5人\n");
        printf("2. 6人\n");
        printf("3. 7人\n");
        printf("4. 8人\n");
        scanf("%d", &peopleNumber);
        peopleNumber = peopleNumber <= 4 ? peopleNumber + 4 : 8;
    } else if (tableNumber >= 91 && tableNumber <= 100) {
        printf("1. 9人\n");
        printf("2. 10人\n");
        printf("3. 11人\n");
        printf("4. 12人\n");
        scanf("%d", &peopleNumber);
        peopleNumber = peopleNumber <= 4 ? peopleNumber + 8 : 12;
    }

    placeOrder(tableNumber, peopleNumber,0,1);
}

// addOrder函数，该函数用于添加订单
void addOrder(){
    int tablenum;
    int latablenum;
    int peoplenum;
    int dishnum;
    double totalprice;
    int status;
    char ordertime[100];
    printf("请输入桌台号：");
    scanf("%d",&tablenum);



    //检查桌台是否支付，是则返回到上一步
    FILE *orderInfoFile = fopen("order_info.txt", "r");
    int found = 0; 

    if(orderInfoFile != NULL)
    {
        //找到桌台
        while(1)
        {
            fscanf(orderInfoFile,"%d %d %d %lf %d %s",
            &latablenum,&peoplenum,&dishnum,&totalprice,&status,ordertime);
            
            if(feof(orderInfoFile)) // 判断是否读到文件末尾
            {
                break; // 跳出循环
            }

            if(latablenum == tablenum && status == 0)
            {
                found = 1; // 标记找到符合条件的桌号
                break;
            }
        }    

        fclose(orderInfoFile);

        if(!found) // 如果没有找到符合条件的桌号
        {
            return; // 返回上一步
        }
    }

    fopen("order_info.txt", "r");
    if (orderInfoFile != NULL) 
    {
        //找到桌台
        while(1)
        {
            fscanf(orderInfoFile,"%d %d %d %lf %d %s",
                &latablenum, &peoplenum, &dishnum, &totalprice, &status, ordertime);
            if(latablenum == tablenum && status == 0)
            break;
            else
            continue;
        }    
    }
    fclose(orderInfoFile);

    int dishIndex = 0;
    loadDishes("dish_info.txt");

    while (1)
    {
        printf(CLEAR_SCREEN_ANSI);
            printf(BWHT "********** 桌号%d，您已点了%d道菜，目前的金额为%.2lf元 "
            "**********\n" RESET,tablenum, dishnum, totalprice);
            printf("\n" BYEL "********** 菜品类别 **********\n" RESET);
            for (int i = 0; i < numCategories; i++) 
            {
                printf(BGRN "%d. %s\n" RESET, i + 1, categories[i]);
            }
            printf(BBLU "请输入要点的菜品类别的编号（输入0下单，输入-"
            "1删除已点菜品，输入-2退出点菜）：" RESET);
            int categoryChoice;
            scanf("%d", &categoryChoice);
                
            // 当用户输入-2时，退出点菜
            if (categoryChoice == -2) 
            {
                char filename[20];
                sprintf(filename, "table_%d.txt", tablenum);
                FILE *tableFile = fopen(filename, "r");
                if (tableFile != NULL) 
                {
                    // 如果文件存在，关闭文件并删除它
                    fclose(tableFile);
                    remove(filename);
                }
                break;
            }
                
            //当用户输入0时开始点菜
            if (categoryChoice == 0) 
            {
                // 在下单时，先从order_info.txt文件中删除旧的订单信息
                FILE *tempFile = fopen("temp.txt", "w");
                orderInfoFile = fopen("order_info.txt", "r");
                if (orderInfoFile != NULL && tempFile != NULL) 
                {
                    while (fscanf(orderInfoFile, "%d %d %d %lf %d %s", &latablenum,
                    &peoplenum, &dishnum, &totalprice,
                    &status, ordertime) == 6) 
                    {
                        if (latablenum != tablenum || status != 0) 
                        {
                            // 如果不是当前桌号的订单，或者已经支付，那么将这一行写入新的文件中
                            fprintf(tempFile, "%d %d %d %.2lf %d %s\n", latablenum,
                            peoplenum, dishnum, totalprice, status,ordertime);
                        }
                    }
                    fclose(orderInfoFile);
                    fclose(tempFile);

                    // 删除旧的文件，并将新的文件重命名为order_info.txt
                    remove("order_info.txt");
                    rename("temp.txt", "order_info.txt");
                }
                // 然后，将新的订单信息保存到order_info.txt文件中
                orderInfoFile =fopen("order_info.txt","a"); 
                // 使用"a"模式打开文件，以便在文件末尾添加新的订单信息

                // 获取当前时间
                time_t t = time(NULL);
                struct tm *tm = localtime(&t);
                char timeStr[64];
                strftime(timeStr, sizeof(timeStr), "%Y-%m-%d_%H_%M_%S", tm);

                fprintf(orderInfoFile, "\n%d %d %d %lf %d %s", tablenum,
                        peoplenum, dishnum, totalprice, 0,
                        timeStr); // 在新的一行中写入订单信息，其中0表示未支付
                fclose(orderInfoFile);

                checkout(tablenum, peoplenum, dishnum, totalprice, 0);
            }
            if (categoryChoice == -1) 
            {
            // 从文件中读取已点的菜品
            char filename[20];
            sprintf(filename, "table_%d.txt", tablenum);
            FILE *tableFile = fopen(filename, "r");
            if (tableFile != NULL) 
            {
                char buffer[1024];
                char lines[1024][1024];
                int lineCount = 0;
                while (fgets(buffer, sizeof(buffer), tableFile) != NULL) 
                {
                buffer[strcspn(buffer, "\n")] = 0; // 去掉换行符
                strcpy(lines[lineCount], buffer);
                lineCount++;
                }
                fclose(tableFile);

                // 显示所有已点的菜品
                printf("\n" BYEL "********** 已点菜品 **********\n" RESET);
                for (int i = 0; i < lineCount; i++) 
                {
                char *line = strdup(lines[i]);
                if (line == NULL) 
                {
                    printf(RED "内存不足，无法删除菜品\n" RESET);
                    continue;
                }
                char *dishName = strtok(line, " ");
                int quantity = atoi(strtok(NULL, " "));
                printf(BGRN "%d. %s x%d\n" RESET, i + 1, dishName, quantity);
                free(line);
                }

                // 让用户选择一个已点的菜品来删除
                printf(BBLU "请输入要删除的菜品的编号（按0返回）：" RESET);
                int dishChoice;
                scanf("%d", &dishChoice);
                dishChoice--; // 转换为数组索引

                if (dishChoice >= 0 && dishChoice < lineCount) 
                {
                // 找到要删除的菜品名称
                char *line = strdup(lines[dishChoice]);
                if (line == NULL) 
                {
                    printf(RED "内存不足，无法删除菜品\n" RESET);
                    continue;
                }
                char *dishToDelete = strdup(strtok(line, " "));
                int quantity = atoi(strtok(NULL, " "));
                double price = atof(strtok(NULL, " "));
                double total = atof(strtok(NULL, " "));
                free(line);

                if (quantity > 1) 
                {
                    // 如果菜品数量大于1，只删除一个
                    quantity--;
                    total -= price;
                    sprintf(lines[dishChoice], "%s %d %.2lf %.2lf", dishToDelete,
                            quantity, price, total);
                } 
                else 
                {
                    // 如果菜品数量等于1，删除整行
                    for (int i = dishChoice; i < lineCount - 1; i++) 
                    {
                    strcpy(lines[i], lines[i + 1]);
                    }
                    lineCount--;
                }
                free(dishToDelete);

                // 将更新后的内容写回到文件中
                tableFile = fopen(filename, "w");
                if (tableFile != NULL) 
                {
                    for (int i = 0; i < lineCount; i++) 
                    {
                    // 在写入文件之前，先检查你要写入的数据是否正确
                    if (strstr(lines[i], "�") == NULL) 
                    {
                        fprintf(tableFile, "%s\n", lines[i]); // 添加换行符
                    }
                    }
                    fclose(tableFile);
                }

                // 找到对应的菜品价格，并从总金额中减去该价格
                totalprice -= price;
                dishnum--; // 在这里将orderCount变量减1

                printf(GRN "已删除菜品：%s\n" RESET, dishToDelete);
                } 
                else 
                {
                printf(RED "无效的选项，请重新选择\n" RESET);
                }
            }
            continue;
            }
            // 让用户选择一个菜品
            dishIndex = selectDishByCategory(categories[categoryChoice - 1]);
            if (dishIndex == -1) 
            {
                continue;
            }

            // 在这里，我们添加代码来处理用户的点单请求
            // 减少所选菜品的库存，增加所选菜品的订单数量
            dishes[dishIndex].stock--;
            dishes[dishIndex].orderCount++;
            dishnum++;                           
            totalprice += dishes[dishIndex].price; 

            // 新建文件并写入菜品信息
            char filename[20];
            sprintf(filename, "table_%d.txt", tablenum);
            FILE *tableFile = fopen(filename, "a+");
            if (tableFile != NULL) 
            {
                fclose(tableFile);
            }

            tableFile = fopen(filename, "r");
            char buffer[1024];
            int found = 0;
            if (tableFile != NULL) 
            {
                while (fgets(buffer, sizeof(buffer), tableFile) != NULL) 
                {
                    char *dishName = strtok(buffer, " ");
                    if (strcmp(dishName, dishes[dishIndex].name) == 0) 
                    {
                        int quantity = atoi(strtok(NULL, " "));
                        double price = atof(strtok(NULL, " "));
                        double total = atof(strtok(NULL, " "));
                        quantity++;
                        total += price;
                        found = 1;
                        break;
                    }
                }
                fclose(tableFile);
            }

                if (found) 
                {
                    // 如果找到了菜品，我们需要更新这个菜品的数量和总价
                    // 首先，我们需要将文件中的所有内容读取到一个字符串数组中
                    char lines[1024][1024];
                    int lineCount = 0;
                    tableFile = fopen(filename, "r");
                    if (tableFile != NULL) 
                    {
                        while (fgets(lines[lineCount], sizeof(lines[lineCount]), tableFile) !=
                            NULL) 
                        {
                            lines[lineCount][strlen(lines[lineCount]) - 1] = '\0'; // 去掉换行符
                            lineCount++;
                        }
                        fclose(tableFile);
                    }

                    // 然后，我们需要更新找到的菜品的数量和总价
                    for (int i = 0; i < lineCount; i++) 
                    {
                        char *line = strdup(lines[i]); // 复制当前行，以便我们可以使用strtok函数
                        char *dishName = strtok(line, " ");
                        if (strcmp(dishName, dishes[dishIndex].name) == 0) 
                        {
                            int quantity = atoi(strtok(NULL, " "));
                            double price = atof(strtok(NULL, " "));
                            double total = atof(strtok(NULL, " "));
                            quantity++;
                            total += price;
                            sprintf(lines[i], "%s %d %.2lf %.2lf", dishName, quantity, price,
                                    total);
                            free(line);
                        break;
                        }
                        free(line);
                    }

                    // 最后，我们需要将更新后的内容写回到文件中
                    tableFile = fopen(filename, "w");
                    if (tableFile != NULL) 
                    {
                        for (int i = 0; i < lineCount; i++) 
                        {
                            fprintf(tableFile, "%s\n", lines[i]); // 添加换行符
                        }
                        fclose(tableFile);
                    }
                } 
                else 
                {
                    // 如果没有找到菜品，我们需要在文件末尾添加新的一行
                    tableFile = fopen(filename, "a");
                    if (tableFile != NULL) 
                    {
                        fprintf(tableFile, "%s 1 %.2lf %.2lf\n", dishes[dishIndex].name,
                                dishes[dishIndex].price, dishes[dishIndex].price);
                        fclose(tableFile);
                    }
                }
    }        
}

// cancelOrder函数，该函数用于取消订单
void cancelOrder() {
    int tableNum;
    printf("请输入桌台号：");
    scanf("%d", &tableNum);
    FILE *orderInfoFile = fopen("order_info.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    
    if(orderInfoFile != NULL && tempFile != NULL) {
        char line[100]; // 假设每行最多100个字符
        int found = 0; // 标记是否找到桌台号对应的行
        
        // 读取每一行，查找匹配的桌台号
        while(fgets(line, sizeof(line), orderInfoFile) != NULL) {
            int currentTableNum;
            // 使用 sscanf 从一行中读取桌台号
            if(sscanf(line, "%d", &currentTableNum) == 1) {
                if(currentTableNum == tableNum) {
                    found = 1;
                    continue; // 跳过这一行，相当于删除这一行
                }
            }
            // 将不是要删除的行写入临时文件
            fprintf(tempFile, "%s", line);
        }
        
        fclose(orderInfoFile);
        fclose(tempFile);
        
        if(found) { // 删除原文件并将临时文件重命名为原文件
            remove("order_info.txt"); 
            rename("temp.txt", "order_info.txt"); 
            char filename[20];
            sprintf(filename, "table_%d.txt", tableNum);
            FILE *tableFile = fopen(filename, "r");
            if (tableFile != NULL) {
                // 如果文件存在，关闭文件并删除它
                fclose(tableFile);
                remove(filename);
            }
            printf("成功撤单\n");
            sleep(5);
        } else {
            remove("temp.txt"); // 删除临时文件
            printf("未找到对应桌台号的订单\n");
        }
    } else {
        printf("无法打开文件\n");
    }
}

// viewTableStatusAndPrice函数，该函数用于查看桌台的状态和价格
void viewTableStatusAndPrice() {
    printf(CLEAR_SCREEN_ANSI);  // 清屏

    int tableNumber;
    printf(BLU "请输入要查看的桌号（按0返回）：" RESET);
    scanf("%d", &tableNumber);

    // 如果输入的桌号为0，返回上一级菜单
    if (tableNumber == 0) {
        return;
    }

    // 打开订单信息文件
    FILE *file = fopen("order_info.txt", "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    // 定义一些变量来存储订单信息
    int currentTableNumber, currentPeopleNumber, currentOrderCount, paid;
    double currentTotalAmount;
    char orderTime[20];

    int orderCount = 0;
    int found = 0;

    // 读取订单信息文件，查找匹配的桌号
    while (fscanf(file, "%d %d %d %lf %d %s", &currentTableNumber, &currentPeopleNumber, &currentOrderCount, &currentTotalAmount, &paid, orderTime) != EOF) {
        if (currentTableNumber == tableNumber) {
            printf("%d. %s桌号：%d, 下单时间：%s\n", ++orderCount, paid ? RED "[已支付]" RESET : BLU "[未支付]" RESET, currentTableNumber, orderTime);
            found = 1;
        }
    }

    fclose(file);

    // 如果没有找到匹配的桌号，提示用户重新输入
    if (!found) {
        printf(RED "未找到对应桌号，请重新输入\n" RESET);
        sleep(5);
        viewTableStatusAndPrice();
        return;
    }

    // 提示用户输入要查看的订单编号
    printf("请输入要查看的订单的编号（按0返回）：");
    int orderChoice;
    scanf("%d", &orderChoice);

    // 如果输入的订单编号为0，返回上一级菜单
    if (orderChoice == 0) {
        return;
    }

    // 重新打开订单信息文件，查找匹配的订单编号
    file = fopen("order_info.txt", "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    orderCount = 0;
    while (fscanf(file, "%d %d %d %lf %d %s", &currentTableNumber, &currentPeopleNumber, &currentOrderCount, &currentTotalAmount, &paid, orderTime) != EOF) {
        if (currentTableNumber == tableNumber) {
            orderCount++;
            if (orderCount == orderChoice) {
                printf("桌号：%d, 下单时间：%s\n", currentTableNumber, orderTime);
                break;
            }
        }
    }

    fclose(file);

    // 提示用户选择一个选项
    printf("请选择一个选项：\n");
    printf(BLU "1. 查看金额\n" RESET);
    printf(GRN "2. 查看订单数\n" RESET);
    printf(CYN "3. 查看用餐人数\n" RESET);
    printf(YEL "4. 查看点单菜品\n" RESET);
    printf(MAG "5. 查看支付小票\n" RESET);
    printf(RED "6. 查看总览\n" RESET);

    int choice;
    scanf("%d", &choice);

    // 根据用户的选择，执行相应的操作
    switch(choice) {
        case 1:
            // 用户选择查看总金额
            printf("总金额：%.2lf\n", currentTotalAmount);
            sleep(5);
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            break;
        case 2:
            // 用户选择查看订单数
            printf("订单数：%d\n", currentOrderCount);
            sleep(5);
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            break;
        case 3:
            // 用户选择查看用餐人数
            printf("用餐人数：%d\n", currentPeopleNumber);
            sleep(5);
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            break;
        case 4:
            // 用户选择查看点单菜品
            if (paid) {
                // 如果已支付，从小票文件中读取菜品信息
                char receiptFileName[50];
                sprintf(receiptFileName, "receipt_%d_%s.txt", currentTableNumber, orderTime);
                FILE *receiptFile = fopen(receiptFileName, "r");
                if (receiptFile == NULL) {
                    printf("无法打开小票文件\n");
                    return;
                }
                char line[100];
                while (fgets(line, sizeof(line), receiptFile)) {
                    if (strstr(line, "菜品") != NULL) {
                        while (fgets(line, sizeof(line), receiptFile)) {
                            if (strstr(line, "合计金额") != NULL) {
                                break;
                            }
                            printf("%s", line);
                        }
                        break;
                    }
                }
                fclose(receiptFile);
            } else {
                // 如果未支付，从桌号文件中读取菜品信息
                char tableFileName[20];
                sprintf(tableFileName, "table_%d.txt", currentTableNumber);
                FILE *tableFile = fopen(tableFileName, "r");
                if (tableFile == NULL) {
                    printf("无法打开桌号文件\n");
                    return;
                }
                char line[100];
                while (fgets(line, sizeof(line), tableFile)) {
                    printf("%s", line);
                }
                fclose(tableFile);
            }
            sleep(5);
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            break;
        case 5:
            // 用户选择查看支付小票
            if (!paid) {
                printf(RED "桌台未支付，无法打印支付小票！\n" RESET);
                sleep(5);
                printf(CLEAR_SCREEN_ANSI);  // 清屏
                break;
            }
            char receiptFileName[50];  // 在这里声明 receiptFileName 变量
            sprintf(receiptFileName, "receipt_%d_%s.txt", currentTableNumber, orderTime);
            FILE *receiptFile = fopen(receiptFileName, "r");
            if (receiptFile == NULL) {
                printf("无法打开小票文件\n");
                return;
            }
            char line[100];
            while (fgets(line, sizeof(line), receiptFile)) {
                printf("%s", line);
            }
            fclose(receiptFile);
            sleep(5);
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            break;
        case 6:
            // 用户选择查看总览
            printf("桌号：%d, 人数：%d, 订单数：%d, 总金额：%.2lf, 状态：%s, 时间：%s\n", currentTableNumber, currentPeopleNumber, currentOrderCount, currentTotalAmount, paid ? "已支付" : "未支付", orderTime);
            sleep(5);
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            break;
        default:
            // 用户输入了无效的选项
            printf("无效的选项，请重新选择\n");
    }
    // 在执行完用户选择的操作后，重新调用自身，以便用户可以继续查看其他信息或进行其他操作
    viewTableStatusAndPrice();
}

// manageTable函数，该函数用于管理桌台
void manageTable() {
    int choice;

    // 使用一个无限循环，让用户可以反复选择操作
    while(1) {
        printf(CLEAR_SCREEN_ANSI);  // 清屏
        printf("\n********** " YEL "桌台管理" RESET " **********\n");
        printf(BLU "1. 预订桌台\n" RESET);  // 提示用户可以选择预订桌台
        printf(GRN "2. 加单\n" RESET);  // 提示用户可以选择加单
        printf(CYN "3. 撤单\n" RESET);  // 提示用户可以选择撤单
        printf(YEL "4. 查看桌台状态\n" RESET);  // 提示用户可以选择查看桌台状态
        printf(RED "5. 返回管理员菜单\n" RESET);  // 提示用户可以选择返回管理员菜单
        printf(MAG "6. 退出系统\n" RESET);  // 提示用户可以选择退出系统

        printf("请选择一个选项：");

        scanf("%d", &choice);  // 读取用户的选择

        // 根据用户的选择，执行相应的操作
        switch(choice) {
            case 1:
                reserveTable();  // 用户选择预订桌台
                break;
            case 2:
                addOrder();  // 用户选择加单
                break;
            case 3:
                cancelOrder();  // 用户选择撤单
                break;
            case 4:
                viewTableStatusAndPrice();  // 用户选择查看桌台状态
                break;
            case 5:
                return;  // 用户选择返回管理员菜单，结束当前函数
            case 6:
                printf(MAG "退出系统\n" RESET);
                exit(0);  // 用户选择退出系统，结束程序
            default:
                printf(RED "无效的选项，请重新选择\n" RESET);  // 用户输入了无效的选项，提示用户重新选择
        }
    }
}