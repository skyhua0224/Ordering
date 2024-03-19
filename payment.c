#include <stdio.h>
#include "payment.h"
#include "color.h"
#include "receipt.h"  // 包含 receipt.h 文件

#ifdef _WIN32
    #include <windows.h>
    #define sleep(x) Sleep(1000 * (x))
#elif _POSIX_C_SOURCE >= 199309L
    #include <time.h>   // for nanosleep
    void sleep(int sec) {
        struct timespec ts;
        ts.tv_sec = sec;
        ts.tv_nsec = 0;
        nanosleep(&ts, NULL);
    }
#else
    #include <unistd.h> // for sleep
#endif

void askPrintReceipt(int tableNumber, double totalAmount, Dish* dishes) {
    // 提示用户是否打印小票
    printf("是否打印小票？（1=是，0=否）：");
    int printReceiptChoice;
    scanf("%d", &printReceiptChoice);
    if (printReceiptChoice == 1) {
        printReceipt(tableNumber, totalAmount, dishes);  // 假设 dishes 是你的菜品数组
        sleep(10);
    }
}

void processPayment(int tableNumber, double totalAmount) {
    printf(CLEAR_SCREEN_ANSI);  // 清屏

    printf(GRN "桌号：%d\n" RESET, tableNumber);
    printf(GRN "总金额：%.2lf元\n" RESET, totalAmount);

    printf(CYN "请选择支付方式：\n" RESET);
    printf("1. " RED "现金\n" RESET);
    printf("2. " BLU "支付宝\n" RESET);
    printf("3. " YEL "微信支付\n" RESET);
    printf("4. " MAG "云闪付\n" RESET);
    printf("5. " GRA "我要吃霸王餐！\n" RESET);
    printf("6. " GRA "回到上级菜单\n" RESET);

    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            printf(GRN "应收：" YEL "%.2lf元\n" RESET, totalAmount);
            sleep(5);
            printf(GRN "实收：" YEL "%.2lf元\n" RESET, (double)((int)totalAmount));
            askPrintReceipt(tableNumber, totalAmount, dishes);
            printf(RED "感谢您光临" GRN "SkyHua" YEL " Virtual" BLU " 餐厅" MAG "，期待您下次光临！\n" RESET);
            sleep(5);
            break;
        case 2:
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            printf(GRN "应收：" YEL "%.2lf元\n" RESET, totalAmount);
            printf(RED "请使用" BLU "支付宝" RED "支付" YEL "%.2lf元\n" RESET, totalAmount);
            sleep(5);
            printf(GRN "实收：" YEL "%.2lf元\n" RESET, totalAmount);
            askPrintReceipt(tableNumber, totalAmount, dishes);
            printf(RED "感谢您光临" GRN "SkyHua" YEL " Virtual" BLU " 餐厅" MAG "，期待您下次光临！\n" RESET);
            sleep(5);
            break;
        case 3:
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            printf(GRN "应收：" YEL "%.2lf元\n" RESET, totalAmount);
            printf(RED "请使用" GRN "微信" RED "支付" YEL "%.2lf元\n" RESET, totalAmount);
            sleep(5);
            printf(GRN "实收：" YEL "%.2lf元\n" RESET, totalAmount);
            askPrintReceipt(tableNumber, totalAmount, dishes);
            printf(RED "感谢您光临" GRN "SkyHua" YEL " Virtual" BLU " 餐厅" MAG "，期待您下次光临！\n" RESET);
            sleep(5);
            break;
        case 4:
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            printf(GRN "应收：" YEL "%.2lf元\n" RESET, totalAmount);
            printf(RED "请使用" MAG "云闪付" RED "支付" YEL "%.2lf元\n" RESET, totalAmount);
            sleep(5);
            printf(GRN "实收：" YEL "%.2lf元\n" RESET, totalAmount);
            askPrintReceipt(tableNumber, totalAmount, dishes);
            printf(RED "感谢您光临" GRN "SkyHua" YEL " Virtual" BLU " 餐厅" MAG "，期待您下次光临！\n" RESET);
            sleep(5);
            break;
        case 5:
            printf(CLEAR_SCREEN_ANSI);  // 清屏
            printf(RED "你想吃霸王餐？！\n" RESET);
            sleep(1);
            printf(RED "这是不可能的！\n" RESET);
            sleep(1);
            printf(RED "我们的保安正在赶来...\n" RESET);
            sleep(1);
            printf(RED "你被赶出了餐厅！\n" RESET);
            sleep(1);
            return;
        case 6:
            // 返回上级菜单
            return;
        default:
            printf(RED "无效的选项，请重新选择\n" RESET);
            break;
    }
}