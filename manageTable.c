#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "sleep.h"
#include <string.h>
#include "dish.h"
#include "order.h"
#include <time.h>
#define MAX_TABLES 100

typedef enum {
    UNPAID,
    PAID,
    RESERVED
} TableStatus;

typedef struct {
    int number;
    int people;
    TableStatus status;
    double price;
} Table;

Table tables[MAX_TABLES];
int numTables = 0;


void reserveTable() {
    // 在这里实现预订桌台功能
    printf("\e[1;1H\e[2J");
    printf("请输入您的桌号（范围1-100）：");
    int tableNumber;
    scanf("%d", &tableNumber);

    if (tableNumber < 1 || tableNumber > 100) {
        printf(RED "无效的桌号，请重新输入\n" RESET);
        return;
    }

    // 从order_info.txt文件中加载订单信息
    int currentTableNumber = 0;
    int currentPeopleNumber = 0;
    int currentOrderCount = 0;
    double currentTotalAmount = 0;
    FILE *orderInfoFile = fopen("order_info.txt", "r");
    if (orderInfoFile != NULL) {
        while (fscanf(orderInfoFile, "%d %d %d %lf", &currentTableNumber, &currentPeopleNumber, &currentOrderCount, &currentTotalAmount) == 4) {
            if (currentTableNumber == tableNumber && currentOrderCount > 0 && currentTotalAmount > 0) {
                // 如果已经有当前桌号的订单信息，并且菜的数量和金额不为0，那么跳过点菜过程，直接进入已下单过程
                checkout(currentTableNumber, currentPeopleNumber, currentOrderCount, currentTotalAmount);
                fclose(orderInfoFile);
                return;
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

    placeOrder(tableNumber, peopleNumber,0);
}

void addOrder(){
    int tablenum;
    int latablenum;
    int peoplenum;
    int dishnum;
    double price;
    int status;
    char ordertime[100];
    printf("请输入桌台号：");
    scanf("%d",&tablenum);
    FILE *orderInfoFile = fopen("order_info.txt", "r");
    if (orderInfoFile != NULL) 
    {
        //找到桌台
        while(1)
        {
            fscanf(orderInfoFile,"%d %d %d %lf %d %s",
            &latablenum,&peoplenum,&dishnum,&dishnum,&price,&status,ordertime);
            if(latablenum==tablenum)
            break;
        }    
    }
    fclose(orderInfoFile);

    int dishIndex = 0;

    while (1)
    {
        printf(CLEAR_SCREEN_ANSI);
            printf(BWHT "********** 桌号%d，您已点了%d道菜，目前的金额为%.2lf元 "
            "**********\n" RESET,tablenum, dishnum, price);
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
                    &peoplenum, &dishnum, &price,
                    &status, ordertime) == 6) 
                    {
                        if (latablenum != tablenum || status != 0) 
                        {
                            // 如果不是当前桌号的订单，或者已经支付，那么将这一行写入新的文件中
                            fprintf(tempFile, "%d %d %d %.2lf %d %s\n", latablenum,
                            peoplenum, dishnum, price, status,ordertime);
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
                strftime(timeStr, sizeof(timeStr), "%Y-%m-%d_%H:%M:%S", tm);

                fprintf(orderInfoFile, "\n%d %d %d %.2lf %d %s", tablenum,
                        peoplenum, dishnum, price, 0,
                        timeStr); // 在新的一行中写入订单信息，其中0表示未支付
                fclose(orderInfoFile);

                checkout(tablenum, peoplenum, dishnum, price);
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
        dishnum++;                           // 在这里将orderCount变量加1
        price += dishes[dishIndex].price; // 在这里将totalAmount变量增加

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

void cancelOrder(){
    int tablenum;
    printf("请输入桌台号：");
    scanf("%d",&tablenum);
     FILE *orderInfoFile = fopen("order_info.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    
    if(orderInfoFile != NULL && tempFile != NULL)
    {
        char line[100]; // 假设每行最多100个字符
        int found = 0; // 标记是否找到桌台号对应的行
        
        while(fgets(line, sizeof(line), orderInfoFile) != NULL)
        {
            int currentTablenum;
            // 使用 sscanf 从一行中读取桌台号
            if(sscanf(line, "%d", &currentTablenum) == 1)
            {
                if(currentTablenum == tablenum)
                {
                    found = 1;
                    continue; // 跳过这一行，相当于删除这一行
                }
            }
            // 将不是要删除的行写入临时文件
            fprintf(tempFile, "%s", line);
        }
        
        fclose(orderInfoFile);
        fclose(tempFile);
        
        if(found)
        {
            remove("order_info.txt"); // 删除原文件
            rename("temp.txt", "order_info.txt"); // 将临时文件重命名为原文件
            printf("成功撤单\n");
        }
        else
        {
            remove("temp.txt"); // 删除临时文件
            printf("未找到对应桌台号的订单\n");
        }
    }
    else
    {
        printf("无法打开文件\n");
    }
}

void viewTableStatusAndPrice() {
    printf(CLEAR_SCREEN_ANSI);  // 清屏

    int tableNumber;
    printf(BLU "请输入要查看的桌号（按0返回）：" RESET);
    scanf("%d", &tableNumber);

    if (tableNumber == 0) {
        return;
    }

    FILE *file = fopen("order_info.txt", "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    int currentTableNumber, currentPeopleNumber, currentOrderCount, paid;
    double currentTotalAmount;
    char orderTime[20];

    int orderCount = 0;
    int found = 0;
    while (fscanf(file, "%d %d %d %lf %d %s", &currentTableNumber, &currentPeopleNumber, &currentOrderCount, &currentTotalAmount, &paid, orderTime) != EOF) {
        if (currentTableNumber == tableNumber) {
            printf("%d. %s桌号：%d, 下单时间：%s\n", ++orderCount, paid ? RED "[已支付]" RESET : BLU "[未支付]" RESET, currentTableNumber, orderTime);
            found = 1;
        }
    }

    fclose(file);

    if (!found) {
        printf(RED "未找到对应桌号，请重新输入\n" RESET);
        sleep(5);
        viewTableStatusAndPrice();
        return;
    }

    printf("请输入要查看的订单的编号（按0返回）：");
    int orderChoice;
    scanf("%d", &orderChoice);

    if (orderChoice == 0) {
        return;
    }

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

    printf("请选择一个选项：\n");
    printf(BLU "1. 查看金额\n" RESET);
    printf(GRN "2. 查看订单数\n" RESET);
    printf(CYN "3. 查看用餐人数\n" RESET);
    printf(YEL "4. 查看点单菜品\n" RESET);
    printf(MAG "5. 查看支付小票\n" RESET);
    printf(RED "6. 查看总览\n" RESET);

    int choice;
    scanf("%d", &choice);
        switch(choice) {
            case 1:
                printf("总金额：%.2lf\n", currentTotalAmount);
                sleep(5);
                printf(CLEAR_SCREEN_ANSI);  // 清屏
                break;
            case 2:
                printf("订单数：%d\n", currentOrderCount);
                sleep(5);
                printf(CLEAR_SCREEN_ANSI);  // 清屏
                break;
            case 3:
                printf("用餐人数：%d\n", currentPeopleNumber);
                sleep(5);
                printf(CLEAR_SCREEN_ANSI);  // 清屏
                break;
        case 4:
            if (paid) {
                // 已支付，从小票文件中读取菜品信息
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
                // 未支付，从桌号文件中读取菜品信息
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
            case 5: {
                // 查看支付小票
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
            }
            case 6:
                printf("桌号：%d, 人数：%d, 订单数：%d, 总金额：%.2lf, 状态：%s, 时间：%s\n", currentTableNumber, currentPeopleNumber, currentOrderCount, currentTotalAmount, paid ? "已支付" : "未支付", orderTime);
                sleep(5);
                printf(CLEAR_SCREEN_ANSI);  // 清屏
                break;
            default:
                printf("无效的选项，请重新选择\n");
        }
        viewTableStatusAndPrice();
    }

void manageTable() {
    int choice;

    while(1) {
        printf(CLEAR_SCREEN_ANSI);
        printf("\n********** " YEL "桌台管理" RESET " **********\n");
        printf(BLU "1. 预订桌台(TODO)\n" RESET);//TODO
        printf(GRN "2. 加单(TODO)\n" RESET);//TODO
        printf(CYN "3. 撤单(TODO)\n" RESET);//TODO
        printf(YEL "4. 查看桌台状态\n" RESET);
        printf(RED "5. 返回管理员菜单\n" RESET);
        printf(MAG "6. 退出系统\n" RESET);

        printf("请选择一个选项：");

        scanf("%d", &choice);

        switch(choice) {
            case 1:
                reserveTable();
                // updateTablePeople();
                // updateTableStatus();
                break;
            case 2:
                addOrder();
                break;
            case 3:
                cancelOrder();
                break;
            case 4:
                viewTableStatusAndPrice();
                break;
            case 5:
                return;
            case 6:
                printf(MAG "退出系统\n" RESET);
                exit(0);
            default:
                printf(RED "无效的选项，请重新选择\n" RESET);
        }
    }
}