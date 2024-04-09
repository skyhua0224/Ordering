# 设置CMake的最小版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(Ordering VERSION 1.0)

# 添加可执行文件
add_executable(Ordering main.c admin_menu.c customer_menu.c dish.c manageTable.c order.c order_info.c payment.c receipt.c table.c adminOrder.c)

# 设置C标准
set(CMAKE_C_STANDARD 99)

# 将文件从example_data目录复制到构建目录
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/example_data/order_info.txt ${CMAKE_CURRENT_BINARY_DIR}/order_info.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/example_data/dish_info.txt ${CMAKE_CURRENT_BINARY_DIR}/dish_info.txt COPYONLY)

# 设置CPack变量
set(CPACK_PACKAGE_NAME "Ordering")
set(CPACK_PACKAGE_VENDOR "SkyHua Studio")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ordering - A simple catering management system with features such as dish management, order management, payment, and receipt generation.")
set(CPACK_PACKAGE_VERSION "1.0.1")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Ordering")
set(CPACK_SOURCE_IGNORE_FILES "/build/;${CPACK_SOURCE_IGNORE_FILES}")

# 设置CPack包的文件名和源代码目录
set(CPACK_PACKAGE_FILE_NAME "Ordering-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_SOURCE_INSTALLED_DIRECTORIES "${CMAKE_SOURCE_DIR};/")

# 设置CPack生成的安装包类型
set(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/package")

# 包含CPack
include(CPack)