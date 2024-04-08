# 设置CMake的最小版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(Ordering VERSION 1.0)

# 添加可执行文件
add_executable(Ordering main.c admin_menu.c customer_menu.c dish.c manageTable.c order.c order_info.c payment.c receipt.c table.c adminOrder.c)

# 设置C标准
set(CMAKE_C_STANDARD 99)

# 包含CPack
include(CPack)