<h1 align="center">🗒️Ordering</h1>
<p align="center">
    <img src="https://img.shields.io/github/stars/skyhua0224/Ordering.svg" />
    <a href="https://github.com/skyhua0224/Ordering">
        <img src="https://img.shields.io/github/forks/skyhua0224/Ordering.svg" />
    </a>
    <a href="https://github.com/skyhua0224/Ordering/issues">
        <img src="https://img.shields.io/github/issues/skyhua0224/Ordering.svg" />
    </a>
    <a href="https://github.com/skyhua0224/Ordering/pulls">
        <img src="https://img.shields.io/github/issues-pr/skyhua0224/Ordering.svg" />
    </a>
    <a href="https://github.com/skyhua0224/Ordering/graphs/contributors">
        <img src="https://img.shields.io/github/contributors/skyhua0224/Ordering.svg" />
    </a>
    <a href="https://twitter.com/SkyHua_Official">
        <img alt="Twitter: SkyHua_Official" src="https://img.shields.io/twitter/follow/SkyHua_Official.svg?style=social" target="_blank" />
    </a>
</p>

> 这是一个简单的餐饮管理系统，它包括了一些基本的功能，如菜品管理、订单管理、支付和收据生成等。

## 🎯 主要组成部分

- `main.c`: 主函数所在的文件。
- `admin_menu.c`: 管理员菜单的实现。
- `customer_menu.c`: 客户菜单的实现。
- `dish.c`: 菜品管理的实现。
- `manageTable.c`: 餐桌管理的实现。
- `order.c`: 订单管理的实现。
- `order_info.c`: 订单信息的实现。
- `payment.c`: 支付系统的实现。
- `receipt.c`: 收据生成的实现。
- `table.c`: 餐桌的实现。


## 🔗 实体关系

在这个餐饮管理系统中，我们有几个主要的实体：Dish（菜品）、Table（餐桌）、Customer（客户）和Order（订单）。这些实体之间存在以下关系：

- **Dish-Order**：一个订单可以包含多种菜品，而一种菜品可以出现在多个订单中，因此Dish和Order之间是多对多的关系。这部分主要在 `dish.c` 和 `order.c` 中实现。
- **Table-Order**：一个餐桌在一段时间内只能有一个订单，但一个订单可以对应一个餐桌，因此Table和Order之间是一对一的关系。这部分主要在 `table.c` 和 `order.c` 中实现。
- **Customer-Order**：一个客户可以下多个订单，但一个订单只能由一个客户下，因此Customer和Order之间是一对多的关系。这部分主要在 `customer_menu.c` 和 `order.c` 中实现。

这些实体和它们之间的关系构成了我们的餐饮管理系统的核心部分。

## 👨‍💻 编译

使用以下命令来编译项目：

```sh
gcc main.c admin_menu.c customer_menu.c dish.c manageTable.c order.c order_info.c payment.c receipt.c table.c -o program
```

## 🚀 运行

编译完成后，根据你的操作系统，使用以下命令来运行程序：

Windows:

```sh
./program.exe
```

Linux/macOS:

```sh
./program
```

## 🗂️ 文件结构

```
.vscode/
    c_cpp_properties.json
    launch.json
    settings.json
    tasks.json
admin_menu.c
adminOrder.c
adminOrder.h
color.h
customer_menu.c
dish_info.txt
dish.c
dish.h
main_menu.h
main.c
manageTable.c
order_info.c
order_info.h
order_info.txt
order.c
order.h
output/
    main
    main.dSYM/
        Contents/
            ...
payment.c
payment.h
program
program.exe
README.md
receipt_16_2024-03-22_16:01:15.txt
receipt_94.txt
receipt.c
receipt.h
sleep.h
table_11.txt
table_15.txt
table_17.txt
table_95.txt
table.c
table.h
```

## ⚠️ 注意事项

请确保在运行程序前已经正确地编译了所有的源文件。此外，你需要安装GCC编译器。你可以通过以下链接来查看如何安装GCC。此处链接提供的是经过本人使用的英文教程，应该很容易上手，配合DeepL翻译使用更佳。也可以咨询本人：

- [Windows](https://sajidifti.medium.com/how-to-install-gcc-and-gdb-on-windows-using-msys2-tutorial-0fceb7e66454)
- [Linux](https://gcc.gnu.org/install/index.html)
- [MacOS](https://mac.install.guide/commandlinetools/4http://osxdaily.com/2014/02/12/install-command-line-tools-mac-os-x/)

## 🤝 贡献

欢迎提供贡献、问题和功能请求。<br />
如果你想贡献，可以查看[问题页面](https://github.com/skyhua0224/Ordering/issues)。<br />

## 👥 代码贡献者

- [@skyhua0224](https://github.com/skyhua0224)
- [@xshsama](https://github.com/xshsama)
- [@maxkinm](https://github.com/maxkinm)

## 👨‍💻 作者

👤 **SkyHua**

- Telegram: [@skyhua](https://t.me/skyhua)
- Github: [@skyhua0224](https://github.com/skyhua0224)
