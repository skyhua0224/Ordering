#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "dish.h"
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

    placeOrder(tableNumber, peopleNumber);
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
        char dish[1000];
        while(fscanf(orderInfoFile,"%d %d %d %lf %d %s",&latablenum,&peoplenum,&dishnum,&dishnum,&price,&status,ordertime)==6)
        {
            if(latablenum==tablenum)
            {
                printf(CLEAR_SCREEN_ANSI);
                printf(BWHT "********** 桌号%d，您已点了%d道菜，目前的金额为%.2lf元 "
                "**********\n" RESET,tablenum, dishnum, price);
                printf("\n" BYEL "********** 菜品类别 **********\n" RESET);
                for (int i = 0; i < numCategories; i++) 
                {
                printf(BGRN "%d. %s\n" RESET, i + 1, categories[i]);
                printf(BBLU "请输入要点的菜品类别的编号（输入0下单，输入-"
                "1删除已点菜品，输入-2退出点菜）：" RESET);
                int categoryChoice;
                scanf("%d", &categoryChoice);
                }
            }
        }
    }
}

void cancelOrder(){

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