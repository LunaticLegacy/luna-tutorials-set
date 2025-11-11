## 0. 引言
Github: 月と猫 - LunaNeko https://github.com/LunaticLegacy

CC-BY-NC-SA 4.0

本教程可能较为硬核，适合具有一定计算机基础和编程基础的人食用。如果在食用过程中昏厥或口吐白沫，本人概不负责。

本教程为自行总结，可能存在错误。如有错误问题请通过Repo反馈。

作者使用的编译器为`GCC`，但推荐新人入坑时从visual studio（使用`MSVC`编译器）开始（至少笔者是这样入坑的）。
- 此时如果有MSVC特性请按照MSVC特性处理。

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
float f = 1.0f;     // 单精度浮点数，IEEE 754标准规定
double db = 2.0;    // 双精度浮点数，IEEE 754标准规定
long double ld = 3.0l; // 长双精度浮点数，基于平台
float f = 1.0f;     // 单精度浮点数，IEEE 754标准规定
double db = 2.0;    // 双精度浮点数，IEEE 754标准规定
long double ld = 3.0l; // 长双精度浮点数，基于平台
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

由于不同类型在不同平台的位宽度不一致，如果需要固定宽度的整数类型，请使用头文件`<stdint.h>`。该头文件提供了诸如`int8_t`、`int16_t`、`int32_t`、`int64_t`等固定宽度的整数类型（及其对应的无符号类型，如`uint64_t`）。

定义数字时，可定义不同类型的字面量（Literal）：

| 进制 | 字面量前缀 | 助记 | 示例 | 对应十进制 | 对应二进制 |
| ---- | -------- | -------- | ------- | ------- | ------- |
| 2 | `0b` | Binary | `0b1001` | 9 | 1001 |
| 8 | `0` | Octo | `0147` | 103 | 110 0111 |
| 10 | 无前缀 | Decimal | `102` | 102 | 110 0110 |
| 16 | `0x` | Hex | `0x100000` | 256 | 1 0000 0000 |

- 二进制字面量非标准C语言内容，具体看编译器支持。（至少GCC支持二进制字面量，并且老版本的MSVC不支持二进制字面量）

除了数字字面量外，C语言也支持**字符串字面量**。
- `'A'`和`"A"`的类型不同，前者的类型为`char`，后者的类型为`const char*`。
- 详见[字符串字面量](#15-字符串字面量string-literal)。

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

- 不要听信其他人所谓“不可变变量”的这种玄乎其神的称呼，其实就是const。

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

#### 1.4.1 算术
`+ - * / % ++ -- = += -= *= /= %= ()`
#### 1.4.1 算术
`+ - * / % ++ -- = += -= *= /= %= ()`
- `+`和`-`：加减法。如果最终结果超出了当前数据类型所表示的范围，则会溢出或下溢。
    - 典例：《文明1》中`甘地`的侵略值为`1`，但游戏中保存“侵略值”的变量用的是`unsigned char`。在满足特定条件下，领导人的侵略值会`-2`，此时就可以看到`1-2=255`的名场面——甘地的侵略值现在是`255`，导致他疯狂给印度造核弹，核平全世界。（~~即使这件事后来被证实是个谣言，但这个例子仍然极为有助于记忆~~）
- `*` 和 `/`：乘除法。对于整数和浮点数而言，该操作的行为会有所不同。
- `%`：取模（MOD），仅限整数使用。
- `=`：赋值。**会修改变量**。
    - 用于赋值，例：`double k = 1.2, x = 0.3, b = 0.1; double y = k * x + b`。
- `+=` `-=` `*=` `/=` `%=`：操作并赋值。**会修改变量**。
    - `a += 3`等价于`a = a + 3`，其他以此类推。
    - 用于赋值，例：`double k = 1.2, x = 0.3, b = 0.1; double y = k * x + b`。
- `+=` `-=` `*=` `/=` `%=`：操作并赋值。**会修改变量**。
    - `a += 3`等价于`a = a + 3`，其他以此类推。
- `++`和`--`：自增、自减。**会修改变量**。
    - 后缀该符号时（如`int p = k++`）：先赋值，再执行操作
    - 前缀该符号时（如`int p = ++k`）：先执行操作，再赋值
    - 不要相信`(i++)+(++i)`，能问出这种问题的都是谭浩强C受害者——在同一表达式中多次**修改**同一变量的行为在C语言中属于**未定义行为**，被编译器自行实现。
        - 这个算不算多次修改：`k = k * k + 3`？不算，因为这个表达式只会赋值一次。
- 运算顺序：先括号里，再括号外。先乘除，后加减。（如果这个都不知道的话，只能说你是小学数学没学好）

#### 1.4.2 逻辑与比较
`&& || ! == < <= >= > !=`
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

这一系列操作符一般被用于[控制流](#5-控制流)。

#### 1.4.3 位操作（Bitwise Operation）

位操作：对每一位进行的操作。

`& | ~ << >>`
- `&`：按位与。
    - 对于两个数字的每个二进制位而言，执行与运算。
    - 例：`char a = 3, b = 10, char c = a & b;`
    - `0b0011 & 0b1010 = 0b0010`，此时c的值为2。
    - 对于两个数字的每个二进制位而言，执行与运算。
    - 例：`char a = 3, b = 10, char c = a & b;`
    - `0b0011 & 0b1010 = 0b0010`，此时c的值为2。
- `|`：按位或。
    - 对于两个数字的每个二进制位而言，执行或运算。
    - 例：`char a = 3, b = 10, char c = a | b;`
    - `0b0011 & 0b1010 = 0b1011`，此时c的值为11。
    - 对于两个数字的每个二进制位而言，执行或运算。
    - 例：`char a = 3, b = 10, char c = a | b;`
    - `0b0011 & 0b1010 = 0b1011`，此时c的值为11。
- `~`：按位取反。
    - 对于单个数字的每个二进制位执行非运算。
    - 例：`char k = 10; ~k;`，此时k
- `<<`：左移运算。
- `>>`：右移运算。

关于位移操作：
- 对无符号整数：
    - `x << n` 等价于 `(x * 2^n) mod 2^w`（w是目标无符号类型的位宽）。
    - `x >> n` 为逻辑右移（高位补 0）。
- 对有符号整数：
    - 若左侧操作数为负，或左移导致结果不能用目标类型表示（即溢出），则行为是未定义（UB）。
    - 右移带符号的有符号整数的行为是**实现定义**（implementation-defined，基于编译器行为）：很多实现做算术右移（高位用符号位填充），也有实现做逻辑右移。**但标准不强制哪种**。
        - 如果可以，尽量别右移有符号整数。

#### 1.4.4 内存管理
`* & . [] ->`
- `*`：解指针（解引用），用于根据当前的数字解析到其对应的内存地址。**如果在定义变量时使用该符号，意为“定义指针变量”。详见[指针](#41-指针pointer)。**
    - 例：`int value = *ptr`，为将变量`ptr`数字作为内存地址，解析对应位置上的值，赋给`value`。
- `&`：取地址，用于获取某个特定变量（的开头）所位于的内存地址。
    - 例：`int p = &a`，将变量`a`的所在地址赋给`p`。

符号`*`既可被用于乘法，也可被用于解指针。建议写法如下：`(*var) * 3`。
- *写成`*ptr * 3`，虽然语义上编译器可以解析，但可读性差，容易误解为“取`ptr`的乘法运算符”——尤其在复杂表达式里。*

符号`&`既可被用于取地址，也可被用于按位或。建议写法如下：`(&var) & 4`。
- *实际上对地址做按位运算通常是**危险操作**，可能会导致严重的内存管理问题，进而导致程序崩溃——这个示例仅用于理解运算符的多重含义，不建议在真实代码里使用。*

- `[]`：[数组](#414-指针与数组)索引运算符。
- `.`：通过[结构体](#7-结构体struct与-typedef)变量访问结构体成员。
- `->`：通过[指针](#41-指针pointer)访问结构体成员。
- `()`：强制类型转换（Type Cast），用于将一个表达式转换为指定类型。它常用于不同类型之间的运算、指针操作或者控制数据布局。

对于强制类型转换，语法：`(type)expression`。
- 例：`int* target = (int*)malloc(sizeof(int) * 4);`
    - 其中，`malloc`函数的返回值为`void*`，将该内存块转为`int*`类型，以保存`int`格式的[数组](#数组)。

### 1.5 字符串字面量（String Literal）

这个内容较为特殊。

`const char*`：
- 为`字符串字面量`，定义字符串字面量时使用双引号`""`。
    - 使用`const char*`定义的会被储存在**数据段**中，而不是在栈上。修改该位置属于未定义行为。
- 不推荐使用`char*`直接定义可变字符串字面量——编译器会警告。
    - 可以：`char var[]`，通过数组定义。
    - 也可以：`char* msg = (char*)malloc(sizeof(char)*150)`，动态分配内存以定义。

```c
int main() {
    const char* msg1 = "Hello world!";  // 可以这样。
    char* msg2 = "Hello";   // 极度不推荐这样写。
    char msg3[] = "Hello";  // 可以这样写。
    return 0;
}
```

对`const char*`可正常进行[指针](#41-指针pointer)的[访问](#414-指针与数组)操作，但是**禁止写入**。

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

格式说明符如下：
| 格式 | 对应类型 | 说明 | 注意事项 |
| ---- | ------ | ----- | --------- |
| `%d`/`%i` | `int` | 十进制有符号整数  | `%i`允许处理不同进制输入（主要用于`scanf`） |
| `%u`| `unsigned int` | 十进制无符号整数  | - |
| `%f` | `float`（`printf` 会提升为 `double`） | 浮点数 | - |
| `%lf` | `double` | 浮点数 | `printf` 中 `%f` 和 `%lf` 等效，`scanf` 需用 `%lf` |
| `%Lf` | `long double` | 扩展精度浮点数  | - |
| `%c` | `char`（会被提升为 `int`） | 单个字符 | - |
| `%s` | `char*` | 字符串 | 字符串必须以 `\0` 结尾 |
| `%p` | `void*` | 指针地址 | 输出平台相关（通常是十六进制）|
| `%x`/`%X` | `unsigned int`  | 十六进制整数 | `%X` 输出大写字母 |
| `%o` | `unsigned int` | 八进制整数 | - |
| `%%` | - | 输出 `%` 字符 | - |
| `%lld` | `long long` | 十进制长整型 | C99标准 |
| `%llu` | `unsigned long long` | 十进制长无符号整型 | C99标准 |
| `%hx` / `%hhd` | `short` / `signed char` | 短整型 / 字节型 | 输出需注意类型提升 |
| `%zu`| `size_t` (aka `unsigned long long`) | 大小  | - |

当函数为[变参函数](#37-变参函数)时（`printf`是一个变参函数），会发生**类型提升**：
- `char`(`int8_t`)和`short`(`int16_t`)会被提升为`int`(`int32_t`)。
    - 但是`int`(`int32__t`)不会被再次提升为`int64_t`。
- `float`会被提升为`double`。

忘记匹配格式和类型会导致 未定义行为，尤其是指针和整型类型。

除`printf`外，还有其他输出函数：
- `puts()`：输出字符串，**并在末尾添加换行符**。
    - 如果`printf`内没有规定格式说明符，编译器一般会用`puts`代替`printf`。
- `putc()`：输出单个字符
格式说明符如下：
| 格式 | 对应类型 | 说明 | 注意事项 |
| ---- | ------ | ----- | --------- |
| `%d`/`%i` | `int` | 十进制有符号整数  | `%i`允许处理不同进制输入（主要用于`scanf`） |
| `%u`| `unsigned int` | 十进制无符号整数  | - |
| `%f` | `float`（`printf` 会提升为 `double`） | 浮点数 | - |
| `%lf` | `double` | 浮点数 | `printf` 中 `%f` 和 `%lf` 等效，`scanf` 需用 `%lf` |
| `%Lf` | `long double` | 扩展精度浮点数  | - |
| `%c` | `char`（会被提升为 `int`） | 单个字符 | - |
| `%s` | `char*` | 字符串 | 字符串必须以 `\0` 结尾 |
| `%p` | `void*` | 指针地址 | 输出平台相关（通常是十六进制）|
| `%x`/`%X` | `unsigned int`  | 十六进制整数 | `%X` 输出大写字母 |
| `%o` | `unsigned int` | 八进制整数 | - |
| `%%` | - | 输出 `%` 字符 | - |
| `%lld` | `long long` | 十进制长整型 | C99标准 |
| `%llu` | `unsigned long long` | 十进制长无符号整型 | C99标准 |
| `%hx` / `%hhd` | `short` / `signed char` | 短整型 / 字节型 | 输出需注意类型提升 |
| `%zu`| `size_t` (aka `unsigned long long`) | 大小  | - |

当函数为[变参函数](#37-变参函数)时（`printf`是一个变参函数），会发生**类型提升**：
- `char`(`int8_t`)和`short`(`int16_t`)会被提升为`int`(`int32_t`)。
    - 但是`int`(`int32__t`)不会被再次提升为`int64_t`。
- `float`会被提升为`double`。

忘记匹配格式和类型会导致 未定义行为，尤其是指针和整型类型。

除`printf`外，还有其他输出函数：
- `puts()`：输出字符串，**并在末尾添加换行符**。
    - 如果`printf`内没有规定格式说明符，编译器一般会用`puts`代替`printf`。
- `putc()`：输出单个字符

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

注意：函数**不能**返回[数组](#414-指针与数组)。

注意：函数**不能**返回[数组](#414-指针与数组)。

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

函数可以返回值，也可以不返回值（使用`void`关键字）：
函数可以返回值，也可以不返回值（使用`void`关键字）：

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
    return *(str + 0);  // 为什么要这样？详见“4.1.4 指针与数组”。
    return *(str + 0);  // 为什么要这样？详见“4.1.4 指针与数组”。
}
```

### 3.5 函数声明与定义

在C语言中，函数声明（或“函数签名”）告诉编译器函数的名称、返回类型和参数。函数定义包含函数的实际实现。
在C语言中，函数声明（或“函数签名”）告诉编译器函数的名称、返回类型和参数。函数定义包含函数的实际实现。

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

### 3.7 变参函数

理解本函数需要先理解什么是**ABI**，请自行查阅相关资料。

最经典的变参函数：

```c
void printf(const char *fmt, ...);
```

在定义了一个变参函数时，请使用头文件`<stdarg.h>`以处理参数。用法：

```c
#include <stdio.h>
#include <stdarg.h>

int sum(int n, ...) {
    va_list args;       // 定义一个参数列表
    va_start(args, n);  // 参数列表解析开始

    int total = 0;
    for (int i = 0; i < n; i++) {
        total += va_arg(args, int);  // 将参数列表内的下一个参数解析为指定类型
    }

    va_end(args);   // 参数列表解析结束
    return total;
}

int main() {
    printf("%d\n", sum(4, 10, 20, 30, 40));  // 输出 100
}
```

让我们复杂一点：
```c
// stdio.c
#include <stdarg.h>
#include <stdint.h>

// char* itoa(uint64_t int_target, char base)
// 假设该函数已被定义，该函数功能：将整数转为ascii字符，规定进制。

// 实现一个简单的printf函数，实际上C语言的printf远比该函数实现复杂。
void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    // parse data.
    while (*fmt) {      // 控制流详见第5章。
        if (*fmt == '%') {
            // numbers format, as: d, x, b, o.
            fmt++;
            switch (*fmt) {
                case 'd':  // decimal
                    puts(itoa(va_arg(args, long long), 10));
                    break;
                case 'x':  // hexadecimal
                    puts(itoa(va_arg(args, long long), 16));
                    break;
                case 'b':  // binary
                    puts(itoa(va_arg(args, long long), 2));
                    break;
                case 'o':  // octal
                    puts(itoa(va_arg(args, long long), 8));
                    break;
            }
        } else {
            // just print the character.
            putc(*fmt, 0, 0);
        }
        fmt++;
    }
    va_end(args);
}
```

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
   - 包含全局变量（详见[作用域](#42-作用域)）和静态变量
   - 包含全局变量（详见[作用域](#42-作用域)）和静态变量
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

在**x86（或x86_64）**架构下，内存布局示例：
在**x86（或x86_64）**架构下，内存布局示例：
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

指针是一个整数，其大小为**平台宽度**。
- 平台宽度：在32位平台下，该值为32位，64位同理。

指针的值为另一个变量的地址。基本语法：

指针是一个整数，其大小为**平台宽度**。
- 平台宽度：在32位平台下，该值为32位，64位同理。

指针的值为另一个变量的地址。基本语法：

```c
数据类型 *指针变量名;
// 或者：
数据类型* 指针变量名;
```

在编译器的眼里，上述两种声明方法**完全等价**。但是：

```c
int *a, *b;     // a和b均为指针
int* a, b;      // a是指针，b不是
```

在编译器的眼里，上述两种声明方法**完全等价**。但是：

```c
int *a, *b;     // a和b均为指针
int* a, b;      // a是指针，b不是
```

指针类型控制`从该指针指向的位置开始，接下来的一段内存块应被如何解析`。
- `0x66 0x73 0x6F 0x61`可被解释为4个ASCII字符：`fIoa`，也可以被解释为一个`int32_t`（`1,718,841,185`）或`uint32_t`（`1,718,841,185`）或`int16_t[2]`（`{26227, 28513}`）...
- 如果不对指向的内存规定类型，则数字将毫无意义。
    - 指针的类型可以为`void*`，意为“不表达任何意义的指针”。由于该指针极度自由，以至于可以通过强制类型转换到任意类型。但由于该类型指针无意义，如果需要表达意义则**必须**将其进行强制类型转换。
    - 指针的类型可以为`void*`，意为“不表达任何意义的指针”。由于该指针极度自由，以至于可以通过强制类型转换到任意类型。但由于该类型指针无意义，如果需要表达意义则**必须**将其进行强制类型转换。

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

指针是C语言强大功能的核心，但也容易出错。使用指针时需要注意：
- 避免使用未初始化的指针 - 未定义行为
- 避免访问已释放的内存 - 未定义行为
- 及时释放动态分配的内存 - 内存泄漏
- 注意指针的类型匹配 - 数据类型出错

指针是C语言强大功能的核心，但也容易出错。使用指针时需要注意：
- 避免使用未初始化的指针 - 未定义行为
- 避免访问已释放的内存 - 未定义行为
- 及时释放动态分配的内存 - 内存泄漏
- 注意指针的类型匹配 - 数据类型出错

#### 4.1.2 指针的声明和初始化

```c
#include <stdlib.h>

#include <stdlib.h>

// 声明指针
int *ptr1;        // 声明指向整型的指针
float *ptr2;      // 声明指向浮点型的指针
char *ptr3;       // 声明指向字符的指针

// 声明时初始化
int num = 42;
int *ptr = &num;  // 声明并初始化指向num的指针

// 空指针
int *nullPtr = NULL;  // 或者 int *nullPtr = 0;

// 表达任意类型的指针
void* buffer = malloc(32);

// 表达任意类型的指针
void* buffer = malloc(32);
```

其中，`NULL`是一个宏定义，来自`<stdlib.h>`，其在C中的原型为：`#define NULL ((void *)0)`。
- 访问0地址将导致`SegFault`。（请自行查阅，该错误属于CPU底层级错误）
- 在C++中，`NULL`被直接定义为：`#define NULL 0`，该语言中表示空白指针时需用`nullptr`。

其中，`NULL`是一个宏定义，来自`<stdlib.h>`，其在C中的原型为：`#define NULL ((void *)0)`。
- 访问0地址将导致`SegFault`。（请自行查阅，该错误属于CPU底层级错误）
- 在C++中，`NULL`被直接定义为：`#define NULL 0`，该语言中表示空白指针时需用`nullptr`。

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
    int arr[5] = {10, 20, 30, 40, 50};  // 定义数组
    int arr[5] = {10, 20, 30, 40, 50};  // 定义数组
    
    // 以下三种方式是等价的
    printf("第一种: %d\n", arr[0]); // 访问数组的第一个元素
    printf("第一种: %d\n", arr[0]); // 访问数组的第一个元素
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

访问数组元素可使用操作符`[]`。如：
```c
int main() {
    int arr[5] = {10, 20, 30, 40, 50};  // 定义数组

    int value1 = arr[0];    // 第一个元素
    int value2 = arr[1];    // 第二个元素
    return 0;
}
```
- 为什么是0开头？历史遗留问题——该值一般被解读为“偏移值”，即距离开头偏移多少个单位。

也可以使用[指针运算](#418-指针的运算)：
```c
int main() {
    int arr[5] = {10, 20, 30, 40, 50};  // 定义数组

    int value1 = *(arr + 0);    // 第一个元素
    int value2 = *(arr + 1);    // 第二个元素
    return 0;
}
```

访问数组元素可使用操作符`[]`。如：
```c
int main() {
    int arr[5] = {10, 20, 30, 40, 50};  // 定义数组

    int value1 = arr[0];    // 第一个元素
    int value2 = arr[1];    // 第二个元素
    return 0;
}
```
- 为什么是0开头？历史遗留问题——该值一般被解读为“偏移值”，即距离开头偏移多少个单位。

也可以使用[指针运算](#418-指针的运算)：
```c
int main() {
    int arr[5] = {10, 20, 30, 40, 50};  // 定义数组

    int value1 = *(arr + 0);    // 第一个元素
    int value2 = *(arr + 1);    // 第二个元素
    return 0;
}
```

注意：如果在函数内传入数组，数组长度信息将丢失——此时在函数内部该变量将被解析为**指针**。
- 而由于作用域和堆栈要求，函数**不能直接返回一个数组**，即不得定义如下内容：

```c
int[] error_func() {
    int arr[5] = {10, 20, 30, 40, 50};  // 定义数组
    return arr;
};
```

如果该函数返回了`arr`（类型实际为`int*`），那么当后来再有定义变量时，它将覆盖原数组的位置。该行为为**未定义行为**。
- 而由于作用域和堆栈要求，函数**不能直接返回一个数组**，即不得定义如下内容：

```c
int[] error_func() {
    int arr[5] = {10, 20, 30, 40, 50};  // 定义数组
    return arr;
};
```

如果该函数返回了`arr`（类型实际为`int*`），那么当后来再有定义变量时，它将覆盖原数组的位置。该行为为**未定义行为**。

#### 4.1.5 指针与函数

指针在函数中可以用来：
1. 通过指针修改函数外部的变量（引用传递）
2. 函数返回多个值
3. 提高大型数据结构传递的效率
    - 如果要直接传递一个大型结构体，而不是指向该结构体的指针，那么所有操作都会在栈上执行——可能会导致效率下降。（请参考不同平台的ABI调用约定）
4. （在裸机开发或驱动开发中）依靠强制转换为内存地址，锁死一个内存地址的类型并解析数据。
    - 如果要直接传递一个大型结构体，而不是指向该结构体的指针，那么所有操作都会在栈上执行——可能会导致效率下降。（请参考不同平台的ABI调用约定）
4. （在裸机开发或驱动开发中）依靠强制转换为内存地址，锁死一个内存地址的类型并解析数据。

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

注意：指针函数（Function returns a pointer）和函数指针（Pointer to a function）是两个东西。

#### 4.1.8 指针的运算

对指针的有效计算有：`+`和`-`。其中：
```c
#include <stdio.h>
#include <stdint.h>

int main() {
    int32_t* k = (int32_t*)1000;  // 直接写一个访问内存位置1000（0x3E8）的东西
    int32_t* m = k + 1;     // 从算术上：1000 + 1 * sizeof(int32_t) = 1004
    int32_t* n = k - 1;     // 从算术上：1000 - 1 * sizeof(int32_t) = 996
    printf("%d, %d\n", m, n);  // 1004, 996
}
```

- 指针和数字的加减法：
    - 对内存地址的实际操作**不会**默认以`byte`为单位，而以`指针类型`为单位。
        - 除非定义的指针类型就是`char*`。
    - `-`：向低地址减少若干个指针类型单位。*低地址：数字较小的内存地址。*
    - `+`：向高地址增加若干个指针类型单位。*高地址：数字较大的内存地址。*
    - `++`和`--`：递增或递减1个指针类型单位。
- 指针和指针：
    - `-`：返回两个指针中间隔了多少个元素。
    - `+`：**无效**：指针之间做加法毫无意义。
    - `>`和`<`：指针
- 对`void*`执行加减法是未定义行为。

指针的运算可被用于**数组**运算。

### 4.2 作用域（Scope）
注意：指针函数（Function returns a pointer）和函数指针（Pointer to a function）是两个东西。

#### 4.1.8 指针的运算

对指针的有效计算有：`+`和`-`。其中：
```c
#include <stdio.h>
#include <stdint.h>

int main() {
    int32_t* k = (int32_t*)1000;  // 直接写一个访问内存位置1000（0x3E8）的东西
    int32_t* m = k + 1;     // 从算术上：1000 + 1 * sizeof(int32_t) = 1004
    int32_t* n = k - 1;     // 从算术上：1000 - 1 * sizeof(int32_t) = 996
    printf("%d, %d\n", m, n);  // 1004, 996
}
```

- 指针和数字的加减法：
    - 对内存地址的实际操作**不会**默认以`byte`为单位，而以`指针类型`为单位。
        - 除非定义的指针类型就是`char*`。
    - `-`：向低地址减少若干个指针类型单位。*低地址：数字较小的内存地址。*
    - `+`：向高地址增加若干个指针类型单位。*高地址：数字较大的内存地址。*
    - `++`和`--`：递增或递减1个指针类型单位。
- 指针和指针：
    - `-`：返回两个指针中间隔了多少个元素。
    - `+`：**无效**：指针之间做加法毫无意义。
    - `>`和`<`：指针
- 对`void*`执行加减法是未定义行为。

指针的运算可被用于**数组**运算。

### 4.2 作用域（Scope）

作用域为一个变量的存在范围，使用`{}`规定作用域。例如：

```c
#include <stdio.h>
#include <stdio.h>

int main() {
    int outer = 3;    // 在作用域外定义3
    volatile int* pointer;  // volatile：告诉编译器不要优化掉它
    volatile int* pointer;  // volatile：告诉编译器不要优化掉它
    {
        int inner = 5;  // 在作用域内定义5
        pointer = &inner;   // 写入指针
        pointer = &inner;   // 写入指针
    }
    short overwrite = 8;
    short overwrite = 8;
    printf("%d\n", *pointer);
}
```

为什么我要知道“作用域”？
- “作用域”的底层是**栈地址**（涉及系统底层）。
- 在实际的C语言编程中，如果一个数组需要在其他位置使用，则**不得将其分配在栈内存中**，请使用`malloc`将其分配到堆内存中。

作用域包括**局部作用域**、**全局作用域**及**文件作用域**。

| 类型 | 定义位置 | 可见范围 | 生命周期 | 存储位置  |
| ----- | --- | ------ | ------ | ------- |
| **局部作用域（Local Scope）** | 函数或块 `{}` 内 | 块内可见 | 执行到块结束 | 栈（Stack）          |
| **全局作用域（Global Scope）** | 所有函数外部 | 整个文件可见 | 程序整个运行期 | 数据区（Data Segment） |
| **文件作用域（File Scope）** | 使用`static`限定的全局变量 | 当前文件 | 程序整个运行期 | 全局区 |
| **函数原型作用域**  | 函数声明中参数 | 函数声明时  | 不存在运行期 | 编译时可见 |

例：

```c
// 在main.c中
#include <stdio.h>
#include <stdint.h>

int global_times = 3;   // 此时该变量为全局变量
```

如何在其他文件中访问该变量？使用`extern`关键字。
```c
// another.c
extern int global_times;
```

将`main.c`和`another.c`一起编译，并输出为同一个可执行文件。此时在another.c中也可以实现该内容。

> `extern`关键字也可以用于调度外部函数，只要给出外部函数签名即可。

### 4.3 动态内存分配

动态内存分配的函数有：`malloc` `calloc` `realloc`，及用于释放内存的函数：`free`。
- 需导入`<stdlib.h>`头文件。

其函数原型分别为：
```c
void *malloc(size_t _Size);
void *calloc(size_t _NumOfElements, size_t _SizeOfElements);
void *realloc(void *_Memory, size_t _NewSize);
void free(void *_Memory);
```

其行为分别为：
- `malloc`：分配指定`byte`数的内存。
- `calloc`：给定元素数量和元素大小，并返回分配内存起点。
- `realloc`：重新调整已有内存块大小。如果`_NewSize == 0`，则等价于`free(_Memory)`。
    - 对于上述三个`alloc`系函数而言，如果内存分配失败，它们将返回`NULL`。
- `free`：释放被分配的内存块。
    - 不能对已经被释放的内存再次释放。
例：    
```c
#include <stdlib.h>

int main() {
    int* buffer = (int*)malloc(sizeof(int) * 4);  // 分配一个可容纳4个int的内存
    // 写入数组信息
    *(buffer+0) = 3;
    *(buffer+1) = 33;
    *(buffer+2) = 333;
    *(buffer+3) = 3333;
    // 访问
    int alpha = buffer[0];
    int bravo = buffer[1];
    int charlie = buffer[2];
    int delta = buffer[3];
    // 释放内存
    free(buffer);
    // free(buffer);    // 禁止：操作系统禁止对同一块内存释放两次。
    // int echo = buffer[0];    // 禁止：访问被释放的内存是不被允许的。
    buffer = NULL;  // 在释放内存后的下一步建议立即设NULL
    // int foxroat = buffer[0];  // 禁止：禁止访问NULL。
}
```

上述四个函数将调用操作系统功能以在程序运行期间动态分配内存。

反例（内存泄漏）：
```c
#include <stdlib.h>
#include <pthread.h>  // 见后文“多线程”章节

void unsafeThread() {
    void* consume = malloc(4096);   // 4096 bytes = 4kb，一次来一页的。
    // 只alloc()，不free()。不出几秒系统内存就会被全部吃光光。
}

int main() {
    const size_t THREAD_NUM = 114514;   // 好臭的线程（悲）
    pthread_t threads[THREAD_NUM];  // 如果栈真的能塞下这么大的东西
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_create(&threads[i], NULL, unsafeThread, NULL);
    }
}
```
- 在实际的C语言编程中，如果一个数组需要在其他位置使用，则**不得将其分配在栈内存中**，请使用`malloc`将其分配到堆内存中。

作用域包括**局部作用域**、**全局作用域**及**文件作用域**。

| 类型 | 定义位置 | 可见范围 | 生命周期 | 存储位置  |
| ----- | --- | ------ | ------ | ------- |
| **局部作用域（Local Scope）** | 函数或块 `{}` 内 | 块内可见 | 执行到块结束 | 栈（Stack）          |
| **全局作用域（Global Scope）** | 所有函数外部 | 整个文件可见 | 程序整个运行期 | 数据区（Data Segment） |
| **文件作用域（File Scope）** | 使用`static`限定的全局变量 | 当前文件 | 程序整个运行期 | 全局区 |
| **函数原型作用域**  | 函数声明中参数 | 函数声明时  | 不存在运行期 | 编译时可见 |

例：

```c
// 在main.c中
#include <stdio.h>
#include <stdint.h>

int global_times = 3;   // 此时该变量为全局变量
```

如何在其他文件中访问该变量？使用`extern`关键字。
```c
// another.c
extern int global_times;
```

将`main.c`和`another.c`一起编译，并输出为同一个可执行文件。此时在another.c中也可以实现该内容。

> `extern`关键字也可以用于调度外部函数，只要给出外部函数签名即可。

### 4.3 动态内存分配

动态内存分配的函数有：`malloc` `calloc` `realloc`，及用于释放内存的函数：`free`。
- 需导入`<stdlib.h>`头文件。

其函数原型分别为：
```c
void *malloc(size_t _Size);
void *calloc(size_t _NumOfElements, size_t _SizeOfElements);
void *realloc(void *_Memory, size_t _NewSize);
void free(void *_Memory);
```

其行为分别为：
- `malloc`：分配指定`byte`数的内存。
- `calloc`：给定元素数量和元素大小，并返回分配内存起点。
- `realloc`：重新调整已有内存块大小。如果`_NewSize == 0`，则等价于`free(_Memory)`。
    - 对于上述三个`alloc`系函数而言，如果内存分配失败，它们将返回`NULL`。
- `free`：释放被分配的内存块。
    - 不能对已经被释放的内存再次释放。
例：    
```c
#include <stdlib.h>

int main() {
    int* buffer = (int*)malloc(sizeof(int) * 4);  // 分配一个可容纳4个int的内存
    // 写入数组信息
    *(buffer+0) = 3;
    *(buffer+1) = 33;
    *(buffer+2) = 333;
    *(buffer+3) = 3333;
    // 访问
    int alpha = buffer[0];
    int bravo = buffer[1];
    int charlie = buffer[2];
    int delta = buffer[3];
    // 释放内存
    free(buffer);
    // free(buffer);    // 禁止：操作系统禁止对同一块内存释放两次。
    // int echo = buffer[0];    // 禁止：访问被释放的内存是不被允许的。
    buffer = NULL;  // 在释放内存后的下一步建议立即设NULL
    // int foxroat = buffer[0];  // 禁止：禁止访问NULL。
}
```

上述四个函数将调用操作系统功能以在程序运行期间动态分配内存。

反例（内存泄漏）：
```c
#include <stdlib.h>
#include <pthread.h>  // 见后文“多线程”章节

void unsafeThread() {
    void* consume = malloc(4096);   // 4096 bytes = 4kb，一次来一页的。
    // 只alloc()，不free()。不出几秒系统内存就会被全部吃光光。
}

int main() {
    const size_t THREAD_NUM = 114514;   // 好臭的线程（悲）
    pthread_t threads[THREAD_NUM];  // 如果栈真的能塞下这么大的东西
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_create(&threads[i], NULL, unsafeThread, NULL);
    }
}
```

## 5. 控制流

控制流用于控制程序语句的执行顺序。常见的控制流结构包括条件分支、循环和跳转。

### 5.1 条件分支

`if` / `else if` / `else`
### 5.1 条件分支

`if` / `else if` / `else`

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

当根据整型或枚举值选择不同路径时，`switch`远比一系列的`if-else`更清晰：

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

注意：忘记`break`会导致“穿透”（fall-through）。该特性有时可有意为之。
- 穿透：在`switch`内，自动执行其他`case`项，或`default`项。

### 5.3 循环（for / while / do-while）

for 循环：常用于已知次数的迭代。

```c
for (int i = 0; i < n; i++) {
    // 每次迭代
}
```

`while`循环：先判断条件再执行。

```c
while (cond) {
    // 当 cond 为真时重复，直到不再是真实内容。
}
```

`do-while`：至少执行一次，然后判断条件。

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

示例（使用`continue`跳过偶数）：
示例（使用`continue`跳过偶数）：

```c
for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) continue;
    printf("奇数: %d\n", i);
}
```

示例：


示例：


### 5.5 常见陷阱与建议

- 不要在同一表达式中对同一变量进行多次修改（未定义行为），例如 `(i++) + (++i)`。
- 注意整型溢出与有符号/无符号比较。
- 处理循环中的边界条件时，`画图`或写测试用例帮助验证。

## 6. 文件IO

文件I（Input）/O（Output）是程序与磁盘文件交互的基本手段。标准C提供了一套以 `FILE*` 为中心的 API（`fopen`/`fclose`/`fread`/`fwrite`/`fprintf`/`fscanf` 等）。
- 其中，`FILE*`直接描述文件。
- 引入`<stdio.h>`以使用文件IO。

### 6.1 打开与关闭文件

```c
#include <stdio.h>

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
#include <stdio.h>

FILE *out = fopen("out.txt", "w");
if (out) {
    fprintf(out, "姓名: %s 年龄: %d\n", "张三", 30);
    fputs("一行文本\n", out);
    fclose(out);
}
```

读文本：

```c
#include <stdio.h>

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
#include <stdio.h>

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
#include <stdio.h>

fseek(f, 0, SEEK_END);
long sz = ftell(f);
fseek(f, 0, SEEK_SET); // 回到文件开头
```

用于获取文件大小或在文件中随机访问。

### 6.5 错误处理

- 使用`ferror(FILE*)`和`feof(FILE*)`判断错误或文件结束。
- 用`perror("fopen")`或`strerror(errno)`打印错误信息。
- 使用`ferror(FILE*)`和`feof(FILE*)`判断错误或文件结束。
- 用`perror("fopen")`或`strerror(errno)`打印错误信息。

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

结构体用于把不同类型的数据**组合**在一起，常用于表示记录或对象样式的数据。

```c
struct Person {
    char name[64];
    int age;
    double height;
};

struct Person p = {"张三", 30, 175.5};
printf("%s %d %.1f\n", p.name, p.age, p.height);
```
对于一个结构体而言，其内部的内存是**连续**的。

可以使用`typedef`为类型创建别名，常与结构体一起使用以简化类型名：

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
    pthread_join(t1, NULL); // join函数会阻塞时间，
    pthread_join(t2, NULL);
    return 0;
}
```

注意：把局部变量地址（例如 `&id1`）传给线程回调时要保证在其他线程访问期间该内存有效，常用做法是为每个线程分配独立的堆内存或使用数组存放参数。


### 8.2 互斥、条件变量和锁机制

当多个线程访问共享资源（内存、文件、I/O 等）时，必须使用同步原语以避免数据竞争（data race）和不一致状态。下面按类别介绍常用同步原语、使用模式与示例。

小名词提醒：
- 临界区（critical section）：对共享资源的访问段，需要互斥保护。
- 死锁（deadlock）：多个线程互相等待对方持有的锁，导致无法继续执行。
- 活锁、饥饿：线程不断尝试但无法获得资源或被频繁抢占。

1) 互斥锁（mutex）

- 基本用法（POSIX pthreads）：

### 8.2 互斥、条件变量和锁机制

当多个线程访问共享资源（内存、文件、I/O 等）时，必须使用同步原语以避免数据竞争（data race）和不一致状态。下面按类别介绍常用同步原语、使用模式与示例。

小名词提醒：
- 临界区（critical section）：对共享资源的访问段，需要互斥保护。
- 死锁（deadlock）：多个线程互相等待对方持有的锁，导致无法继续执行。
- 活锁、饥饿：线程不断尝试但无法获得资源或被频繁抢占。

1) 互斥锁（mutex）

- 基本用法（POSIX pthreads）：

```c
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&m);
// 修改共享数据（临界区）
// 修改共享数据（临界区）
pthread_mutex_unlock(&m);
```

- 属性与变种：
    - 默认（fast）mutex：快速但对死锁检测有限；在不同实现上行为略有差异。
    - 递归（recursive）mutex：同一线程可重复加锁多次（需对应次数解锁）。使用场景：递归函数或在持锁时调用可能再次获得锁的库函数。
        ```c
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_t rm;
        pthread_mutex_init(&rm, &attr);
        ```
    - robust（可靠）mutex：当持锁线程崩溃时，其他线程可以检测并接手恢复（非所有平台都支持）。

- 错误处理：始终检查返回值（尤其在初始化、加锁/解锁时）。在可能抛出异常或线程中断的环境中，尽量使用 RAII 风格包装（C++）或确保 finally/cleanup 路径释放锁。

2) 读写锁（rwlock）

- 读写锁允许多个读线程并发访问，但写线程独占访问，适用于读多写少场景：

```c
pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
// 读者：
pthread_rwlock_rdlock(&rw);
// 读取共享数据
pthread_rwlock_unlock(&rw);

// 写者：
pthread_rwlock_wrlock(&rw);
// 修改共享数据
pthread_rwlock_unlock(&rw);
```

- 注意事项：读写锁可引起写者饥饿（长期读者阻塞写者）。可根据实现选择公平策略或使用读者优先/写者优先变体。

3) 自旋锁（spinlock）

- 自旋锁在循环中忙等待（busy-wait），适合临界区非常短且线程切换成本高（/内核不可抢占短操作）的场景。

```c
#include <pthread.h>
pthread_spinlock_t s;
pthread_spin_init(&s, PTHREAD_PROCESS_PRIVATE);
pthread_spin_lock(&s);
// 临界区（短）
pthread_spin_unlock(&s);
pthread_spin_destroy(&s);
```

- 自旋锁不要用于可能被阻塞/睡眠的场景，否则会浪费 CPU 并可能导致活锁。

4) 信号量（semaphore）

- POSIX 信号量既可以用于进程间（named semaphore）也可用于线程间（unnamed semaphore），常用于计数资源或实现生产者-消费者模型：

```c
#include <semaphore.h>
sem_t sem;
sem_init(&sem, 0, 0); // pshared=0 表示线程间

// 生产者：发布
sem_post(&sem);

// 消费者：等待
sem_wait(&sem);

sem_destroy(&sem);
```

- 注意：sem_wait 可阻塞线程，sem_trywait 可做非阻塞尝试。

5) 条件变量（condition variable）——等待/通知模式

- 条件变量用于在线程间等待某个条件（通常与互斥锁配合）：
- 属性与变种：
    - 默认（fast）mutex：快速但对死锁检测有限；在不同实现上行为略有差异。
    - 递归（recursive）mutex：同一线程可重复加锁多次（需对应次数解锁）。使用场景：递归函数或在持锁时调用可能再次获得锁的库函数。
        ```c
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_t rm;
        pthread_mutex_init(&rm, &attr);
        ```
    - robust（可靠）mutex：当持锁线程崩溃时，其他线程可以检测并接手恢复（非所有平台都支持）。

- 错误处理：始终检查返回值（尤其在初始化、加锁/解锁时）。在可能抛出异常或线程中断的环境中，尽量使用 RAII 风格包装（C++）或确保 finally/cleanup 路径释放锁。

2) 读写锁（rwlock）

- 读写锁允许多个读线程并发访问，但写线程独占访问，适用于读多写少场景：

```c
pthread_rwlock_t rw = PTHREAD_RWLOCK_INITIALIZER;
// 读者：
pthread_rwlock_rdlock(&rw);
// 读取共享数据
pthread_rwlock_unlock(&rw);

// 写者：
pthread_rwlock_wrlock(&rw);
// 修改共享数据
pthread_rwlock_unlock(&rw);
```

- 注意事项：读写锁可引起写者饥饿（长期读者阻塞写者）。可根据实现选择公平策略或使用读者优先/写者优先变体。

3) 自旋锁（spinlock）

- 自旋锁在循环中忙等待（busy-wait），适合临界区非常短且线程切换成本高（/内核不可抢占短操作）的场景。

```c
#include <pthread.h>
pthread_spinlock_t s;
pthread_spin_init(&s, PTHREAD_PROCESS_PRIVATE);
pthread_spin_lock(&s);
// 临界区（短）
pthread_spin_unlock(&s);
pthread_spin_destroy(&s);
```

- 自旋锁不要用于可能被阻塞/睡眠的场景，否则会浪费 CPU 并可能导致活锁。

4) 信号量（semaphore）

- POSIX 信号量既可以用于进程间（named semaphore）也可用于线程间（unnamed semaphore），常用于计数资源或实现生产者-消费者模型：

```c
#include <semaphore.h>
sem_t sem;
sem_init(&sem, 0, 0); // pshared=0 表示线程间

// 生产者：发布
sem_post(&sem);

// 消费者：等待
sem_wait(&sem);

sem_destroy(&sem);
```

- 注意：sem_wait 可阻塞线程，sem_trywait 可做非阻塞尝试。

5) 条件变量（condition variable）——等待/通知模式

- 条件变量用于在线程间等待某个条件（通常与互斥锁配合）：

```c
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

// 等待线程：
pthread_mutex_lock(&m);
while (!predicate()) { // 使用 while 而不是 if，防止虚假唤醒
        pthread_cond_wait(&cv, &m); // atomically unlock m and wait
}
// predicate 满足，处理共享数据
pthread_mutex_unlock(&m);

// 通知线程：
pthread_mutex_lock(&m);
// 更新状态，使 predicate() 为真
pthread_cond_signal(&cv);    // 唤醒一个等待者
// 或者 pthread_cond_broadcast(&cv); // 唤醒所有等待者
pthread_mutex_unlock(&m);
```

- 关键点：
    - `pthread_cond_wait` 调用会在原子性地解锁互斥锁后进入等待，返回时会重新加锁。必须在加锁状态下调用 `pthread_cond_wait`。
    - 用 `while` 循环检查条件以应对虚假唤醒或多个线程竞争唤醒的情况。
    - `pthread_cond_signal` 只是通知一个等待线程；若多个线程需同时继续，应使用 `pthread_cond_broadcast`。

6) 避免死锁的常见策略

- 按固定顺序加锁（lock ordering）。
- 使用 trylock（非阻塞尝试）并在失败时回退释放已持有的锁然后重试或采取其他策略。
- 限制锁的粒度与持有时间，尽量缩小临界区。
- 使用更高级的并发数据结构或无锁算法（lock-free）以减少锁竞争。

7) C11 原子（stdatomic.h）简介

- C11 标准提供了原子类型和原子操作，用于实现无数据竞争的并发访问。核心头文件为 `<stdatomic.h>`。

简单示例：

```c
#include <stdatomic.h>
atomic_int a = 0;
atomic_fetch_add(&a, 1); // 原子加
int v = atomic_load(&a); // 原子读
atomic_store(&a, 5);     // 原子写
```

- memory_order（内存序）常用选项：
    - `memory_order_relaxed`：仅保证原子性，不保证跨线程的可见性顺序。
    - `memory_order_acquire`：在加载操作上使用，保证随后的读/写不会被重排序到 acquire 之前。
    - `memory_order_release`：在存储操作上使用，保证之前的读/写不会被重排序到 release 之后。
    - `memory_order_acq_rel`：load+store 的复合语义（用于 read-modify-write 操作）。
    - `memory_order_seq_cst`：最严格的序，提供全序一致性（默认）。

示例（release-acquire 通信）：

```c
atomic_int flag = 0;
int data = 0;

// 生产者
data = 42;                     // 普通写数据
atomic_store_explicit(&flag, 1, memory_order_release);

// 消费者
if (atomic_load_explicit(&flag, memory_order_acquire) == 1) {
        // 现在可以安全读取 data
        printf("data = %d\n", data);
}
```

- 使用原子变量可避免某些情形下的锁，但需要理解内存序和可见性语义；错误使用会导致难以察觉的并发 bug。

8) 小结与推荐实践

- 优先使用互斥锁 + 条件变量实现常见的等待/通知逻辑；当性能成为瓶颈时分析是否使用读写锁或自旋锁（仅限短临界区）。
- 对跨进程同步，使用 POSIX 命名信号量或系统级同步原语（如 futex、Win32 同步对象）。
- 当需要高性能并发结构时，学习 C11 原子和 lock-free 算法；在不熟悉内存序时优先使用 `memory_order_seq_cst`。


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


## 9. 预处理指令（Preprocessing）

预处理指令的开头为`#`，前文中使用的`#include`也属于预处理指令。

### 9.0 编译过程

一般人们认为的广义的“编译”过程，实际由如下四个过程完成：

| 阶段 | 作用 |
| ------ | --- |
| **预处理（Preprocessing）** | 展开宏、包含文件、条件编译 |
| **编译（Compilation）** | 把预处理后的代码翻译成（对应平台的）汇编语言 |
| **汇编（Assembly）** | 把汇编转成机器码（目标文件） |
| **链接（Linking）** | 把多个目标文件合并为可执行程序 |

对于纯粹的C语言编程而言，现代的C工具链（如gcc、clang、msvc）都同时包含上述4个步骤的工具，因此该流程在大多数情况下被直接抽象为“编译”。
- 除非是裸机开发，否则不需要手动处理汇编——裸机开发一般需要手写汇编。

如果想要看在经过预处理环节后，程序到底变成了什么样子，请在编译器中加入参数`-E`。
```
gcc -E main.c -o main.i
```

### 9.1 `#include`

该预处理指令为将指定文件包含在该位置里，实际等价于复制粘贴。
- 这个程序真的可以过编译：

```c
// header1.h
}
```
 
```c
// main.c
int main() {
    return 0;
#include "header1.h"
```

`#include`导入文件的方式有两种：
- `"needing_header.h"`，先相对于当前文件搜索，如果找不到则搜索系统路径
- `<needing_header.h>`，搜索系统路径（使用编译器参数`-I <path>`规定路径）

### 9.2 控制流系列

#### 9.2.1 条件跳转类

`#if` `#elif` `#else` `#endif`
- 该类主要用于控制在何种条件下跳转。
- `#ifndef xx`等价于`#if !defined(xx)`，`#ifdef xx`等价于`#if defined(xx)`。
- 例：

```c
#ifdef __WIN32
// 如果宏__WIN32被定义，下文有效（在编译时将被加入编译），否则下文无效（不会被加入编译）

#elif defined(__linux__)
// 如果宏__linux__被定义，下文有效

#else
// 如果以上条件都不满足，下文有效

#endif  // 使用endif，结束if控制流
```
> “下文有效”意为“本段内容将被加入编译”，且“宏”（Macro）的作用可被视为环境。


#### 9.2.2 定义类

`#define` `#undef`
- 用于定义或取消定义某个宏。例：
```c
#define ALPHA 1
#define BETA 2

int main() {
    int k = BETA;  // 会被自动替换为2
    return 0;
}

#undef BETA

int m = BETA;   // 这个变量无效，因为BETA已经被取消定义。
```
此时在经过预处理后，BETA会被自动替换为2。

#### 9.2.3 实践：“宏定义保护”——头文件的必备技巧

基于上述两条内容，让我们写一个头文件：

```c
// fibo.h
// 写一个宏保护，以免在多个文件同时引用该头文件时，因为函数被重复定义而发生编译错误。
// 宏的名字可以自定义，但这里推荐和头文件名称相关。
#ifndef FIBO_H
#define FIBO_H

int fibonacci(int n) {
    if (n < 2) return n;      // 没错如果if后面的东西很简短也可以这样写 
    return fibonacci(n - 1) + fibonacci(n - 2);
}

#endif // FIBO_H
```

### 9.3 `#pragma`

`#pragma`为使用编译器功能，用于调用编译器特定功能或设置优化选项。
- 它不属于标准C语言的一部分，并且：**不同编译器对`#pragma`可能有不同支持**。
- 本段内容的`#pragma`可能不完全，具体请参考自己的编译器。（如果你想手搓编译器当我没说）

常用的有：

#### 9.3.1 `#pragma once`

告诉编译器这个头文件只导入一次，**等价于[宏定义保护](#923-实践时间)**。

#### 9.3.2 `#pragma warning()`

用于处理编译器警告。
- 注意：该指令对于不同编译器的实现不同。
    - 对于MSVC：
    ```c
    #pragma warning(disable: 4996)  // 禁用4996号警告
    ```
    - 对于GCC：
    ```c
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-variable"  // 忽略未使用变量警告
    int x;
    #pragma GCC diagnostic pop
    ```

#### 9.3.3 `#pragma pack()`

控制**内存对齐**的。
- 内存对齐（Memory Alignment）：由于CPU读取时按固定位宽进行读取，将数据按照位宽对齐时，可避免同一条数据需被CPU读取两次及以上，导致运行效率降低。

语法：

```c
#pragma pack(n)      // 设置对齐为n字节
#pragma pack()       // 恢复（当前编译器）默认对齐
#pragma pack(push, n) // 保存当前对齐状态并设置n
#pragma pack(pop)    // 恢复上一次保存的对齐状态
```
其中，n一般为2的次方项，且单位为`byte`。

例：

```c
#include <stdio.h>

#pragma pack(push, 1)  // 1字节对齐
struct MyStruct {
    char a;
    int b;
};
#pragma pack(pop)

int main() {
    printf("sizeof(MyStruct) = %zu\n", sizeof(struct MyStruct));  // 5字节
    return 0;
}
```

经实验，最大对齐内容**不会超过**struct内的最大元素的大小。也就是说：
```c
#pragma pack(push, 16)  // min(4, 16) = 4
struct ItsStruct {
    char a;
    int b;
};
#pragma pack(pop)
```

等价于：

```c
#pragma pack(push, 4)
struct HisStruct {
    char a;
    int b;
};
#pragma pack(pop)
```

## 附录

### 术语解释

未定义行为（Undefined Behaviour，UB）：
- 当程序执行了标准中没有规定结果的操作时，程序的行为**完全不可预测**。
- 常见例：
    - 数组越界访问
    ```c
    int arr[3] = {0, 2, 3};  // 最大索引为arr[2]
    int varError = arr[3];   // 卒 - 超索引
    ```
    - 解引用空指针
    ```c
    int *nullptr = (int*)NULL;
    int var = *nullptr;     // 卒 - SegFault
    ```
    - 访问被释放的内存
        - 栈内存版（超作用域）
        ```c
        int outer = 3;    // 在作用域外定义3
        {
            int inner = 5;  // 在作用域内定义5
        }
        int new = inner;    // 卒 - 不在inner的作用域内
        ```
        - 堆内存版
        ```c
        int *ptr = (int*)malloc(sizeof(int));
        free(ptr);
        int value = *ptr;   // 卒 - 内存已被释放，不得访问
        ```
    - 对已被释放的内存再次释放
    ```c
    int *ptr = (int*)malloc(sizeof(int));
    free(ptr);
    free(ptr);  // 卒 - 内存已被释放，再次释放会被OS报错
    ```
    - 使用未初始化的变量
    ```c
    int x;
    printf("%d\n", x);  // 卒 - x未被初始化——强行执行会让x随机输出[-2147483648, 2147483647]之间的任何一个数字
    ```
    - 有符号整数溢出
    ```c
    #include <limits.h>
    int x = INT_MAX;    // 宏定义在limits.h，#define INT_MAX 2147483647
    x = x + 1;  // 卒 - 有符号整数溢出
    ```
    - 修改字符串字面量
    ```c
    char* str = "Hello";    // 会被编译器搞成const char*，而字符串字面量在data段内
    str[0] = 'A';   // 卒 - SegFault
    ```


