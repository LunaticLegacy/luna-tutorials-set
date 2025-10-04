## 0. 引言
Github: 月と猫 - LunaNeko https://github.com/LunaticLegacy

CC-BY-NC-SA 4.0

本教程可能较为硬核，适合具有一定计算机基础和编程基础的人食用。如果在食用过程中昏厥或口吐白沫，本人概不负责。

本教程为自行总结，可能存在错误。如有错误问题请通过Repo反馈。

本教程使用`GNU C`，如果有MSVC特性，请按照MSVC特性处理。

### 0.1 你的第一条C程序

```c
// 复制粘贴下列内容，并命名为main.c
#include <stdio.h>
int main() {
    printf("Hello world!\n");
    return 0;
}
```

使用该程序，将其**编译**并**运行**即可。
- 编译器可选择：GCC（传统且跨行业）、Clang（新兴）或MSVC（微软官方，仅限windows环境）。

在windows环境下，使用该命令以编译并执行。
```powershell
gcc main.c -o main.exe
.\main.exe
```

输出结果为：
```
Hello world!
```

恭喜你，你的第一条程序跑通了！好耶~

## 1. 变量、常量和运算符（Constant, Variable & Operator） 

### 1.1 数字（Numbers）

数字分为**整数**和**小数**，其中小数使用浮点数表示。

其中，整数类型包含：
```c
char c = 2;     // 字节
short s = 4;    // 短整数
int a = 3;      // 整数
long b = 4;     // 长整数
long long d = 5;    // 超长整数
```

整数类型均可加入`unsigned`，以表示"无符号"位。无符号数最低值为0，有符号整数最低值为**对应无符号数最大值÷2，并加负号**。

浮点数类型包含：
```c
float f = 1.0f;     // 单精度浮点数
double db = 2.0;    // 双精度浮点数
long double ld = 3.0l; // 长双精度浮点数
```

不用类型的内存占用如下：

| 数据类型 | 平台 | 平台位宽 | 占用内存 |
| ------- | ---- | -------- | ------ |
| char | 通用 | 任意 | 1字节 |
| short | 通用 | 任意 | 2字节 |
| int | 32位平台 | 32位 | 4字节 |
| int | 64位平台 | 64位 | 4字节（常见）或 8字节 |
| long | 32位Windows | 32位 | 4字节 |
| long | 64位Windows | 64位 | 4字节 |
| long | Unix/Linux 64位 | 64位 | 8字节 |
| long long | 通用 | 任意 | 8字节 |
| float | 通用 | 任意 | 4字节 |
| double | 通用 | 任意 | 8字节 |
| long double | Windows | 任意 | 8字节 |
| long double | Unix/Linux | 任意 | 12或16字节 |

由于不同类型在不同平台的位宽度不一致，如果需要固定宽度的整数类型，请使用头文件`<stdint.h>`。该头文件提供了诸如`int8_t`、`int16_t`、`int32_t`、`int64_t`等固定宽度的整数类型。

### 1.2 常量（Constant）

常量是固定值，在程序执行期间不允许被改变。C语言中可以使用以下方式定义常量：

1. 直接使用`#define`预处理器指令：
```c
#define PI 3.14159
#define MAX 100
```

2. 使用`const`关键字：
```c
const int MAX_LENGTH = 100;
const float PI_VALUE = 3.14159f;
```

### 1.3 变量（Variable）

变量是可以在程序执行期间改变其值的标识符。在C语言中，变量在使用前必须先声明。（但是可以在声明时不初始化）

变量声明的基本形式为：
```c
type variable_name;
```

例如：
```c
int age;
float salary;
char initial;
```

变量也可以在声明时初始化：
```c
int age = 25;
float salary = 50000.0f;
char initial = 'A';
```

变量的作用域：
- 局部变量：在函数或代码块内定义，只在该范围内有效
- 全局变量：在所有函数外部定义，整个程序都可访问

变量的存储类型：
- `auto`：自动变量（默认）
- `register`：寄存器变量（建议存储在CPU寄存器中，`现已被弃用`）
- `static`：静态变量（生命周期为整个程序运行期间）
    - “静态”的意思是该变量不会在程序运行期间被动态创建。
- `extern`：外部变量（在其他文件中定义）

### 1.4 操作符（Operator）

1. 算术：`+ - * / % ++ -- = += -= *= /=`
- `+`和`-`：加减法。如果最终结果超出了当前数据类型所表示的范围，则会溢出或下溢。
    - 典例：《文明1》中`甘地`的侵略值为`1`，但游戏中保存“侵略值”的变量用的是`unsigned char`。在满足特定条件下，领导人的侵略值会`-2`，此时就可以看到`1-2=255`的名场面——甘地的侵略值现在是`255`，导致他疯狂给印度造核弹。（~~即使这件事后来被证实是个谣言，但这个例子仍然极为有助于记忆~~）
- `*` 和 `/`：乘除法。对于整数和浮点数而言，该操作的行为会有所不同。
- `%`：取模（MOD），仅限整数使用。
- `=`：赋值。**会修改变量**。
    - 用于赋值，例：`double k, x; ...; double y = k * x + b`。
- `++`和`--`：自增、自减。**会修改变量**。
    - 后缀该符号时（如`int p = k++`）：先赋值，再执行操作
    - 前缀该符号时（如`int p = ++k`）：先执行操作，再赋值
    - 不要相信`(i++) + (++i)`，能问出这种问题的都是脑残——在同一表达式中多次**修改**同一变量的行为在C语言中属于**未定义行为**，被编译器自行实现。
        - 这个算不算多次修改：`k = k * k + 3`？不算。

2. 逻辑与比较：`&& || ! == < <= >= > !=`
逻辑运算符一般配合比较运算符一起使用。
- `&&`：与
- `||`：或
- `!`：非
- `==`：等于
- `<`：小于
- `<=`：大于
- `>`：小于或等于
- `>=`：大于或等于
- `!=`：不等于

4. 位操作：`& | ~`
- `&`：按位与。
- `|`：按位或。
- `~`：按位取反。

5. 内存管理：`* &`。
- `*`：解指针（解引用），用于根据当前的数字解析到其对应的内存地址。**如果在定义变量时使用该符号，意为“定义指针变量”。详见[指针](#41-指针pointer)。**
    - 例：`int value = *ptr`，为将变量`ptr`数字作为内存地址，解析对应位置上的值，赋给`value`。
- `&`：取地址，用于获取某个特定变量（的开头）所位于的内存地址。
    - 例：`int p = &a`，将变量`a`的所在地址赋给`p`。


### 1.5 本章节示例程序

```c

int main() {
    int a = 3;
    short b = 5;
    long k = 9;
    long p = a + b + k;  // 这里，p为17
}

```

## 2. 输入与输出（Input & Output）

在C语言中，程序与外界进行数据交互主要通过标准输入输出流来实现。C语言提供了三个标准流：

- `stdin`（标准输入）：通常是键盘输入
- `stdout`（标准输出）：通常是屏幕显示
- `stderr`（标准错误）：通常是屏幕显示，但可以独立重定向

要使用这些流，需要包含`<stdio.h>`头文件。

本节会有**函数**概念，位于[函数](#3-函数)节。详见该内容。

### 2.1 输出流（Output）

C语言中最常用的输出函数是`printf`，它可以将格式化的数据输出到标准输出设备（通常是屏幕）。

基本语法：
```c
#include <stdio.h>
int printf(const char *format, ...);
```

示例：
```c
#include <stdio.h>

int main() {
    int age = 25;
    float height = 175.5f;
    char name[] = "张三";
    
    printf("姓名: %s\n", name);
    printf("年龄: %d 岁\n", age);
    printf("身高: %.1f cm\n", height);
    printf("综合信息: %s, %d岁, %.1fcm\n", name, age, height);
    
    return 0;
}
```

常用的格式说明符：
- `%d` 或 `%i`：十进制整数
- `%f`：浮点数
- `%c`：字符
- `%s`：字符串
- `%x`：十六进制整数
- `%o`：八进制整数
- `%%`：输出百分号本身

除了[printf](file:///E:/Program%20Files/CodeBlocks/MinGW/include/stdio.h#L361-L361)外，还有其他输出函数：
- `puts()`：输出字符串并在末尾添加换行符
- `putchar()`：输出单个字符

### 2.2 输入流（Input）

C语言中最常用的输入函数是`scanf`，它可以从标准输入设备（通常是键盘）读取格式化的数据。

基本语法：
```c
#include <stdio.h>
int scanf(const char *format, ...);
```

示例：
```c
#include <stdio.h>

int main() {
    int age;
    float height;
    char name[50];
    
    printf("请输入您的姓名: ");
    scanf("%s", name);  // 注意：这里不使用&，因为name本身就是一个地址
    
    printf("请输入您的年龄: ");
    scanf("%d", &age);  // 注意：这里需要使用&获取变量的地址
    
    printf("请输入您的身高: ");
    scanf("%f", &height);
    
    printf("您输入的信息:\n");
    printf("姓名: %s\n", name);
    printf("年龄: %d 岁\n", age);
    printf("身高: %.1f cm\n", height);
    
    return 0;
}
```

常用的格式说明符与[printf](file:///E:/Program%20Files/CodeBlocks/MinGW/include/stdio.h#L361-L361)类似：
- `%d` 或 `%i`：读取十进制整数
- `%f`：读取浮点数
- `%c`：读取字符
- `%s`：读取字符串
- `%x`：读取十六进制整数
- `%o`：读取八进制整数

除了[scanf](file:///E:/Program%20Files/CodeBlocks/MinGW/include/stdio.h#L356-L356)外，还有其他输入函数：
- `gets()`：读取一行字符串（不推荐使用，因为不安全）
- `fgets()`：读取一行字符串（推荐使用）
- `getchar()`：读取单个字符

### 2.3 错误流（Error）

错误流（stderr）通常用于输出错误信息，与标准输出流（stdout）分开，这样可以独立重定向错误信息。

示例：
```c
#include <stdio.h>

int main() {
    int divisor, dividend;
    
    printf("请输入被除数: ");
    scanf("%d", &dividend);
    
    printf("请输入除数: ");
    scanf("%d", &divisor);
    
    if (divisor == 0) {
        fprintf(stderr, "错误: 除数不能为零!\n");
        return 1;  // 返回错误码
    }
    
    printf("结果: %d / %d = %.2f\n", dividend, divisor, (float)dividend/divisor);
    
    return 0;
}
```

使用`fprintf`函数可以将输出定向到指定的流，例如：
- `fprintf(stdout, ...)` 等价于 `printf(...)`
- `fprintf(stderr, ...)` 将输出到错误流

## 3. 函数

函数是C语言程序的基本构建块，用于将复杂的任务分解为更小、更易管理的部分。函数可以接收输入参数，执行特定任务，并返回结果。

### 3.1 函数的定义

C语言函数的基本结构如下：

```c
返回类型 函数名(参数列表) {
    // 函数体
    // 执行语句
    return 返回值;  // 如果返回类型不是void
}
```

示例：
```c
// 一个简单的加法函数
int add(int a, int b) {
    int sum = a + b;
    return sum;
}

// 一个不返回值的函数
void printWelcome() {
    printf("欢迎使用我们的程序！\n");
}
```

### 3.2 函数的调用

要使用函数，需要在代码中调用它：

```c
#include <stdio.h>

// 函数声明（可选，但推荐）
int add(int a, int b);
void printWelcome();

// 函数定义
int add(int a, int b) {
    return a + b;
}

void printWelcome() {
    printf("欢迎使用我们的程序！\n");
}

int main() {
    // 调用函数
    printWelcome();
    
    int result = add(5, 3);
    printf("5 + 3 = %d\n", result);
    
    return 0;
}
```

### 3.3 函数的参数

函数可以接收零个或多个参数：

```c
// 无参数函数
void sayHello() {
    printf("Hello!\n");
}

// 一个参数函数
void printNumber(int num) {
    printf("数字是: %d\n", num);
}

// 多个参数函数
int multiply(int x, int y) {
    return x * y;
}

// 无返回值函数
void printInfo(char name[], int age) {
    printf("姓名: %s, 年龄: %d\n", name, age);
}
```

### 3.4 函数的返回值

函数可以返回值，也可以不返回值（使用[void](file:///E:/Program%20Files/CodeBlocks/MinGW/include/raylib.h#L279-L279)关键字）：

```c
// 返回整数的函数
int getMax(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

// 返回浮点数的函数
float calculateArea(float radius) {
    return 3.14159 * radius * radius;
}

// 不返回值的函数
void printMessage(char message[]) {
    printf("%s\n", message);
}

// 返回字符的函数
char getFirstChar(char str[]) {
    return str[0];
}
```

### 3.5 函数声明与定义

在C语言中，函数声明告诉编译器函数的名称、返回类型和参数。函数定义包含函数的实际实现。

```c
#include <stdio.h>

// 函数声明
int square(int x);
void printResult(int value);

// 主函数
int main() {
    int number = 5;
    int squared = square(number);
    printResult(squared);
    return 0;
}

// 函数定义
int square(int x) {
    return x * x;
}

void printResult(int value) {
    printf("结果是: %d\n", value);
}
```

### 3.6 递归函数

函数可以调用自身，这称为递归：

```c
#include <stdio.h>

// 计算阶乘的递归函数
int factorial(int n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

int main() {
    int num = 5;
    printf("%d的阶乘是: %d\n", num, factorial(num));
    return 0;
}
```

以上示例将输出：`5的阶乘是: 120`

- 注意：写一个递归函数时，**必须设置**结束条件，否则会**栈溢出**。

函数是C语言程序设计的核心概念，合理使用函数可以提高代码的可读性、可维护性和可重用性。

## 4. 内存管理

在C语言里，内存需要被程序员**手动管理**。
- 如果是裸机开发，这一条内容极为重要。
- 如果是基于操作系统开发，该内容仍然很重要。
- 总之，无论是哪个环境，C的指针都极为重要。

### 4.0 内存结构

C程序在运行时，内存通常被划分为几个不同的区域：

1. **代码区（Text Segment）**：
   - 存放程序的机器指令
   - 通常是只读的，防止程序意外修改自身指令

2. **数据区（Data Segment）**：
   - 包含全局变量和静态变量
   - 进一步分为已初始化数据区和未初始化数据区（BSS）

3. **堆区（Heap）**：
   - 用于动态内存分配
   - 程序运行时通过`malloc`、`calloc`、`realloc`函数分配。
       - 堆区的内存被操作系统控制，调用内存分配函数时，将调用操作系统功能
   - 需要程序员手动释放（`free`），否则会造成内存泄漏
       - 内存泄漏：在程序运行周期内，一块被分配的内存未被回收了

4. **栈区（Stack）**：
   - 用于存储函数的局部变量、参数和返回地址
   - 自动分配和释放
   - 大小有限，过深的递归可能导致**栈溢出**
   - 操作系统会给程序分配一个有限大小的栈区

内存布局示例：
```
高地址 -->  +------------------+
            |                  |
            |   命令行参数      |
            |   环境变量        |
            +------------------+
            |     栈区         |  ← 栈顶向低地址增长
            |    (Stack)       |
            +------------------+
            |                  |
            |       ↓          |
            |                  |
            +------------------+
            |     堆区         |  ← 堆顶向高地址增长
            |     (Heap)       |
            +------------------+
            |   未初始化数据    |
            |     (BSS)        |
            +------------------+
            |   已初始化数据    |
            |    (Data)        |
            +------------------+
            |     代码区        |
            |    (Text)        |
低地址 -->  +------------------+
```

### 4.1 指针（Pointer）

指针是C语言的核心概念之一，它存储变量的内存地址。

#### 4.1.1 指针的基本概念

指针是一个整数，其值为另一个变量的地址。基本语法：
```c
数据类型 *指针变量名;
// 或者：
数据类型* 指针变量名;
```

指针类型控制`从该指针指向的位置开始，接下来的一段内存块应被如何解析`。
- `0x66 0x73 0x6F 0x61`可被解释为4个ASCII字符：`fIoa`，也可以被解释为一个`int32_t`（`1,718,841,185`）或`uint32_t`（`1,718,841,185`）或`int16_t[2]`（`{26227, 28513}`）...
- 如果不对指向的内存规定类型，则数字将毫无意义。

示例：
```c
#include <stdio.h>

int main() {
    int var = 100;        // 声明一个整型变量
    int *ptr;             // 声明一个指向整型的指针
    
    ptr = &var;           // 将var的地址赋给ptr
    
    printf("var的值: %d\n", var);           // 输出: 100
    printf("var的地址: %p\n", &var);        // 输出var的地址
    printf("ptr的值: %p\n", ptr);           // 输出ptr存储的地址
    printf("ptr指向的值: %d\n", *ptr);      // 输出ptr指向的值，即var的值
    
    return 0;
}
```

#### 4.1.2 指针的声明和初始化

```c
// 声明指针
int *ptr1;        // 声明指向整型的指针
float *ptr2;      // 声明指向浮点型的指针
char *ptr3;       // 声明指向字符的指针

// 声明时初始化
int num = 42;
int *ptr = &num;  // 声明并初始化指向num的指针

// 空指针
int *nullPtr = NULL;  // 或者 int *nullPtr = 0;
```

#### 4.1.3 指针的使用

```c
#include <stdio.h>

int main() {
    int var = 10;
    int *ptr = &var;
    
    // 通过指针访问值（解引用）
    printf("var的值: %d\n", *ptr);
    
    // 通过指针修改值
    *ptr = 20;
    printf("修改后var的值: %d\n", var);
    
    // 指针的算术运算
    int arr[] = {1, 2, 3, 4, 5};
    int *arrPtr = arr;  // 数组名本身就是地址
    
    printf("数组第一个元素: %d\n", *arrPtr);
    printf("数组第二个元素: %d\n", *(arrPtr + 1));
    
    return 0;
}
```

#### 4.1.4 指针与数组

在C语言中，数组名本质上就是指向数组第一个元素的指针：

```c
#include <stdio.h>

int main() {
    int arr[5] = {10, 20, 30, 40, 50};
    
    // 以下三种方式是等价的
    printf("第一种: %d\n", arr[0]);
    printf("第二种: %d\n", *arr);
    printf("第三种: %d\n", *(arr + 0));
    
    // 使用指针遍历数组
    int *ptr;
    for (ptr = arr; ptr < arr + 5; ptr++) {
        printf("元素: %d\n", *ptr);
    }
    
    return 0;
}
```

注意：如果在函数内传入数组，数组长度信息将丢失——此时在函数内部该变量将被解析为**指针**。

#### 4.1.5 指针与函数

指针在函数中可以用来：
1. 通过指针修改函数外部的变量（引用传递）
2. 函数返回多个值
3. 提高大型数据结构传递的效率

```c
#include <stdio.h>

// 通过指针修改变量值
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 指针作为返回值
int* getMax(int *arr, int size) {
    int *max = arr;
    for (int i = 1; i < size; i++) {
        if (*(arr + i) > *max) {
            max = arr + i;
        }
    }
    return max;
}

int main() {
    int x = 10, y = 20;
    printf("交换前: x = %d, y = %d\n", x, y);
    
    swap(&x, &y);
    printf("交换后: x = %d, y = %d\n", x, y);
    
    int numbers[] = {3, 7, 2, 9, 1};
    int *maxPtr = getMax(numbers, 5);
    printf("数组中的最大值: %d\n", *maxPtr);
    
    return 0;
}
```

#### 4.1.6 多级指针

指针也可以指向另一个指针：

```c
#include <stdio.h>

int main() {
    int num = 100;
    int *ptr = &num;      // ptr指向num
    int **pptr = &ptr;    // pptr指向ptr
    
    printf("num的值: %d\n", num);
    printf("通过ptr访问: %d\n", *ptr);
    printf("通过pptr访问: %d\n", **pptr);
    
    return 0;
}
```

指针是C语言强大功能的核心，但也容易出错。使用指针时需要注意：
- 避免使用未初始化的指针 - 未定义行为
- 避免访问已释放的内存 - 未定义行为
- 及时释放动态分配的内存 - 内存泄漏
- 注意指针的类型匹配 - 数据类型出错

#### 4.1.7 函数指针

函数指针用于保存一个函数地址。
- 在汇编中，“函数”的实现是一个需要被传入参数的内存地址。

```c
// 已在外部定义了一个该名称的函数。
int func1(int p);

// 随后，传入它。
int (*pfun)(int) = func1;

// 使用该函数。
*pfun(3);    // 直接调用
(*pfun)(3);  // 通过解引用调用（等价于上面）
```


### 4.2 作用域

作用域为一个变量的存在范围，使用`{}`规定作用域。例如：

```c

int main() {
    int outer = 3;    // 在作用域外定义3
    int* pointer;
    {
        int inner = 5;  // 在作用域内定义5
    }
    int overwrite = 8;
    outer = inner;    // 该操作是不被允许的：命令inner已被释放。
    printf("%d\n", *pointer);
}
```

为什么我要知道“作用域”？
- “作用域”的底层是**栈地址**（涉及系统底层）。
- 在分配数组是不被允许的。

## 5. 控制流

控制流用于控制程序语句的执行顺序。常见的控制流结构包括条件分支、循环和跳转。

### 5.1 条件分支（if / else）

```c
if (condition) {
    // 条件为真时执行
} else if (other_condition) {
    // 其他条件为真时执行
} else {
    // 以上都不满足时执行
}
```

示例：

```c
#include <stdio.h>
int main() {
    int x = 10;
    if (x > 0) {
        printf("x 为正数\n");
    } else if (x == 0) {
        printf("x 等于 0\n");
    } else {
        printf("x 为负数\n");
    }
    return 0;
}
```

### 5.2 多分支（switch）

当根据整型或枚举值选择不同路径时，`switch` 更清晰：

```c
switch (expr) {
    case 0:
        // ...
        break;
    case 1:
        // ...
        break;
    default:
        // 默认分支
}
```

注意：忘记 `break` 会导致“穿透”（fall-through），有时是有意为之。

### 5.3 循环（for / while / do-while）

for 循环：常用于已知次数的迭代。

```c
for (int i = 0; i < n; i++) {
    // 每次迭代
}
```

while 循环：先判断条件再执行。

```c
while (cond) {
    // 当 cond 为真时重复
}
```

do-while：至少执行一次，然后判断条件。

```c
do {
    // 至少执行一次
} while (cond);
```

示例（遍历数组）：

```c
#include <stdio.h>
int main() {
    int arr[] = {1,2,3,4,5};
    for (int i = 0; i < 5; i++) {
        printf("%d\n", arr[i]);
    }
    return 0;
}
```

### 5.4 跳转（break / continue / goto / return）

- `break`：结束最近的循环或 `switch`。
- `continue`：跳过本次循环，进入下一次迭代。
- `goto`：无条件跳转到程序中同一函数内的标签（不推荐滥用，可能导致代码难懂）。
- `return`：从函数返回。

示例（使用 continue 跳过偶数）：

```c
for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) continue;
    printf("奇数: %d\n", i);
}
```

### 5.5 常见陷阱与建议

- 不要在同一表达式中对同一变量进行多次修改（未定义行为），例如 `(i++) + (++i)`。
- 注意整型溢出与有符号/无符号比较。
- 处理循环中的边界条件时，画图或写测试用例帮助验证。

## 6. 文件IO

文件 I/O 是程序与磁盘文件交互的基本手段。标准 C 提供了一套以 `FILE*` 为中心的 API（`fopen`/`fclose`/`fread`/`fwrite`/`fprintf`/`fscanf` 等）。

### 6.1 打开与关闭文件

```c
FILE *f = fopen("path/to/file.txt", "r"); // r: 只读, w: 覆盖写入, a: 追加
if (!f) {
    // 处理打开失败
}
fclose(f);
```

注意：打开文件失败时 `fopen` 返回 NULL，需检查 errno（或 perror）以获取错误原因。

### 6.2 文本 I/O：fprintf / fscanf / fgets / fputs

写文本：

```c
FILE *out = fopen("out.txt", "w");
if (out) {
    fprintf(out, "姓名: %s 年龄: %d\n", "张三", 30);
    fputs("一行文本\n", out);
    fclose(out);
}
```

读文本：

```c
char buf[256];
FILE *in = fopen("out.txt", "r");
if (in) {
    while (fgets(buf, sizeof(buf), in)) {
        printf("读取: %s", buf);
    }
    fclose(in);
}
```

不要使用不安全的 `gets()`；优先使用 `fgets()` 并检查返回值。

`fscanf` 可以按格式读取，但对输入格式非常敏感，读取用户输入或不可信文件时要小心缓冲区溢出。

### 6.3 二进制 I/O：fread / fwrite

当需要读写结构化二进制数据时使用：

```c
struct Point { int x, y; } p = {1,2};
FILE *fb = fopen("pts.bin", "wb");
fwrite(&p, sizeof(p), 1, fb);
fclose(fb);

FILE *fr = fopen("pts.bin", "rb");
struct Point q;
fread(&q, sizeof(q), 1, fr);
fclose(fr);
```

注意：二进制格式与平台（字节序、对齐）相关，不适合跨平台直接交换结构体镜像。

### 6.4 文件定位与文件大小

```c
fseek(f, 0, SEEK_END);
long sz = ftell(f);
fseek(f, 0, SEEK_SET); // 回到文件开头
```

用于获取文件大小或在文件中随机访问。

### 6.5 错误处理

- 使用 `ferror(FILE*)` 和 `feof(FILE*)` 判断错误或文件结束。
- 用 `perror("fopen")` 或 `strerror(errno)` 打印错误信息。

示例：读取整个文件到缓冲区（小文件情况）：

```c
#include <stdio.h>
#include <stdlib.h>

char *read_all(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc(sz + 1);
    if (!buf) { fclose(f); return NULL; }
    if (fread(buf, 1, sz, f) != (size_t)sz) { free(buf); fclose(f); return NULL; }
    buf[sz] = '\0';
    fclose(f);
    return buf;
}
```

### 6.6 小结

- 文本文件适合人读、便于调试；二进制文件更高效但平台相关。
- 总是检查返回值（fopen/fread/fwrite/malloc 等），并在完成后关闭文件和释放资源。

## 7. 结构体（struct）与 typedef

结构体用于把不同类型的数据组合在一起，常用于表示记录或对象样式的数据。

```c
struct Person {
    char name[64];
    int age;
    double height;
};

struct Person p = {"张三", 30, 175.5};
printf("%s %d %.1f\n", p.name, p.age, p.height);
```

可以使用 `typedef` 为类型创建别名，常与结构体一起使用以简化类型名：

```c
typedef struct Person Person; // 给 struct Person 起别名 Person
// 或者同时定义并 typedef：
typedef struct {
    char name[64];
    int age;
} Student;

Student s = {"李四", 20};

// 这里有必要专门写一下函数指针的别名要怎么写：
typedef int (*Processor)(int);    // 定义类型Processor作为函数指针。
```

结构体可以作为函数参数传递（按值）或传递指针以避免拷贝：

```c
void print_person(const Person *p) {
    printf("%s %d\n", p->name, p->age);
}

// 传指针，避免复制
print_person(&p);
```

结构体对齐：不同平台可能有不同的对齐与填充规则。使用 `#pragma pack()` 或编译器特性可以改变对齐，但会影响可移植性。

### 7.1 嵌套结构体与位域

结构体可以嵌套：

```c
typedef struct {
    int r, g, b;
} Color;

typedef struct {
    char title[64];
    Color bg;
} Page;
```

位域用于节省空间，按位定义字段长度：

```c
struct Flags {
    unsigned int a:1;
    unsigned int b:3;
    unsigned int c:4;
};
```

注意：位域在不同编译器/平台上的行为（顺序、对齐）可能不同，不适合跨平台二进制协议。

### 7.2 联合体（Union）

在一个联合体内，其所有成员**共享一块内存空间**，其大小等于最大成员的大小。

```c
// 定义联合体
union Data {
    int i;
    float f;
    char str[20];
};

int main() {
    union Data data;        // 声明联合体变量
    data.i = 10;            // 赋值给i成员
    printf("data.i: %d\n", data.i);
    
    data.f = 220.5;         // 赋值给f成员（此时i的值已被覆盖）
    printf("data.f: %.1f\n", data.f);
    
    strcpy(data.str, "C Programming"); // 赋值给str成员（此时f的值已被覆盖）
    printf("data.str: %s\n", data.str);
    
    return 0;
}
```

在`union`中修改值会影响到其他值。

### 7.3 枚举（Enumerate）

```c
enum Color { RED, GREEN, BLUE };  // RED=0, GREEN=1, BLUE=2
enum Status { SUCCESS = 0, ERROR = -1, WARNING = 1 };

enum Color myColor = GREEN;
if (myColor == GREEN) {
    printf("It's green!\n");
}
```

### 7.4 小插曲：混合类型

`int *(*(*funcptr)(int, int)[4])[2]`
- 这种类型最好别写，直接用typedef：
```c
typedef int (*funcptr)(int, int);
int *((*funcptr[4])[2])
```

## 8. 多线程编程（threading）

标准 C（C11）引入了线程支持（`<threads.h>`），但在实际工程中常用 POSIX threads（pthreads）或在 Windows 上使用 Win32 线程 API。下面给出 POSIX threads（Linux、macOS、MinGW-w64 可用）和 Windows 的基本示例与注意事项。

> 说明：在 Windows 下使用 MSVC 时，通常使用 Windows 线程（CreateThread/_beginthreadex）；在类 Unix 系统使用 pthreads。请根据目标平台选择合适的 API。

### 8.1 POSIX pthreads 示例

```c
#include <stdio.h>
#include <pthread.h>

void *worker(void *arg) {
    int id = *(int*)arg;
    printf("线程 %d 正在运行\n", id);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;
    pthread_create(&t1, NULL, worker, &id1);
    pthread_create(&t2, NULL, worker, &id2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
```

注意：把局部变量地址（例如 `&id1`）传给线程回调时要保证在其他线程访问期间该内存有效，常用做法是为每个线程分配独立的堆内存或使用数组存放参数。

### 8.2 互斥与条件变量

多线程共享数据时必须同步：

```c
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&m);
// 修改共享数据
pthread_mutex_unlock(&m);
```

条件变量用于线程间的等待/通知：

```c
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
// pthread_cond_wait(&cv, &m);
// pthread_cond_signal(&cv);
```

### 8.3 Windows 线程示例（简单示范）

在 Windows 下可使用 Win32 API：

```c
#ifdef _WIN32
#include <windows.h>
#include <process.h>

unsigned __stdcall worker(void *arg) {
    int id = *(int*)arg;
    printf("Win 线程 %d\n", id);
    return 0;
}

// 创建线程：
// uintptr_t h = _beginthreadex(NULL, 0, worker, &id, 0, NULL);
// WaitForSingleObject((HANDLE)h, INFINITE);
#endif
```

### 8.4 常见问题与建议

- 线程安全的内存管理：避免在多个线程并发 free 同一块内存，或者在未同步的情况下访问共享资源。
- 尽量减少锁的持有时间，避免死锁。
- 使用原子操作（C11 的 stdatomic.h 或平台原子）处理简单计数器，以提升性能。
- 在设计多线程程序时将共享数据最小化（尽量使用不可变数据或消息传递）。

小结：多线程编程强大但复杂。先用简单的同步原语验证设计，再逐步优化性能与可伸缩性。




