#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "dish.h"
#include "order.h"
#include <time.h>
#include <string.h>
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
                checkout(currentTableNumber, currentPeopleNumber, currentOrderCount, currentTotalAmount,0);
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

    placeOrder(tableNumber, peopleNumber,0,1);
    printf("预定成功！");
    sleep(5);
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

    //检查桌台是否支付，是则返回到上一步
    FILE *orderInfoFile = fopen("order_info.txt", "r");
    if(orderInfoFile != NULL)
    {
        //找到桌台
        while(1)
        {
            fscanf(orderInfoFile,"%d %d %d %.2lf %d %s",
            &latablenum,&peoplenum,&dishnum,&dishnum,&price,&status,ordertime);
            if(latablenum==tablenum)
            break;
        }    

        if(status == 1)
        {
            printf("\e[1;1H\e[2J");//清屏
            printf("该桌台已经支付完成，请重新下单。");
            return;//返回上级菜单
        }
    }

    FILE *orderInfoFile = fopen("order_info.txt", "r");
    if (orderInfoFile != NULL) 
    {
        //找到桌台
        while(1)
        {
            fscanf(orderInfoFile,"%d %d %d %.2lf %d %s",
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
                    while (fscanf(orderInfoFile, "%d %d %d %.2lf %d %s", &latablenum,
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

                checkout(tablenum, peoplenum, dishnum, price,0);
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
        
        if(found)  // 删除原文件并将临时文件重命名为原文件
        {
            remove("order_info.txt"); 
            rename("temp.txt", "order_info.txt"); 
            char filename[20];
            sprintf(filename, "table_%d.txt", tablenum);
            FILE *tableFile = fopen(filename, "r");
            if (tableFile != NULL) 
            {
                // 如果文件存在，关闭文件并删除它
                fclose(tableFile);
                remove(filename);
            }
            printf("成功撤单\n");
            sleep(5);
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

void updateTablePeople() {
    // 在这里实现更新桌台人数功能
    // int newpeople;
    // int lapeople;
    // int tablenum;
    // char Line[1024][1024];
    // int lineCount = 0;
    // printf("请输入桌台号：");
    // scanf("%d",tablenum);
    // printf("请输入新的人数：");
    // scanf("%d",&newpeople);
    // FILE *orderInfoFile = fopen("order_info.txt", "r+");
    // if((orderInfoFile != NULL))
    // {
    //     //将文件读取到Line中
    //      while (fgets(Line[lineCount], sizeof(Line[lineCount]), orderInfoFile) !=
    //            NULL) {
    //       Line[lineCount][strlen(Line[lineCount]) - 1] = '\0'; // 去掉换行符
    //       lineCount++;
    //     }
    //     fclose(orderInfoFile); 
    // }
    // //修改Line
    //     for(int i=0;Line[tablenum][i]!='/0';i++)
    //     {
    //         if(Line[tablenum][i]=lapeople)
    //         Line[tablenum][i]=newpeople;
    //     }
    // //将Line写回文件
    // FILE *orderInfoFile = fopen("order_info.txt","w+")  ;  
    // if((orderInfoFile != NULL))
    // {

    // }

}

void updateTableStatus() {
    // 在这里实现更新桌台状态功能
}

void viewTablePrice() {
    // 在这里实现查看桌台价格功能
}

void viewTablepeople() {
    // 在这里实现查看桌台人数功能
}

void viewTableStatus() {
    //在这里实现查看桌台状态功能
}

void manageTable() {
    int choice;

    while(1) {
        printf("\n********** " YEL "桌台管理" RESET " **********\n");
        printf(BLU "1. 预订桌台\n" RESET);
        printf(GRN "2. 加单\n" RESET);
        printf(CYN "3. 撤单\n" RESET);
        printf(YEL "4. 桌台人数\n" RESET);
        printf(GRA "5. 桌台状态\n" RESET);
        printf(MAG "6. 查看桌台价格\n" RESET);
        printf(RED "7. 返回管理员菜单\n" RESET);
        printf("8. 退出系统\n");

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
                viewTablePeople();
                break;
            case 5:
                viewTableStatus();
                break;
            case 6:
                viewTablePrice();
                break;
            case 7:
                return;
            case 8:
                printf(MAG "退出系统\n" RESET);
                exit(0);
            default:
                printf(RED "无效的选项，请重新选择\n" RESET);
        }
    }
}