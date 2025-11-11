## 0. 引言
Github: 月と猫 - LunaNeko https://github.com/LunaticLegacy

CC-BY-NC-SA 4.0

本教程可能较为硬核，适合具有一定计算机基础和编程基础的人食用。如果在食用过程中昏厥或口吐白沫，本人概不负责。

本教程为自行总结，可能存在错误。如有错误问题请通过Repo反馈。

本教程使用`GNU ARM`。

### 0.1 操作符号

## 1. ARM CPU架构：

概述：
- ARM（Advanced RISC Machine）是一种基于精简指令集计算（RISC）原理的CPU架构，最初由Acorn Computers于1983年开发。ARM架构以其低功耗、高性能和可扩展性而闻名，广泛应用于移动设备、嵌入式系统、物联网设备、服务器等领域。
- 上面这个扯淡的可以直接删掉，总之ARM是RISC流派的CPU（但ARM后来恶堕了，接纳了CISC的一些特性。）

### 1.1 寄存器（32位架构下）

#### 1.1.1 32位寄存器概述

32位架构下，ARM共计有37个寄存器，包括31个通用寄存器和6个状态寄存器。

| 寄存器名 | 速记 | 寄存器功能 | 特殊情况 |
| -------- | ------ | --------- | -------- |
| `r{n}` | Register \{n\} | 通用寄存器 | `r13~r15`有特殊用途。 |
| `r13` | Stack Pointer | 栈指针，指向当前栈顶。相当于`rsp` |
| `r14` | Link Register | 链接指针，储存在**调用**后的返回地址 |
| `r15` | Program Counter | 程序计数器，下一条指令的地址 | - |
| `CPSR` | Current Program Status Register | 当前程序状态寄存器，包含条件标志、控制位等 | 包含N、Z、C、V等条件标志位 |
| `SPSR` | Saved Program Status Register | 保存的程序状态寄存器，用于异常模式 | 在异常发生时保存CPSR的值 |

- `n in range(1, 1+15)`。

#### 1.1.2 影子寄存器 / 银行寄存器（Bank Register）

| 寄存器 | 常见共享 | 特殊/模式专用 |
| ------ | ------ | ------ |
| r0–r7 | 共用 | — |
| r8–r12 | 共用（除FIQ） | 在FIQ模式下，有单独 r8_fiq–r12_fiq |
| r13 (SP) | 共用/覆盖 | 各模式有各自 r13_xxx |
| r14 (LR) | 共用/覆盖 | 各模式有各自 r14_xxx |
| r15 (PC) | 全局 | — |
| CPSR | 全局 | —  |
| SPSR (模式专用) | — | SPSR_irq, SPSR_fiq, SPSR_svc, … |

### 1.2 寄存器（64位架构下）

ARM的64位架构称为AArch64，使用A64指令集。与32位架构相比，它提供了更多的寄存器和更大的寻址空间。

#### 1.2.1 64位寄存器概述

AArch64提供了31个64位通用寄存器，命名为X0-X30。每个64位寄存器还有一个对应的32位视图，命名为W0-W30。

| 寄存器名 | 速记 | 寄存器功能 | 特殊情况 |
| -------- | ------ | --------- | -------- |
| `X{n}`/`W{n}` | Register \{n\} | 64位/32位通用寄存器 | X30用作链接寄存器 |
| `X29`/`W29` | Frame Pointer | 帧指针 | 相当于x86的rbp |
| `X30`/`W30` | Link Register | 链接寄存器，存储函数返回地址 | 相当于x86的rip（间接） |
| `SP` | Stack Pointer | 栈指针 | 专用寄存器，不是通用寄存器 |
| `PC` | Program Counter | 程序计数器 | 不能直接访问 |
| `CPSR` | Current Program Status Register | 当前程序状态寄存器 | 包含N、Z、C、V等条件标志位 |

- `n in range(0, 0+30)`

与32位版本不同，AArch64没有影子寄存器。异常处理通过软件方式保存和恢复寄存器状态，而不是使用硬件分组寄存器。

#### 1.2.2 64位架构特点

1. **寄存器数量增加**：从16个增加到31个通用寄存器，减少了对栈的依赖
2. **寄存器宽度增加**：从32位扩展到64位
3. **PC寄存器不可直接访问**：提高了安全性和代码健壮性
4. **SP为专用寄存器**：不能作为通用寄存器使用
5. **统一寄存器模型**：没有影子寄存器，异常处理方式不同

## 2. GNU ASM教程

### 2.1 内存IO

| 指令 | 速记 | 用途 | 示例代码 | 示例释义 | 架构版本 |
| ------ | ------ | ---- | ------ | ----- | -------- |
| `MOV` | Move | 移动立即数或另一个寄存器的内容到目标寄存器 |
| `LDR`  | LoaD Register| 从内存读取数据到寄存器 | `LDR R1, [R0, #0x12]`| 将地址 `R0+0x12` 处的32位数据读出，保存到 `R1`。 | ARMv4+ (A32)，ARMv8-A (A64) |
| `STR`  | STore Register  | 将寄存器数据写入内存 | `STR R0, [R1, #8]` | 将寄存器 `R0` 的值存储到地址 `R1+8` 处。| ARMv4+ (A32)，ARMv8-A (A64) |
| `PUSH` | Push | 将寄存器列表推入栈（存储到内存） | `PUSH {R4, R5}`  | 将 `R4` 和 `R5` 的值压入栈顶（等价于 `STMDB SP!, {R4,R5}`）。 | ARMv4+ (A32) |
| `POP` | Pop | 将栈顶数据弹出到寄存器列表 | `POP {R4, R5}` | 从栈顶加载数据到 `R4` 和 `R5`（等价于 `LDMIA SP!, {R4,R5}`）。| ARMv4+ (A32) |
| `STP` | STore Pair | 将两个寄存器的值存储到连续内存位置（常用于栈）  | `STP X29, X30, [SP, #-16]!` | 将 `X29` 和 `X30` 的值存入栈顶（SP 减少 16 字节）。 | ARMv8-A (A64)              |
| `LDP` | LoaD Pair | 从连续内存位置加载两个寄存器（常用于栈） | `LDP X29, X30, [SP], #16`   | 从栈顶加载值到 `X29` 和 `X30`，并将 SP 增加 16 字节。 | ARMv8-A (A64)|
| `ADR` | Address   | 生成标签地址的低位并载入寄存器（PC 相对）| `ADR R0, label` | 计算标签 `label` 的地址（PC+offset），加载到 `R0`。| ARMv6T2+ (A32) |
| `ADRP` | Address Page | 生成标签所在4KB页基地址并载入寄存器（A64） | `ADRP X0, label` | 计算标签 `label` 所在4KB页的基地址，加载到 `X0`。| ARMv8-A (A64) |


### 2.2 算术类
| 指令 | 速记 | 用途 | 示例代码 | 示例释义 | 架构版本 |
| ------ | ----- | ----- | ---- | ----- | ----- |
| `ADD`  | Add | 两寄存器或寄存器与立即数相加 | `ADD R0, R1, R2` | 将 `R1 + R2` 的结果加到 `R0`（不更新标志位）。 | ARMv4+           |
| `ADC`  | Add with inC | 带进位加 `Rn + Rm + C` | `ADC R0, R1, R2` | 将 `R1 + R2 + Carry` 的结果加到 `R0`（用于多字算术）。 | ARMv4+ |
| `SUB`  | Subtract | 两寄存器或寄存器与立即数相减 | `SUB R0, R1, R2` | 将 `R1 - R2` 的结果存入 `R0`（不更新标志位）。 | ARMv4+ |
| `SBC`  | Subtract with deC | 带借位减 `Rn - Rm - ¬C`    | `SBC R0, R1, R2` | 将 `R1 - R2 - (NOT Carry)` 的结果存入 `R0`（用于多字算术）。        | ARMv4+           |
| `QADD` | Q Add | 两寄存器相加结果饱和（32位范围）      | `QADD R0, R1, R2` | 将 `R1 + R2` 做饱和加法，结果限于32位带符号范围（溢出时设Q标志）。 | ARMv6+ (ARMv7-A) |
| `QSUB` |  Q Subtract  | 两寄存器相减结果饱和（32位范围） | `QSUB R0, R1, R2` | 将 `R1 - R2` 做饱和减法，结果限于32位带符号范围（溢出时设Q标志）。 | ARMv6+ (ARMv7-A) |
| `MUL`  | Multiple | 32位寄存器乘法，结果低32位 | `MUL R0, R1, R2` | 将 `R1 * R2` 的低32位结果存入 `R0`。 | ARMv4+ |
| `MLA`  | Multiple-LinearAdd | `Rn*Rm + Ra` 乘加，结果低32位 | `MLA R0, R1, R2, R3` | 将 `R1 * R2 + R3` 的低32位结果存入 `R0`。 | ARMv4+ |
| `SDIV` | Symboled Division | 两寄存器带符号整数除法 | `SDIV R0, R1, R2` | 将 `R1 (signed) / R2 (signed)` 的结果存入 `R0`（ARMv7+ 新增）。 | ARMv7+ (A32) |
| `UDIV` | Unsymboled Division | 两寄存器无符号整数除法 | `UDIV R0, R1, R2`    | 将 `R1 / R2` 的无符号商存入 `R0`（ARMv7+ 新增）。 | ARMv7+ (A32) |

### 2.3 跳转类
| 指令 | 速记 | 用途 | 示例代码 | 示例释义 | 架构版本 |
| -------- | --------- | ------- | --- | ----- | ------- |
| `B` | 跳转 | 无条件跳转至标签 | `B loop` | 跳转到标签 `loop` 处继续执行，无条件分支。 | ARMv4+  |
| `B.cond` | 条件跳转 | 按条件跳转至标签 | `BNE label` | 如果上一次结果**不等**（Z=0），则跳转到 `label`；否则继续。 | ARMv4+  |
| `BL` | 带链接跳转（调用） | 调用子程序（保存返回地址） | `BL func` | 调用函数 `func`：将下一条指令地址保存在 `LR`，然后跳转执行 `func`。 | ARMv4+  |
| `BLR` | 带链接跳转寄存器  | 跳转至寄存器指定地址并调用子程序 | `BLR X1` | 跳转到寄存器 `X1` 指向的地址并执行，将返回地址保存在 `LR`（A64 新增）。 | ARMv8-A |
| `BR` | 寄存器跳转 | 无条件跳转至寄存器指定地址 | `BR X0` | 跳转到寄存器 `X0` 指向的地址（无链接），用于 A64 的间接跳转。 | ARMv8-A |

跳转类地址将直接改变寄存器`PC`。

### 2.4 逻辑运算类
| 指令 | 速记 | 用途 | 示例代码 | 示例释义 | 架构版本 |
| ---- | ---- | ---- | -------- | -------- | -------- |
| `AND` | AND | 按位与操作 | `AND R0, R1, R2` | 将 `R1` 和 `R2` 进行按位与操作，结果存入 `R0` | ARMv4+ |
| `BIC` | Bitwise bit Clear | 位清除（按位与非） | `BIC R0, R1, R2` | 将 `R1` 和 `R2` 的反码进行按位与操作，结果存入 `R0` | ARMv4+ |
| `EOR` | Exclusive OR | 按位异或操作 | `EOR R0, R1, R2` | 将 `R1` 和 `R2` 进行按位异或操作，结果存入 `R0` | ARMv4+ |
| `ORR` | OR | 按位或操作 | `ORR R0, R1, R2` | 将 `R1` 和 `R2` 进行按位或操作，结果存入 `R0` | ARMv4+ |
| `MVN` | MoVe Not | 按位取反 | `MVN R0, R1` | 将 `R1` 的每一位取反，结果存入 `R0` | ARMv4+ |
| `ORN` | OR NOT | 按位或非操作 | `ORN R0, R1, R2` | 将 `R1` 和 `R2` 的反码进行按位或操作，结果存入 `R0` | ARMv6+ |

### 2.5 比较和测试类
| 指令 | 速记 | 用途 | 示例代码 | 示例释义 | 架构版本 |
| ---- | ---- | ---- | -------- | -------- | -------- |
| `CMP` | CoMPare | 比较两个值（更新标志位） | `CMP R0, R1` | 比较 `R0` 和 `R1` 的值，更新标志位（相当于执行 `SUBS R0, R0, R1` 但不保存结果） | ARMv4+ |
| `CMN` | CoMPare Negative | 比较负值（更新标志位） | `CMN R0, R1` | 比较 `R0` 和 `-R1` 的值，更新标志位（相当于执行 `ADDS R0, R0, R1` 但不保存结果） | ARMv4+ |
| `TST` | TeST | 测试位（更新标志位） | `TST R0, R1` | 测试 `R0` 和 `R1` 的按位与结果，更新标志位（相当于执行 `ANDS R0, R0, R1` 但不保存结果） | ARMv4+ |
| `TEQ` | Test EQual | 测试相等（更新标志位） | `TEQ R0, R1` | 测试 `R0` 和 `R1` 的按位异或结果，更新标志位（相当于执行 `EORS R0, R0, R1` 但不保存结果） | ARMv4+ |

### 2.6 移位操作类
| 指令 | 速记 | 用途 | 示例代码 | 示例释义 | 架构版本 |
| ---- | ---- | ---- | -------- | -------- | -------- |
| `LSL` | Logical Shift Left | 逻辑左移 | `LSL R0, R1, #2` | 将 `R1` 的值左移 2 位，结果存入 `R0` | ARMv4+ |
| `LSR` | Logical Shift Right | 逻辑右移 | `LSR R0, R1, #2` | 将 `R1` 的值右移 2 位，结果存入 `R0` | ARMv4+ |
| `ASR` | Arithmetic Shift Right | 算术右移 | `ASR R0, R1, #2` | 将 `R1` 的值算术右移 2 位，结果存入 `R0` | ARMv4+ |
| `ROR` | ROtate Right | 循环右移 | `ROR R0, R1, #2` | 将 `R1` 的值循环右移 2 位，结果存入 `R0` | ARMv4+ |
| `RRX` | Rotate Right with eXtend | 带进位循环右移 | `RRX R0, R1` | 将 `R1` 的值与进位标志一起循环右移 1 位，结果存入 `R0` | ARMv4+ |

## 3. 标志寄存器（Flags Registers）

ARM架构中的标志寄存器称为**CPSR**（Current Program Status Register），是一个32位的寄存器，包含了条件标志位、中断禁止位和处理器模式位等。

### 3.1 条件标志位
| 标志位 | 速记 | 位数 | 描述 |
|--------|------|------|------|
| N | Negative Flag | 31 | 负数标志：运算结果为负时置1 |
| Z | Zero Flag | 30 | 零标志：运算结果为零时置1 |
| C | Carry Flag | 29 | 进位标志：无符号运算中出现进位或借位时置1 |
| V | Overflow Flag | 28 | 溢出标志：有符号运算溢出时置1 |

### 3.2 控制位
| 标志位 | 速记 | 位数 | 描述 |
|--------|------|------|------|
| Q | Cumulative saturation flag | 27 | 饱和标志：DSP指令饱和时置1 |
| IT[1:0] | If-Then state bits | 26:25 | Thumb IT（If-Then）块状态 |
| J | Jazelle bit | 24 | Jazelle扩展使能位 |
| GE[3:0] | Greater than or Equal flags | 19:16 | SIMD指令比较结果标志 |
| IT[7:2] | If-Then state bits | 15:10 | Thumb IT（If-Then）块状态 |
| E | Endianness bit | 9 | 端序控制位：0为小端序，1为大端序 |
| A | Asynchronous abort mask | 8 | 异步中止中断禁止位 |
| I | IRQ mask | 7 | IRQ中断禁止位 |
| F | FIQ mask | 6 | FIQ中断禁止位 |
| T | Thumb bit | 5 | Thumb状态位：1为Thumb状态，0为ARM状态 |
| M[4:0] | Mode bits | 4:0 | 处理器模式位 |

### 3.3 处理器模式
ARM处理器有多种工作模式，由CPSR的M[4:0]位决定：

| 模式 | M[4:0]位 | 描述 |
|------|---------|------|
| User (USR) | 10000 | 用户模式，普通程序执行模式 |
| FIQ (FIQ) | 10001 | 快速中断模式 |
| IRQ (IRQ) | 10010 | 中断模式 |
| Supervisor (SVC) | 10011 | 管理模式，操作系统使用 |
| Monitor (MON) | 10110 | 监控模式，安全扩展使用 |
| Abort (ABT) | 10111 | 中止模式，处理内存访问异常 |
| Hyp (HYP) | 11010 | 虚拟化模式 |
| Undefined (UND) | 11011 | 未定义模式，处理未定义指令 |
| System (SYS) | 11111 | 系统模式，特权级的用户模式 |

## 4. 如何编写ARM汇编程序？

### 4.1 基本结构
ARM汇编程序通常包含以下几个部分：
- 指令集声明（.syntax）
- 段声明（.data, .text）
- 全局符号声明（.global）
- 代码和数据定义

### 4.2 段和数据定义
| 伪指令 | 描述 | 示例 |
|--------|------|------|
| `.data` | 声明数据段 | `.data` |
| `.text` | 声明代码段 | `.text` |
| `.global` | 声明全局符号 | `.global _start` |
| `.word` | 定义32位数据 | `value: .word 0x12345678` |
| `.hword` | 定义16位数据 | `value: .hword 0x1234` |
| `.byte` | 定义8位数据 | `value: .byte 0x12` |
| `.asciz` | 定义字符串（以0结尾） | `msg: .asciz "Hello World!"` |
| `.space` | 分配空间 | `buffer: .space 256` |

### 4.3 示例程序
以下是一个简单的ARM汇编程序示例，在Linux环境下通过系统调用输出"Hello World!"：

```armasm
.syntax unified
.arch armv7a
.text
.global _start

_start:
    @ 写入系统调用
    mov r0, #1              @ 文件描述符 (stdout)
    ldr r1, =message        @ 消息地址
    mov r2, #13             @ 消息长度
    mov r7, #4              @ 系统调用号 (sys_write)
    swi #0                  @ 软件中断

    @ 退出系统调用
    mov r0, #0              @ 退出状态
    mov r7, #1              @ 系统调用号 (sys_exit)
    swi #0                  @ 软件中断

.data
message:
    .asciz "Hello World!\n"
```

### 4.4 编译和链接
使用GNU工具链编译ARM程序：

```bash
# 编译为目标文件
$ arm-linux-gnueabihf-as -o hello.o hello.s

# 链接生成可执行文件
$ arm-linux-gnueabihf-ld -o hello hello.o

# 或者使用GCC一步完成编译和链接
$ arm-linux-gnueabihf-gcc -nostdlib -o hello hello.s
```

### 4.5 在树莓派等ARM设备上运行
将生成的可执行文件传输到ARM设备上运行：

```bash
# 通过scp传输到设备
$ scp hello pi@raspberrypi:/home/pi/

# 在设备上执行
$ ./hello
```

## 5. 系统寄存器和控制寄存器

### 5.1 系统控制寄存器
ARM处理器包含多个系统控制寄存器，用于控制处理器的各种功能：

| 寄存器 | 全称 | 描述 |
|--------|------|------|
| SCTLR | System Control Register | 系统控制寄存器，控制处理器的基本功能 |
| ACTLR | Auxiliary Control Register | 辅助控制寄存器，控制处理器的辅助功能 |
| CPACR | Coprocessor Access Control Register | 协处理器访问控制寄存器 |
| DFSR | Data Fault Status Register | 数据错误状态寄存器 |
| DFAR | Data Fault Address Register | 数据错误地址寄存器 |
| IFSR | Instruction Fault Status Register | 指令错误状态寄存器 |
| IFAR | Instruction Fault Address Register | 指令错误地址寄存器 |

### 5.2 内存管理寄存器
| 寄存器 | 全称 | 描述 |
|--------|------|------|
| TTBR0 | Translation Table Base Register 0 | 转换表基址寄存器0 |
| TTBR1 | Translation Table Base Register 1 | 转换表基址寄存器1 |
| TTBCR | Translation Table Base Control Register | 转换表基址控制寄存器 |
| DACR | Domain Access Control Register | 域访问控制寄存器 |

## 6. ARM ABI调用约定

### 6.1 寄存器使用约定
ARM架构有明确的寄存器使用约定，以确保函数调用的正确性：

| 寄存器 | 角色 | 描述 |
|--------|------|------|
| R0-R3 | 参数/返回值 | 用于传递前4个参数，R0也用于返回值 |
| R4-R11 | 局部变量 | 被调用者保存（Callee-saved） |
| R12 | IP寄存器 | 内部过程调用寄存器，临时使用 |
| R13 | SP寄存器 | 栈指针 |
| R14 | LR寄存器 | 链接寄存器，保存返回地址 |
| R15 | PC寄存器 | 程序计数器 |

### 6.2 AAPCS (ARM Architecture Procedure Call Standard)
AAPCS是ARM架构的过程调用标准，规定了函数调用的细节：

1. **参数传递**：
   - 前4个参数通过R0-R3传递
   - 超过4个的参数通过栈传递
   - 64位参数需要对齐到偶数寄存器

2. **返回值**：
   - 32位以下的返回值通过R0传递
   - 64位返回值通过R0和R1传递
   - 更大返回值通过内存传递

3. **寄存器保存规则**：
   - **调用者保存**（Caller-saved）：R0-R3, R12, PC
   - **被调用者保存**（Callee-saved）：R4-R11, SP, LR

4. **栈对齐**：
   - 栈指针必须8字节对齐
   - 函数入口处需要维护栈帧

### 6.3 AAPCS64 (ARM 64-bit Architecture Procedure Call Standard)
在AArch64架构中，调用约定有所变化：

| 寄存器 | 角色 | 描述 |
|--------|------|------|
| X0-X7 | 参数/返回值 | 用于传递前8个参数，X0也用于返回值 |
| X8-X15 | 参数 | 用于传递额外参数 |
| X16-X17 | IP寄存器 | 内部过程调用寄存器 |
| X18 | 平台寄存器 | 特定平台使用 |
| X19-X29 | 局部变量 | 被调用者保存（Callee-saved） |
| SP | 栈指针 | 栈指针 |
| X30 | LR寄存器 | 链接寄存器 |
| X29 | FP寄存器 | 帧指针 |

### 6.4 示例函数调用
```armasm
.syntax unified
.arch armv7a
.text
.global main

@ int add(int a, int b)
add:
    push {lr}           @ 保存返回地址
    add r0, r0, r1      @ 计算 a + b
    pop {pc}            @ 返回

@ int main()
main:
    push {lr}           @ 保存返回地址
    mov r0, #5          @ 第一个参数
    mov r1, #3          @ 第二个参数
    bl add              @ 调用add函数
    mov r3, r0          @ 保存返回值
    mov r0, #0          @ 返回0
    pop {pc}            @ 返回
```

## 7. 内存管理单元（MMU）和页表

### 7.1 MMU概述
ARM处理器的内存管理单元（MMU）负责虚拟地址到物理地址的转换，提供内存保护和共享机制。

### 7.2 页表结构
ARM架构支持多种页表格式，最常见的是一级和二级页表：

#### 一级页表（段描述符）
| 位 | 名称 | 描述 |
|----|------|------|
| 31-20 | Section base address | 段基地址（1MB对齐） |
| 19-16 | AP | 访问权限位 |
| 15 | Domain | 域字段 |
| 14-10 | IMPLEMENTATION DEFINED | 实现定义 |
| 9 | SBZ | Should Be Zero |
| 8-5 | TEX | 类型扩展 |
| 4-3 | AP | 访问权限位（扩展） |
| 2 | C | Cacheable位 |
| 1 | B | Bufferable位 |
| 0 | 1 | 段描述符标识位 |

#### 二级页表（粗页表描述符）
| 位 | 名称 | 描述 |
|----|------|------|
| 31-10 | Coarse page table base address | 粗页表基地址 |
| 9-4 | IMPLEMENTATION DEFINED | 实现定义 |
| 3 | Domain | 域字段 |
| 2 | P | 权限检查忽略位 |
| 1 | 0 | 固定为0 |
| 0 | 1 | 粗页表描述符标识位 |

## 8. 中断和异常处理

### 8.1 异常类型
ARM处理器支持多种异常类型：

| 异常类型 | 模式 | 优先级 | 向量地址 |
|----------|------|--------|----------|
| Reset | Supervisor | 1 (最高) | 0x00000000 |
| Undefined Instruction | Undefined | 7 | 0x00000004 |
| Software Interrupt (SWI) | Supervisor | 6 | 0x00000008 |
| Prefetch Abort | Abort | 5 | 0x0000000C |
| Data Abort | Abort | 4 | 0x00000010 |
| IRQ | IRQ | 3 | 0x00000018 |
| FIQ | FIQ | 2 | 0x0000001C |

### 8.2 异常处理过程
当异常发生时，ARM处理器会执行以下操作：
1. 将CPSR复制到异常模式的SPSR
2. 设置CPSR中的模式位到对应的异常模式
3. 设置CPSR中的中断禁止位（根据异常类型）
4. 将返回地址保存到LR
5. 跳转到异常向量地址

### 8.3 异常返回
异常处理完成后，需要通过特定的返回指令返回到被中断的程序：
- `MOVS PC, LR`：用于Reset、Undefined Instruction、SWI和Prefetch Abort异常
- `SUBS PC, LR, #4`：用于Data Abort异常
- `SUBS PC, LR, #8`：用于IRQ和FIQ异常（在ARMv7及更早版本中）

## 9. GNU ARM汇编语法补充

### 9.1 指令后缀
ARM指令可以有多种后缀来改变其行为：

| 后缀 | 含义 | 示例 |
|------|------|------|
| S | 更新条件标志位 | ADDS R0, R1, R2 |
|条件码| 条件执行 | ADDEQ R0, R1, R2 |
| .W | 宽指令（Thumb-2） | ADD.W R0, R1, R2 |
| .N | 窄指令（Thumb-2） | ADD.N R0, R1, R2 |

### 9.2 常用伪指令
| 伪指令 | 描述 |
|--------|------|
| .align | 对齐数据 |
| .ascii | 定义ASCII字符串 |
| .asciz | 定义以NULL结尾的ASCII字符串 |
| .equ | 定义常量 |
| .macro/.endm | 定义宏 |
| .ifdef/.ifndef | 条件编译 |
