## 0. 引言
Github: 月と猫 - LunaNeko https://github.com/LunaticLegacy

CC-BY-NC-SA 4.0

本文为针对C/C++的配置方案所写的教程，且适合让新手快速上手。

## 1. CMake的运行结构

CMake是基于一系列（可能来自不同文件的）`CMakeLists.txt`进行运转的、该系统会**基于当前架构**生成对应的构建系统的系统。该系统基于**目标**而构建。

基础使用方法（对make）：

```bash
cmake .     # 在当前目录生成makefile
make        # 正式进行编译
```

如果是对ninja，则：

```bash
cmake . -G "Ninja"  # -G: Generator，指定目标构建系统
ninja
```

## 2. CMake基础教程

基础教程包含对单个`CMakeLists.txt`的常见命令使用方法。

在一个`CMakeLists.txt`里，所有的命令都将从上到下被执行。

### 2.1 设置一个项目

设置一个项目，首先需要规定如下内容：

```cmake
cmake_minimum_required(VERSION 3.20)  # 指定要求的CMake最低版本
project(LunaMoon VERSION 1.01)        # 定义项目名称和版本号
```

`project()`也可以规定特定语言，例：

```cmake
# 使用C和C++，其中CXX在CMake中代指C++。（很形象，不是吗？）
project(LunaMoon VERSION 1.01 LANGUAGES C CXX)  
```

### 2.2 设置需被编译的文件

如果，你的源文件在`src`文件夹下，这样设置以选择需要编译的文件：

```cmake
# 添加可执行文件
add_executable(LunaMoonApp
    main.cpp
)
```

该命令将设置一个**目标**，在命令中列举的将被编译为该目标。

- 在部分情况下，也可以见到使用`${CMAKE_SOURCE_DIR}`后跟上目录。
  - 使用`${VAR}`以将变量名为`VAR`的变量填充到字符串中。
  - 如果目录里有空格，则必须使用`"src/filename with space.cpp"`。
  - 目录中，也可以使用[正则表达式]()。

生成一个目标时，可以一次编译多个文件，例如：
```cmake
# 变量CMAKE_SOURCE_DIR为主目录下的CMakeLists.txt所在位置。
add_executable(LunaMoonApp
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/src/logger.cpp
    ${CMAKE_SOURCE_DIR}/src/sql_handler.cpp
)
```

目标并不一定必须是可执行文件，也可以是静态库或者动态库。
```cmake
# 静态库，目标格式：.a(GCC/Clang)或.lib(MSVC)
add_library(MyLib STATIC
    src/math.cpp
)

# 动态库，目标格式：.so(GCC/Clang)或.dll(MSVC)
add_library(MySharedLib SHARED
    src/math.cpp
)

```
- 静态库：编译期被链接（并打包）到可执行文件的库。
- 动态库；在运行时由可执行文件在`相同目录下`寻找并被加载的库。（可执行文件本身不包含动态库的代码）

### 2.3 链接库和可执行文件

使用：

```cmake
# 链接库到可执行文件
target_link_libraries(LunaMoonApp
    PRIVATE MyLib        # 私有链接：只对LunaMoonApp可见
)
```
以将一个库链接到其他目标。该方法同时适用于静态库和动态库。
- 静态库会被直接打包到目标内。
- 动态库将被复制到目标可执行文件下。
  - 无法将动态库链接到静态库。（从两者的使用方法上也注定了这个）

| 关键词      | 作用                                                |
| ----------- | -------------------------------------------------- |
| `PRIVATE`   | 库仅用于当前目标（可执行文件）                       |
| `PUBLIC`    | 库用于当前目标，同时会传递给依赖该目标的其他目标      |
| `INTERFACE` | 库不直接用于当前目标，只会传递给依赖该目标的其他目标  |


### 2.4 设置头文件路径

如果你写的头文件在`include`文件夹下，这样设置以选择需被包含的头文件：

```cmake
target_include_directories(LunaMoonApp
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)
```

### 2.5 条件分支跳转

```cmake
if(<condition>)
    # 条件为真时执行的命令
elseif(<condition2>)
    # 上一个条件不满足，且此条件为真时执行
else()
    # 上述条件都不满足时执行
endif()
```

例：
```cmake
set(USE_MYLIB ON)

if(USE_MYLIB)
    message("MyLib will be used")
else()
    message("MyLib will NOT be used")
endif()
```

也可以用于比较操作，例：
```cmake
set(VERSION 2)

if(VERSION EQUAL 1)
    message("Version is 1")
elseif(VERSION GREATER 1)
    message("Version greater than 1")
else()
    message("Version less than 1")
endif()
```
常用比较操作：
- `EQUAL`：等于
- `LESS` / `LESS_EQUAL`：小于 / 小于等于
- `GREATER` / `GREATER_EQUAL`：大于 / 大于等于
- `STREQUAL`：字符串相等
- `DEFINED VAR`：变量`VAR`是否定义

也可以用于检测使用平台和编译器：
```cmake
if(WIN32)                                   # OS类型：windows还是unix类（linux）？
    message("Windows platform")
elseif(UNIX)
    message("Unix or Linux platform")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")    # 是否使用GNU编译器？
    message("Using GCC")
endif()
```

| OS flag | 释意 |
| ------- | ---- |
| WIN32   | windows |
| UNIX    | linux/unix等类unix系统 |
| APPLE   | IOS等苹果系统 |

## 3. CMake进阶教程

进阶教程包含：
- 多个`CMakeLists.txt`之间的联动
- 子目标，及命令`add_subdirectory`
- 更为详细的运行环境配置
- 变量作用域、目标属性继承
- 文件管理

### 3.1 多个CMakeLists.txt之间的联动

现在，你的项目结构是：

```bash
LunaMoon/
├── CMakeLists.txt       # 根目录的主CMakeLists.txt（简称“主头”）
├── src/
│   ├── CMakeLists.txt   # 子目录的CMakeLists.txt（简称“子头”）
│   ├── main.cpp
│   └── app/
│       ├── CMakeLists.txt
│       └── logger.cpp
└── include/
    └── logger.h
```

且主头内包含：
```cmake
cmake_minimum_required(VERSION 3.20)
project(LunaMoon VERSION 1.01 LANGUAGES C CXX)

add_subdirectory(src)
```

`src`目录的子头包含：
```cmake
add_subdirectory(app)

add_executable(LunaMoonApp main.cpp)

target_link_libraries(LunaMoonApp
    PRIVATE AppLib
)

target_include_directories(LunaMoonApp
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

```

`src/app`目录子头包含：
```cmake
add_library(AppLib STATIC logger.cpp)

target_include_directories(AppLib
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)
```

- `add_subdirectory(target)`命令将执行目录`target`下的子头，并在本命令后允许后续命令使用该子头生成的目标。

### 3.2 更详细的运行环境配置

配置运行环境可使用函数：`set()`。使用方法例如：

```cmake
set(CMAKE_CXX_STANDARD 20)  # 使用C++标准：C++ 20
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制要求使用该标准
set(CMAKE_CXX_EXTENSIONS OFF)     # 禁用编译器扩展
```

可以更改或设置如下系统内置变量：
（本表仅有极少部分的变量内容，更多内容请自行查找，本教程不继续赘述）

| 变量名 | 作用 | 默认值 |
| --------------------------------- | -------------------------------------------- | ------------- |
| `CMAKE_CXX_STANDARD`   | 设置 C++ 标准（如 11、14、17、20 等）  | 无（由编译器默认）|
| `CMAKE_C_STANDARD`  | 设置 C 标准（如 99、11、17 等）   | 无（由编译器默认）  |
| `CMAKE_BUILD_TYPE` | 设置编译类型（如 `Debug`、`Release`、`RelWithDebInfo`） | 空（CMake 默认为空） |
| `CMAKE_EXPORT_COMPILE_COMMANDS` | 是否生成 `compile_commands.json`，方便 IDE/工具使用 | OFF|
| `CMAKE_SOURCE_DIR`  | 项目根目录的路径 | - |
| `CMAKE_BINARY_DIR` | 构建目录路径（通常与执行 `cmake .` 的目录相同） | - |
| `CMAKE_CURRENT_SOURCE_DIR` | 当前 CMakeLists.txt 所在目录  | - |
| `CMAKE_CURRENT_BINARY_DIR` | 当前构建目录对应的路径   | - |
| `CMAKE_C_COMPILER` | C 编译器路径 | - |
| `CMAKE_CXX_COMPILER`| C++ 编译器路径 | - |
| `CMAKE_C_FLAGS`| C 编译器默认选项 | - |
| `CMAKE_CXX_FLAGS` | C++ 编译器默认选项 | - |
| `CMAKE_SYSTEM_NAME` | 系统名称（如 Linux、Windows、Darwin） | 基于运行环境 |
| `CMAKE_SYSTEM_PROCESSOR` | 系统架构（如 x86\_64） | - |
| `CMAKE_SIZEOF_VOID_P` | 指针大小（4 或 8 字节） | - |

- 小插曲：这里给出一份我在工程中使用的小技巧，用于在生成Debug版本内容时，使用GDB调试构建的内容。
```cmake
set(CMAKE_BUILD_TYPE Debug)     # 在这里设置编译选项。

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -g -ggdb -O0")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g -ggdb")
    # -g：生成调试符号，-ggdb：生成gdb可用的调试符号
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -O2")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
endif()
```

### 3.3 文件管理

使用`file(<operation> ...)`以进行文件管理。注意：该命令不生成目标。

#### 3.1 查找文件

```cmake
# 查找项目根目录下的所有 .cpp 文件
file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# 查找 src 下及子目录的所有 .h 文件
file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/include/*.h")
```
- `GLOB batch_name <filenames>`：查找目标目录，并将找到的目录以`batch_name`为变量供后续使用。
- `GLOB_RECURSE batch_name <filenames>`：递归查找目标目录及其所有子目录。（在使用正则表达式时使用）

例：
```
# 源文件
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# 创建可执行文件
add_executable(starcity ${SRC_FILES})
```

#### 3.2 目录操作和文件操作
```cmake
# 创建目录
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated)

# 复制文件
file(COPY ${CMAKE_SOURCE_DIR}/data/config.json DESTINATION ${CMAKE_BINARY_DIR}/data)

# 删除文件或目录
file(REMOVE ${CMAKE_BINARY_DIR}/old_file.txt)
file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/old_dir)
```

#### 3.3 文件写入

```
# 写入一个配置文件，覆盖写
file(WRITE ${CMAKE_BINARY_DIR}/version.txt "Version 1.01\n")

# 在文件末尾追加内容，追加写
file(APPEND ${CMAKE_BINARY_DIR}/version.txt "Build: Debug\n")
```

## 4. 外部使用时的命令

使用：

```bash
cmake .
```
以在当前位置启动。其中，`.`可以被更换为其他相对文件路径，但生成配置文件时均在本命令执行路径。

推荐用法：
```bash
mkdir build
cd build
cmake ..
```
这样会将所有东西放倒文件夹`build`中，便于进行管理。

```bash
cmake -D VAR_NAME=VALUE .
```
`-D`：Define，在外部定义某个变量。其效果等同于脚本文件中使用的`set()`。

```bash
cmake -G "Ninja" .
```
`-G`：Generator，选择生成器。
