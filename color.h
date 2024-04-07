#ifndef COLOR_H
#define COLOR_H

// ANSI转义码，用于在控制台输出中设置颜色
#define RED   "\x1B[31m" // 红色
#define GRN   "\x1B[32m" // 绿色
#define YEL   "\x1B[33m" // 黄色
#define BLU   "\x1B[34m" // 蓝色
#define MAG   "\x1B[35m" // 品红色
#define CYN   "\x1B[36m" // 青色
#define RESET "\x1B[0m"  // 重置颜色
#define GRA "\x1B[90m"   // 灰色
#define WHT "\x1B[37m"   // 白色
#define BRED "\x1B[1;31m" // 亮红色
#define BGRN "\x1B[1;32m" // 亮绿色
#define BYEL "\x1B[1;33m" // 亮黄色
#define BBLU "\x1B[1;34m" // 亮蓝色
#define BMAG "\x1B[1;35m" // 亮品红色
#define BCYN "\x1B[1;36m" // 亮青色
#define BWHT "\x1B[1;37m" // 亮白色
#define RESET "\x1B[0m"   // 重置颜色
#define CLEAR_SCREEN_ANSI "\e[1;1H\e[2J" // ANSI转义码，用于清屏

#endif