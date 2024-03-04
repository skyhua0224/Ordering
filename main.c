#include <stdio.h>
#include <unistd.h>
#include "color.h"
#include "main_menu.h"

#define CLEAR_SCREEN_ANSI "\e[1;1H\e[2J"

void clearScreen() {
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

void adminMenu();  // 声明adminMenu函数
void customerMenu();  // 声明customerMenu函数\

void mainMenu() {
    int choice;

    while(1) {
        clearScreen();
        printf("\n********** " RED "欢迎使用餐饮管理系统" RESET " **********\n");
        printf(GRN "1. 我是管理员\n" RESET);
        printf(BLU "2. 我是顾客\n" RESET);
        printf(YEL "3. 退出\n" RESET);
        printf("请选择一个选项：");

        scanf("%d", &choice);

        switch(choice) {
            case 1:
                adminMenu();
                break;
            case 2:
                customerMenu();
                break;
            case 3:
                printf(YEL "退出程序\n" RESET);
                return;
            default:
                printf(RED "无效的选项，请重新选择\n" RESET);
        }
    }
}

int main() {
    mainMenu();
    return 0;
}