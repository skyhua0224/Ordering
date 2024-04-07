#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "color.h"
#include "main_menu.h"

// adminMenu函数的声明，该函数用于显示管理员菜单
void adminMenu();

// customerMenu函数的声明，该函数用于显示顾客菜单
void customerMenu();

// mainMenu函数，该函数用于显示主菜单，并根据用户的选择调用相应的函数
void mainMenu() {
    int choice;

    while(1) {
        printf(CLEAR_SCREEN_ANSI);
        printf("\n********** " RED "欢迎使用餐饮管理系统" RESET " **********\n");
        printf(GRN "1. 我是管理员\n" RESET);
        printf(BLU "2. 我是顾客\n" RESET);
        printf(YEL "3. 退出\n" RESET);
        printf("请选择一个选项：");

        scanf("%d", &choice);

        switch(choice) {
            case 1:
                // 如果用户选择1，显示管理员菜单
                adminMenu();
                break;
            case 2:
                // 如果用户选择2，显示顾客菜单
                customerMenu();
                break;
            case 3:
                // 如果用户选择3，退出程序
                printf(YEL "退出程序\n" RESET);
                exit(0);
            default:
                // 如果用户选择的不是1、2或3，显示错误消息
                printf(RED "无效的选项，请重新选择\n" RESET);
        }
    }
}

// main函数，程序的入口点。这个函数调用mainMenu函数来显示主菜单
int main() {
    mainMenu();
    return 0;
}