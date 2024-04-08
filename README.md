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

你需要CMake和GCC来编译这个项目。首先，创建一个新的目录用于构建：

```sh
mkdir build
cd build
```

然后，运行CMake来生成Makefile文件：

```sh
cmake ..
```

最后，运行make来编译项目：

```sh
make
```

这将会在`build`目录下生成一个名为`Ordering`的可执行文件。

## 🚀 运行

编译完成后，根据你的操作系统，使用以下命令来运行程序：

Windows:

```sh
cd build
./Ordering.exe
```

Linux/macOS:

```sh
cd build
./Ordering
```

编译默认会复制example_data的菜单数据和订单数据复制到默认文件夹，如果你要自己使用，请确保在执行文件Ordering的同目录下存在dish_info.txt和order_info.txt。以下分别为两个文件的格式：
dish_info.txt
```
库存 价格（两位小数） 菜名 类别
```
order_info.txt
```
桌号 人数 点菜数量 总金额 是否支付 下单时间（yyyy-MM-dd_HH_mm_ss）
```

## 🗂️ 文件结构

```
.vscode/
    c_cpp_properties.json  // VS Code 的 C/C++ 配置文件
    launch.json  // VS Code 的调试配置文件
    settings.json  // VS Code 的设置文件
    tasks.json  // VS Code 的任务配置文件
example_data/
    dish_info.txt  // 菜品信息的样例数据文件
    order_info.txt  // 订单信息的样例数据文件
.gitignore // Git上传忽略的文件与文件夹
admin_menu.c  // 管理员菜单的实现
adminOrder.c  // 管理员订单的实现
adminOrder.h  // 管理员订单的声明
CMakeLists.txt // 项目的CMake文件
color.h  // 颜色的定义
customer_menu.c  // 客户菜单的实现
dish.c  // 菜品管理的实现
dish.h  // 菜品管理的声明
main_menu.h  // 主菜单的声明
main.c  // 主函数的实现
manageTable.c  // 餐桌管理的实现
order_info.c  // 订单信息的实现
order_info.h  // 订单信息的声明
order.c  // 订单管理的实现
order.h  // 订单管理的声明
payment.c  // 支付系统的实现
payment.h  // 支付系统的声明
README.md  // 项目的 README 文件
receipt.c  // 收据生成的实现
receipt.h  // 收据生成的声明
sleep.c // sleep 函数的实现
sleep.h  // sleep 函数的声明
table.c  // 餐桌的实现
table.h  // 餐桌的声明
```

## ⚙️ 安装CMake

你需要CMake来编译这个项目。以下是在不同操作系统上安装CMake的方法：

- **Windows**：你可以从[CMake官方网站](https://cmake.org/download/)下载CMake的Windows安装程序。下载并运行安装程序，然后按照提示进行操作。我们更推荐使用Windows自带WSL使用Linux环境运行我们的程序，因为团队所使用的是macOS和Ubuntu(WSL)进行开发。你可以使用以下命令在有支持硬件虚拟化的电脑并搭载Windows 10和Windows 11上通过PowerShell安装默认的WSL2 Ubuntu 22.04 LTS:
    ```
    wsl --install
    ```
    然后再参考Linux安装方式安装cmake。

- **Linux**：在大多数Linux发行版中，你可以使用包管理器来安装CMake和make。例如，在团队使用的Debian系Linux中，你可以使用以下命令：

    ```
    sudo apt install build-essential cmake
    ```

- **macOS**：在macOS中，你可以使用Homebrew来安装CMake。如果你还没有安装Homebrew，你可以从Homebrew官方网站下载并安装。然后，你可以使用以下命令来安装CMake：

    ```
    brew install cmake
    ```

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
