## 0. 引言
Github: 月と猫 - LunaNeko https://github.com/LunaticLegacy

CC-BY-NC-SA 4.0

本教程可能较为硬核，适合具有一定计算机基础和编程基础的人食用。如果在食用过程中昏厥或口吐白沫，本人概不负责。

本教程为自行总结，可能存在错误。如有错误问题请通过Repo反馈。

## 1. 寄存器（Registers）和使用方式

### 1.1 通用寄存器（Global Registers）
| 寄存器名 | 速记 | 描述 | 低32位格式 | 低16位格式 | 16位内低8 | 16位内高8 |
| ---------- | ------ | ------ | ----------- | ----------- | -------------- | -------------- |
| rax | Accumulator | 通用寄存器，用于存放返回值 | eax | ax | al | ah |
| rbx | Base | 基址寄存器，用于存放基址 | ebx | bx | bl | bh |
| rcx | Counter | 计数器，用于存放循环次数 | ecx | cx | cl | ch |
| rdx | Data | 数据寄存器，用于存放数据 | edx | dx | dl | dh |
| rsi | Source Index | 源索引寄存器| esi | si | sil | — |
| rdi | Destination Index | 目标索引寄存器 | edi | di | dil | — |
| rsp | Stack Pointer | 栈指针 | esp | sp | spl | — |
| rbp | Base Pointer | 栈基址寄存器 | ebp | bp | bpl | — |

寄存器在读写低32位时，高32位将自动设置为0。

以下寄存器为x86-64架构下扩展的通用寄存器（仅64位支持）：
| 寄存器名 | 描述 | 低32位格式 | 低16位格式 | 16位内低8 |
| ------- | ------- | ------- | ------- | ------- |
| r8 | 传参用 | r8d | r8w | r8b |
| r9 | 传参用 | r9d | r9w | r9b |
| r10 | 临时寄存器，caller-saved | r10d | r10w | r10b |
| r11 | 临时寄存器，caller-saved | r11d | r11w | r11b |
| r12 | 被调用者保存，callee-saved | r12d | r12w | r12b |
| r13 | 被调用者保存 | r13d | r13w | r13b |
| r14 | 被调用者保存 | r14d | r14w | r14b |
| r15 | 被调用者保存 | r15d | r15w | r15b |

这些新增的寄存器无法从16位内的高8位读取或写入。

调用约定见[4.5 ABI调用约定](#45-abi调用约定)。

### 1.2 标志寄存器（Flags Registers）

x86_64架构中的标志寄存器称为**RFLAGS**（但本人在GDB中查看为EFLAGS），是一个64位的寄存器，但其中有效和定义的标志位主要集中在低32位，尤其是低16位。

| 寄存器（标志）名 | 速记  | 位数 | 描述 |
|--------|--------|------|--------------------------------------|
| CF  | Carry Flag    | 0    | 进位标志：无符号运算中出现进位或借位时置1  |
| PF  | Parity Flag    | 2    | 奇偶标志：结果最低字节中1的个数为偶数时置1   |
| AF  | Auxiliary Carry Flag | 4 | 辅助进位标志：用于`二进制转十进制`（BCD）运算，表示第4位有进位。<br>（因计算效率低于纯二进制，BCD已极少使用。本教程不会给出该算法相关汇编指令） |
| ZF  | Zero Flag    | 6    | 零标志：结果为零时置1 |
| SF  | Sign Flag     | 7    | 符号标志：结果为负时置1  |
| IF  | Interrupt Enable Flag | 9 | 中断允许标志：为1时允许响应外部中断   |
| DF  | Direction Flag    | 10   | 方向标志：影响串操作指令的方向（递增/递减）  |
| OF  | Overflow Flag  | 11   | 溢出标志：有符号运算溢出时置1               |

注：
- 位数根据Intel官方文档，部分位号可能稍有区别，但标志名不变。
- 在64位时，IF会和`CR8`寄存器一起控制中断，两者对中断的屏蔽范围为AND关系。详见[CPU中断表](#41cpu中断表)和[CR8说明](#425-cr8)。

### 1.3 段寄存器（Segment Registers）
`如果不需要自己写OS，可跳过本节。`

在系统调用、任务切换、内存保护和权限判断时，这些寄存器的值至关重要。

| 寄存器 | 全称 | 描述 | 64位模式 | 32位模式 |
| ----- | ---- | --- | ---------- | ------ |
| **CS** | Code Segment   | 代码段 | - 基址强制为 0<br>- 用于权限检查（RPL/CPL）<br>- 64位代码必须设置 L = 1| - 存储当前代码段基址<br>- 控制指令执行位置  |
| **SS** | Stack Segment  | 栈段  | - 基址强制为 0<br>- 栈操作使用 RSP<br>- 用于特权级检查 | - 定义栈段基址<br>- 栈操作使用 \[SS\:SP]                         |
| **DS** | Data Segment | 数据段 | - 基址强制为 0<br>- 内存访问默认段<br>- 仅用于权限检查 | - 默认数据访问段<br>- `mov eax, [ebx]` 实际访问 \[DS\:EBX]       |
| **ES** | Extra Segment  | 附加段 | - 基址强制为 0<br>- 特定指令使用（`MOVS`, `STOS` 等）<br>- 兼容性保留  | - 字符串操作目标段<br>- `MOVS` 指令默认使用 \[DS\:ESI] 到 \[ES\:EDI] |
| **FS** | Far Segment | 扩展段 | - 可设置非零基址<br>- Linux：线程局部存储（TLS）<br>- Windows：线程环境块（TEB） | - 操作系统特定用途<br>- Win32：结构化异常处理 |
| **GS** | Global Segment | 全局段 | - 可设置非零基址<br>- Linux：Per‑CPU 数据区<br>- x64 系统调用：内核栈指针 | - 较少使用<br>- 某些 OS 用于处理器特定数据 |


注：
- 在早期的x86架构（16位、32位实模式和保护模式）中，内存访问是通过`段选择器 + 偏移`地址机制实现的。
  - 段寄存器保存的是“段选择子”（Segment Selector），它会指向[全局描述符表](#431-gdt)（GDT）或者[局部描述符表](#432-ldt)（LDT）中的段描述符，从而获得实际的段基地址、段限长和权限信息。
- 64位模式下，除FS和GS外，段寄存器的基址默认为0，不参与地址转换，主要是为了兼容和权限检查。
  - 且在64位模式下，CS寄存器的低2位代表当前CPU权限级别。

### 1.4 一些特殊的寄存器
`如果不需要自己写OS，可跳过本节。`

有些寄存器较为特殊，用于控制机器底层的运行情况。
| 寄存器名 | 速记 | 描述 | 备注 |
| -------- | ----- | ---- | ----- |
| GDTR | Global Descriptor Table Register | 保存全局描述符表所在位置，使用`lgdt [dir]`以写入。 | 见[GDT说明](#431-gdt)。 |
| LDTR | Local Descriptor Table Register | 保存局部描述符表所在位置，使用`lldt [dir]`以写入。 | 见[LDT说明](#432-ldt)。|
| CR0 | Control Register 0 | 控制寄存器0，控制CPU自身的全局特性。 | 见[CR0说明](#421-cr0)。 |
| CR2 | Control Register 2 | 控制寄存器2，用于存储最近发生的页面错误的虚拟地址。 | 见[CR2说明](#422-cr2)。 |
| CR3 | Control Register 3 | 控制寄存器3，存储当前页目录表的物理地址（用于分页），在多级分页中，CR3指向页表目录。 | 寄存器功能见[CR3说明](#423-cr3-页目录基址寄存器)，页表功能见[页表说明](#433-页表)。 |
| CR4 | Control Register 4 | 控制寄存器4，用于扩展的CPU功能和控制，包括支持SSE、支持64位模式、启用监视器等功能。 | 见[CR4说明](#424-cr4)。 |
| CR8 | Control Register 8 | 控制中断优先级（TPI）。 | 仅在（64位）长模式中可用。见[CR8说明](#425-cr8) |
| EFER | | 扩展功能使能寄存器，用于控制x86_64架构中与长模式（Long Mode）相关的特性。 | 属于模型特定寄存器（MSR），见[#4.2.6](#426-efer寄存器)。 |
| RIP | ~~Requiescat in Pace~~ Insturction Pointer | 指向下一条即将执行的指令的64位地址。 | 只读，只能用`jmp`控制。|

### 1.5 模型特定寄存器（Model Specific Registers）

这是一种只能使用`rdmsr`和`wrmsr`指令访问的寄存器。在访问这些寄存器前，需要先对`ecx`寄存器写入该寄存器地址以设置目标寄存器。随后：
```
rdmsr  ; read MSR - 读取MSR，高32位储存于EDX，低32位于EAX。
wrmsr  ; write MSR - 读取64位MSR内容，规则同上。
```

| 寄存器名 | 地址 |
| -------- | ---- |
| EFER | 0xC0000080 |
| STAR | 0xC0000081 |

## 2. 指令集
在CPU里，有一条核心原则：**一切对象，皆有内存**。 

### 2.1 数据定义指令
使用以下指令在内存中分配空间并初始化数据。

推荐在.data段中**定义（已初始化的）数据**，在.bss段分配**未初始化的数据**（预留字节）。

| 指令 | 速记 | 描述 | 示例 |
| --- | --- | --- | --- |
| db | define byte | 定义一个字节（8位） | `char db 0x21` |
| dw | define word | 定义一个字（16位） | `short dw 0x1234` |
| dd | define dword | 定义一个双字（32位） | `int dd 0x12345678` |
| dq | define qword | 定义一个四字（64位） | `long dq 0x1234567890ABCDEF` |

---

注1：
- 一条数据定义指令只能定义一种数据类型，但可以定义**相同数据类型下的**多个数据。例如：
```nasm
chars db 0x21, 0x22, 0x23, 0x24
```
- 这样会在内存中开辟多个空间，用于储存4个byte的空间。此时可使用地址偏移访问到不同的内容。

注2：
- `db`指令可用于定义字符串。例如：
```nasm
message db "Hello, World!", 0
```
- 因为ASCII格式的字符串在内存中实际上是一连串连续的byte。（如果是其他格式就另当别论了）
- 定义字符串时，最后跟上的0意为“字符串末尾”。

---

### 2.2 基础操作指令（权限：RING 3）
以下指令，除`syscall`和特殊说明指令外，均以32位为例。

#### 2.2.1 内存调度相关指令
| 指令 | 速记 | 描述 | 复杂指令？ | 示例 | 示例释义 | 备注 | 等价指令 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| `[]` | address | 取地址 | N/A | `mov eax, [msg]` | 从`msg`指向的地址取值 | 不是指令，但其重要性和指令不相上下。见本节注1。 |
| mov | move | 移动数据（实际是写入） | ❌ | `mov eax, 0x1234` | 将立即数`0x1234`赋值给寄存器eax | 两个操作数位宽必须相等。<br>支持寄存器与内存、寄存器与寄存器、寄存器与立即数等多种组合。 |
| lea | load effective address | 取地址而非内容 | ❌ | `lea esi, [msg + 2]` | 将`msg`地址+2后赋值给esi，不读取 |
| push | stack push | 将数据压入栈顶 | ✅ | `push ebx` | 把ebx的值压入栈中 | 64位情况下为压入8个byte，实际本指令按位宽进行。 | `sub esp, 4`<br>`mov [esp], ebx` | 
| pop | stack pop | 从栈顶取出数据 | ✅ | `pop eax` | 从栈顶弹出4个byte，并将它们交给eax | 64位情况下为弹出8个byte，实际本指令按位宽进行。| `mov eax, [esp]`<br>`add esp, 4` |

---
- `[]` 表示**内存寻址操作（解引用）**，用于访问内存中的值。
- `mov eax, ebx`：将寄存器 `ebx` 的值赋给 `eax`。
- `mov eax, [ebx]`：将寄存器 `ebx` 的值**作为地址**，找到对应地址的值，并赋给 `eax`。
- `mov eax, [msg + 2]`：取 `msg` 地址偏移 2 字节处的值。
- `[]` 中也可以使用寄存器：
  - `mov eax, [esi]`：将 `esi` 所指向地址的值读入 `eax`
  - `mov eax, [ebx + 4]`：对寄存器进行偏移
- 这是汇编中最核心的**间接寻址**机制，支持多种寻址格式：  
  `base + index * scale + offset` 形式，如 `mov eax, [ebx + esi*4 + 8]`
- 在`[]`中，如果不加任何修饰词，则默认读取字节数为目标寄存器大小。
- 可用修饰词包括：`qword dword word byte`，分别对应向后取`64 32 16 8`个bit（自己换算byte数去）。
---

#### 2.2.2 算术指令
| 指令 | 速记 | 描述 | 复杂指令？ | 示例 | 示例释义 | 32位处理 | 64位处理 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| add | add | 相加操作 | ❌ | `add eax, ebx` | 将`ebx`的值加到`eax`上。 | `eax = eax + ebx` | `rax = rax + rbx` |
| sub | subtract | 相减操作 | ❌ | `sub eax, 4` | 从`eax`的值中减`4`。 | `eax = eax - 4` | `rax = rax - 4` |
| inc | increment | 自增操作 | ❌ | `inc eax` | 将`eax`的值`+1`。 | `eax = eax + 1` | `rax = rax + 1` |
| dec | decrement | 自减操作 | ❌ | `dec eax` | 将`eax`的值`-1`。 | `eax = eax - 1` | `rax = rax - 1` |
| mul | multiply |（无符号）乘法操作 | ❌ | `mul ebx` | 将`eax`和`ebx`的值相乘。<br>**使用前需提前压入`eax`**。| `edx:eax = eax * ebx`（结果为64位） | `rdx:rax = rax * rbx`（结果为128位） |
| imul | integer multiply | （有符号）乘法操作 | ❌ | `imul ebx` | 同上，但考虑符号。| 同上，但考虑符号。| 同上，但考虑符号。|
| div | divide | （无符号）除法操作 | ❌ | `div ebx` | 见右侧。 | `(edx:eax) / ebx = eax ...... edx`<br>（即商在`eax`，余数在`edx`） | `(rdx:rax) / rbx = rax ...... rdx` |
| idiv | integer division | （有符号）取模操作 | ❌ | `idiv ebx` | 同上，但考虑符号位；余数可为负。 | 同上，此时`eax`有符号。| 同上，此时`rax`有符号。|

- `a:b`意为`低位储存在寄存器b，高位储存在寄存器a`。后文同。

#### 2.2.3 逻辑门运算指令
| 指令 | 速记 | 描述 | 复杂指令？ | 示例 | 示例释义 | 备注 | 等价指令 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| and | and | 按位与 | ✅ | `and eax, 0x1` | 将`eax`和`0x1`做按位与运算。 |
| or  | or | 按位或 | ✅ | `or eax, ebx` | 将`eax`和`ebx`做按位或运算。 |
| not | not | 按位非（反转） | ✅ | `not eax` | 将`eax`每位取反。 |
| xor | exclusive or | 异或操作 | ✅ | `xor eax, ebx` | 对`eax`和`ebx`按位异或。 | 可用于将寄存器清零，例如`xor eax, eax`。|

- 其他逻辑门，例如`nand nor xnor xand`都可由这些东西直接实现。

#### 2.2.4 调度控制用指令
| 指令 | 速记 | 描述 | 复杂指令？ | 示例 | 示例释义 | 备注 | 等价指令 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| cmp | compare | 比较操作（不保存结果，仅设置标志） | ❌ | `cmp eax, ebx` | 比较`eax`和`ebx`的值，更新标志位 |
| test | test | 按位与操作（不保存结果，仅设置标志） | ❌ | `test eax, ebx` | 比较`eax`和`ebx`的值，更新标志位 |
| jmp | jump | 无条件跳转 | ✅ | `jmp label` | 跳转到标签位置 | 相当于直接修改 `eip` 指向目标地址（汇编器自动计算偏移） |
| jz  | jump if zero | 若零标志位为1，跳转 | ✅ | `jz label` | 如果上一个指令结果为零，则跳转 | 相当于：`if (ZF == 1) jmp label`，需依赖如 `cmp`、`test` 设置标志位 |
| je  | jump if equal | 等于时跳转 | ✅ | `je label` | 如果前一次的比较结果为相等，则跳转 | `je` 是 `jz` 的别名，本质一样：`if (ZF == 1) jmp label` |
| jl  | jump if less | 小于时跳转 | ✅ | `jl label` | 如果前一次比较结果是小于，则跳转 | 相当于：`if (SF ≠ OF) jmp label`，依赖有符号比较后的标志位 |
| jg  | jump if greater than | 若大于则跳转（有符号） | ✅ | `jg label` | 如果比较后大于，则跳转 | `ZF == 0 && SF == OF`，有符号比较 |
| jle | jump if less or equal | <= |
| jge | jump if greater than or equal | >= |
| jne | jump if not equal | != |
| jb  | jump if below | 若CF标志位为1，跳转 | ✅ | `jb label` | | 相当于：`if (CF == 1) jmp label`，需设置标志位 |
| call | call | 函数调用 | ✅ | `call func` | 调用名为`func`的函数 | 相当于：`push eip` + `jmp func`（将返回地址压入eip后跳转） |
| ret  | return | 从函数返回 | ✅ | `ret` | 返回到调用函数的地方 | 相当于：`pop eip`（从栈中弹出返回地址，赋值给指令指针） |
| int | interrupt | 中断 | ✅ | `int 0x80` | 调用中断0x80，即触发Linux系统调用 | 会自动跳转到[IDT](#411-如何写中断向量表idt)中对应的中断处理程序。<br>在没设置好对应编号的IDT时`int`了这个编号会直接Triple Fault。|
| syscall | system call | 系统调用 | ✅ | `syscall` | 调用系统调用，按照系统调用时的寄存器规则 | 用于64位模式的快速系统调用，受MSR（模型特定寄存器）配置影响；进入内核时会自动切换`cs`、`ss`、`rsp`，调度时自动进入内核态。 |
| stos | store output | 存储输出 | ✅ | `stos` | 将AL寄存器的值写入当前地址 | 不常用。 |
| movs | move output | 移动输出 | ✅ | `movs` | 将当前地址的值写入AL | 不常用。 |
| lodsb | load output | 读取输入 | ✅ | `lodsb` | 将当前地址的值赋给AL | 不常用。 |
| rdmsr | read model specific register | 读取模型特定寄存器 | ✅ | `rdmsr` | 读取MSR（模型特定寄存器） | 需要先在`ecx`写入MSR编号，再执行此指令，否则无效。 | 
| wrmsr | write model specific register | 写入模型特定寄存器 | ✅ | `wrmsr` | 写入MSR（模型特定寄存器） | 同上。 |


### 2.3 内核指令（RING 0）
以下指令为**内核态指令**，用户态执行将触发异常（13号中断）。

这些指令用于控制硬件、中断、特权寄存器等系统资源。

| 指令 | 速记 | 描述 | 复杂指令？ | 示例 | 示例释义 | 备注 |
|------|------|------|-------------|--------|-------------|------|
| cli | clear interrupt | 禁用中断 | ❌ | `cli` | 清除符号寄存器的IF标志位，关闭外部中断响应 | 执行后，CPU会屏蔽除不可屏蔽中断外其他所有中断。 |
| sti | set interrupt | 启用中断 | ❌ | `sti` | 执行后，CPU会退出`cli`状态。 |
| hlt | halt | 停机（等待中断） | ❌ | `hlt` | 让 CPU 进入低功耗等待中断 | 常用于空闲循环，需配合 `sti`。且当RESET线上有复位信号、CPU响应不可屏蔽中断、CPU响应可屏蔽中断3种情况之一时，CPU脱离暂停状态 |
| lgdt | load GDT | 加载全局描述符表 | ✅ | `lgdt [gdt_ptr]` | 加载段描述表地址 | `DEPRECATED`，详见[#4.3.1](#431-gdt)。 |
| lidt | load IDT | 加载中断描述符表 | ✅ | `lidt [idt_ptr]` | 设置中断向量表地址 | 中断格式见[#4.1](#41cpu中断表)。 |
| ltr | load task register | 加载任务寄存器 | ✅ | `ltr ax` | 设置 TSS（任务状态段）指针 |
| mov crX, reg | 控制寄存器写入CR系列（特殊） | ✅ | `mov cr3, eax` | 设置页目录表基地址（开启分页），必须提前将页表mov到eax里 | CR系列寄存器不得在用户态被mov，且不得输入立即数。 |
| in | input port | 从I/O端口读数据 | ✅ | `in al, 0x60` | 从键盘控制器端口读取字节（假设键盘为`0x60`，下同） |
| out | output port | 向I/O端口写数据 | ✅ | `out 0x60, al` | 向键盘控制器发送命令 |
| iret | interrupt return | 中断状态恢复 | ✅ | `iret` | 从中断状态回到内核态 | 回到用户态并继续执行用户态指令。 |

- 附：现代操作系统一般每秒会调度1000次中断，所以不要担心中断有什么坏处。

## 3. 如何写一条汇编程序？
- 汇编程序（个人建议）以`.asm`为后缀，编译成可执行文件（例如`.exe`）后运行。
- 汇编程序以**段**为单位进行编写，每个段都有自己的起始地址和结束地址。
- 在代码段内，以**指令**为单位进行编写，每个指令都有自己的操作码和操作数。
- 注释的开头是`;`。

示例程序：在Linux环境下，通过系统调用输出"Hello world!"。

环境：Linux x86_64

```nasm
bits 64 ; 告诉nasm，这是一个64位程序。

section .data
    msg db "Hello world!", 0    ; 定义一个字符串，其参数为msg。0为结尾标志。

section .text
global _start
    _start:
        ; 进入时栈帧保护。栈帧保护目的为本函数执行期间对栈的修改不会影响到其他函数。
        push rbp        ; 将rbp的值压入栈顶，此时栈顶的值为rbp的值。
        mov rbp, rsp    ; 然后，将当前栈顶（rbp）地址作为栈底地址，进入本函数的栈帧。
        sub rsp, 8      ; 让rbp指针向下走8个字节，作为第一个空间。（由于系统是64位）

        ; 开始打印。
        ; 在Linux系统中，为使用syscall调度输出，各个寄存器状态如下：
        ; rax：系统调用号，设置为1时表示执行write。
        ; rdi：第一个参数，字符串起始地址。
        ; rsi：第二个参数，字符串长度。
        mov rax, 1      ; 将系统调用号1赋值给rax。
        mov rdi, msg    ; 将msg的地址赋值给rdi。
        mov rsi, 13     ; 将字符串长度赋值给rsi。长度为13，包括最后那个0。
        int 80h          ; 调用系统调用，让Linux执行该操作。

        ; 退出前栈帧保护。
        xor rax, rax    ; 将rax清零，用于返回值。(return 0)
        add rsp, 8      ; 恢复栈帧到开发之前。
        pop rbp         ; 将先前pop进的rbp值压回rbp。
        ret             ; 返回。
```

然后，执行如下内容（生成目标文件）：

```bash
$ nasm -f elf64 hello.asm -o hello.obj
```

链接目标文件以得到可执行程序：
```bash
$ gcc -no-pie -nostartfiles hello.obj -o hello
```

这样，你的第一条程序就写好了！庆祝！

## 4. 寄存器备注表
`从此开始，你将进入深渊。`

### 4.1：CPU中断表

| 中断号 | 优先级 | 异常名称                     | 描述                                            |
|--------|--------|------------------------------|------------------------------------------------|
| 0      | `18`   | Divide Error                  | 除零错误，如除以0                              |
| 1      | `17`   | Debug Exception               | 调试异常，单步执行或断点命中                    |
| 2      | `∞`    | Non-maskable Interrupt        | 不可屏蔽中断（NMI），通常用于紧急硬件错误       |
| 3      | `16`   | Breakpoint                    | 断点异常，由 `int 3` 指令触发                  |
| 4      | 15     | Overflow                      | 溢出异常，由 `int 4` 指令触发                  |
| 5      | 14     | Bound Range Exceeded          | 越界检查失败                                  |
| 6      | 13     | Invalid Opcode                | 非法指令                                      |
| 7      | 12     | Device Not Available          | 浮点/协处理器不可用                            |
| 8      | `20`   | Double Fault                  | 双重故障，处理第一个中断时出错。                |
| 9      | 11     | Coprocessor Segment Overrun   | 协处理器段溢出（保留）                          |
| 10     | 10     | Invalid TSS                   | 无效的任务状态段                               |
| 11     | 9      | Segment Not Present           | 段不存在                                       |
| 12     | 8      | Stack-Segment Fault           | 栈段错误                                       |
| 13     | 7      | General Protection Fault      | 常规保护异常，访问非法内存或越权                |
| 14     | `19`   | Page Fault                    | 页错误，内存页访问不到内存                      |
| 15     | —      | Reserved                      | 保留，未定义                                   |
| 16     | 6      | x87 FPU Error                 | 浮点处理器错误                                 |
| 17     | 5      | Alignment Check               | 数据对齐检查失败（仅在启用时）                   |
| 18     | `21`   | Machine Check                 | 硬件错误                                       |
| 19     | 4      | SIMD Floating-Point Exception | SIMD 浮点异常                                  |
| 20     | 3      | Virtualization Exception      | 虚拟化异常（Intel VT/AMD-V）                   |
| 21–31  | —      | Reserved                      | 保留，`为未来标准保留`。                        |
| 32+    | 1      | User-defined Interrupts       | 用户自定义中断，通常用于 IRQ（中断请求）         |

- 中断优先级数值从1开始，越高越优先。
  - 优先级和屏蔽相关，屏蔽等级受`CR8`寄存器影响。可被屏蔽的中断优先级范围是`0~15`。
- 三重错误（Triple Fault）没有中断号，并将直接导致CPU复位。
- 在32位Linux中，`int 80`一般是系统调用号。

### 4.1.1 如何写中断向量表（IDT）？
一个中断向量表的对齐格式是：

```C
#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t offset_low;   // 中断处理程序低16位地址
    uint16_t selector;     // 代码段选择子（通常是内核代码段）
    uint8_t  reserved;     // 保留，置0
    union {
      uint8_t internal;
      struct {
        uint8_t  type : 4;     // 类型，通常 0xE = 32位中断门
        uint8_t  always0 : 1;  // 固定为0
        uint8_t  dpl : 2;      // 描述符特权级，0最高权限
        uint8_t  present : 1;  // 有效位，1表示该中断门存在
      }
    }
    uint16_t offset_high;  // 中断处理程序高16位地址
} IDTEntry;
#pragma pack(pop)

#define IDT_SIZE 256
IDTEntry idt[IDT_SIZE];

// 设置IDT中断门函数，offset是中断处理函数地址
void set_idt_entry(int n, uint32_t offset, uint16_t selector, uint8_t dpl, uint8_t type) {
    idt[n].offset_low = offset & 0xFFFF;
    idt[n].selector = selector;
    idt[n].reserved = 0;
    idt[n].type = type & 0xF;        // 通常0xE=32位中断门
    idt[n].always0 = 0;
    idt[n].dpl = dpl & 0x3;
    idt[n].present = 1;
    idt[n].offset_high = (offset >> 16) & 0xFFFF;
}

// IDTR结构体，用于 lidt 指令
#pragma pack(push, 1)
typedef struct {
    uint16_t limit;
    uint32_t base;
} IDTR;
#pragma pack(pop)
IDTR idtr;

// 加载IDT函数，调用 lidt 指令
static inline void lidt(void* base, uint16_t size) {
    IDTR idtr;
    idtr.limit = size - 1;
    idtr.base = (uint32_t)base;
    __asm__ __volatile__("lidt %0" : : "m"(idtr));
}

// 例：初始化IDT
void idt_init() {
    // 清空idt
    for (int i = 0; i < IDT_SIZE; i++) {
        idt[i].present = 0;
    }

    // 0号中断（除零错误）处理函数假设为 isr0 地址
    extern void isr0();
    set_idt_entry(0, (uint32_t)isr0, 0x08, 0, 0xE);

    // 其他中断类似设置...

    // 加载IDT寄存器
    lidt(idt, sizeof(idt));
}
```


### 4.2 CR系列寄存器各位的值
#### 4.2.1 CR0
| 位  | 名称        | 意义 | 说明 |
|-----|-------------|----------------|---------------------------------|
| 0   | PE          | Protection Enable（进入保护模式） |
| 1   | MP          | Monitor Coprocessor   |
| 2   | EM          | Emulation（数学协处理器仿真） | 0：表示有协处理器，1：没有协处理器。一般被BIOS设置。|
| 3   | TS          | Task Switched（任务切换）|
| 4   | ET          | Extension Type （扩展类型） | 1：使用intel 80387浮点指令，0：使用intel 287浮点指令。本位与EM协调使用。 |
| 5   | NE          | Numeric Error（内部x87浮点错误报告机制）|
| 16  | WP          | Write Protect（写保护） |
| 18  | AM          | Alignment Mask（对齐检查启用） |
| 29  | NW          | Not Write-through（禁止写回缓存） |
| 30  | CD          | Cache Disable（禁用缓存） |
| 31  | PG          | Paging（分页使能） |

#### 4.2.2 CR2
CR2用于储存页错误发生时的地址，其内部bit没有特殊意义。

#### 4.2.3 CR3 (页目录基址寄存器)

| 位     | 说明                                  |
|--------|-------------------------------------|
| 51..12 | 页目录基址（物理地址高位）           |
| 11..0  | 保留或其他控制位                    |

#### 4.2.4 CR4

| 位  | 名称    | 说明（英文原文）          | 说明（中译）                       |
|-----|------------|------------------------|--------------------------------|
| 0   | VME       | Virtual-8086 Mode Extensions         | 虚拟8086模式扩展 |
| 1   | PVI       | Protected-mode Virtual Interrupts    | 保护模式虚拟中断 |
| 2   | TSD       | Time Stamp Disable                   | 禁用时间戳指令 |
| 3   | DE        | Debugging Extensions              | 调试扩展 |
| 4   | PSE       | Page Size Extensions               | 页大小扩展 |
| 5   | PAE       | Physical Address Extension          | 物理地址扩展 |
| 6   | MCE       | Machine Check Enable                 | 机器检查启用 |
| 7   | PGE       | Page Global Enable                | 页全局启用 |
| 8   | PCE       | Performance-Monitoring Counter Enable      | 性能监控计数器启用 |
| 9   | OSFXSR    | OS Support for FXSAVE and FXRSTOR instructions  | 操作系统支持 FXSAVE 和 FXRSTOR 指令  |
| 10  | OSXMMEXCPT  | OS Support for Unmasked SIMD Floating-Point Exceptions | 操作系统支持未屏蔽的 SIMD 浮点异常 |
| 11  | UMIP      | User-Mode Instruction Prevention     | 用户模式指令防护 |
| 12  | Reserved  | Reserved                            | 保留位 |
| 13  | LA57      | 5-level Paging Enable         | 启用五级分页 |
| 14  | VMXE      | VMX Enable (Intel Virtualization)      | 启用虚拟机扩展（Intel 虚拟化技术）  |
| 15  | SMXE      | SMX Enable                      | 启用安全模式扩展 |

#### 4.2.5 CR8

| 位     | 名称                          | 说明                    |
| ----- | --------------------------- | ---------------------------- |
| 3..0  | TPR（Task Priority Register） | 当前中断优先级（0–15），值=k时屏蔽优先级≤k的IRQ。 |
| 63..4 | 保留                          | 保留位，写入时必须为0      |

- 值为0：允许所有中断；
- 值为15：屏蔽所有中断（除了NMI和SMI等不可屏蔽中断）；

通过`mov`指令设置CR8，例如：
```
mov cr8, rax    ; 设置中断优先级
mov rax, cr8    ; 读取当前优先级
```

- 在64位时，RFLAGS寄存器的IF位会与该寄存器以`AND`关系一起屏蔽外部中断。

#### 4.2.6 EFER寄存器
该寄存器位宽为64位，无法直接读写，只能设置`ecx`寄存器的值为`0xC0000080`后使用`rdmsr`指令读取或使用`wrmsr`指令写入。

| 位     | 名称（英文） | 名称（中文） | 读写权限 | 描述    | 参考  |
| ----- | ------ | ----------- | ---- | ---------------------------------------- | ----------------- |
| 0     | SysCall Enable (SCE) | SYSCALL 启用  | R/W  | 启用 `SYSCALL` 和 `SYSRET` 指令（仅在 64 位模式下有效） |   |
| 8     | Long Mode Enable (LME) | IA-32e 模式启用 | R/W  | 启用 IA-32e 模式（64位模式即长模式必须在这里启用） | |
| 10    | Long Mode Activated? (LMA) | IA-32e 模式活动 | R  | 指示 IA-32e 模式是否处于活动状态    |  |
| 11    | No-eXecution Enable (NXE) | 执行禁用启用   | R/W  | 启用或禁用执行禁用位（NX） |   |
| 12–63 | Reserved | 保留   | — | 保留位，写入时必须为0 | ([OSDev Wiki][1]) |

[1]: https://wiki.osdev.org/CPU_Registers_x86-64?utm_source=chatgpt.com "CPU Registers x86-64"

### 4.3 内存管理相关

#### 4.3.1 GDT
`ALMOST DEPRECATED - 现代64位操作系统已采用页表作为内存管理单位。GDT目前仅用于从保护模式转入长模式。`

格式：

| 位范围 | 字段 | 备注 |
| ----- | ------------------------ | ------------------------------------- |
| 0–15  | Limit [15:0]             | 段界限低 16 位                             |
| 16–31 | Base [15:0]              | 段基址低 16 位                             |
| 32–39 | Base [23:16]             | 段基址中间 8 位                             |
| 40–43 | Type [0:3]               | 段类型（4 位，定义可执行/可写/访问等属性）               |
| 44    | Type [4] or S            | Descriptor Type：0=系统段，1=代码/数据段        |
| 45–46 | DPL                      | 描述符特权级别（0~~3，对应 Ring 0~~3）            |
| 47    | P                        | Present：1=段存在，0=段不存在                  |
| 48–51 | Limit [19:16]            | 段界限高 4 位                              |
| 52    | AVL                      | Available：操作系统可自行使用                   |
| 53    | L                        | Long Mode：1=64 位代码段，0=非 64 位段         |
| 54    | D/B (Default Size / Big) | 0=16 位段，1=32 位或 64 位段（在非 long mode 下） |
| 55    | G                        | Granularity：0=按字节粒度，1=按 4 KB 粒度       |
| 56–63 | Base [31:24]             | 段基址高 8 位                              |

其中，段类型`Type`的各位含义如下：
| 位 | 名称 | 含义 (代码段的场合) | 含义 (数据段的场合) |
| - | ---- | ---------------- | ---------- |
| 3 | E (Executable) | 1 = 代码段  | 0 = 数据段 |
| 2 | C (Conforming) / E (Expand‑down) | 1 = 其他权限可执行（Conforming）；可在更高（数字）或相同特权级运行<br>0 = 非顺序（Non‑conforming）；仅能在相同权限下运行 | 1 = 向下扩展（Expand‑down，*地址映射方式从高地址开始向基地址*）<br>0 = 向上扩展（Expand‑up，*地址映射方式从基地址开始向高地址*） |
| 1 | R (Readable) / W (Writable) | 1 = 可读（Readable，*可作为数据读取*）<br>0 = 仅执行（Execute‑only，*不得作为数据读取*） | 1 = 可写（Writable）<br>0 = 只读（Read‑only）  |
| 0 | A (Accessed)   | CPU访问后写1，用于操作系统或硬件跟踪是否被使用 | 同左 |

一个GDT的内存管辖范围为：`[ Base,  Base + (Limit + 1) × (G ? 4096 : 1) ]`

其中，GDT可以这么写：
```
section .text32  ; 我实际的连接脚本将这里链接在.text段，不要在意名字。
bits 32

gdt_start:
    dq 0                   ; empty sign
    dq 0x00CF9A000000FFFF  ; code as: base=0 limit=4G r-x
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; limit
    dd gdt_start                ; base

```
并通过`lgdt [gdt_descriptor]`载入。

其中，`0x00CF9A000000FFFF`的意义为：

- `0b 0000 0000 1100 1111 1001 1010 0000 0000 0000 0000 0000 0000 1111 1111 1111 1111`
  - Segment Limit: `1111 1111 1111 1111 1111` -> `0xFFFFF` : 1048575 segments
  - Base Address: `0000 0000 0000 0000 0000 0000 0000 0000` -> `0x000000`: Base is 0x0
  - Type: `1010` -> meant `Code segment, not conforting, Readable and unused`
  - Descriptor Privilege Level: `00` -> Ring 0
  - Present: `1` -> This seg is present.
  - Long Mode: `0` -> Still in 32-bit mode.
  - Available: `1` -> Use it for OS reading.
  - Default Size: `1` -> This is a 32-bit segment.
  - Granualarity: `1` -> meant `Unit as 4kB`.

因此该GDT管理的内存范围为：`0x0 to 1048575 * 4kB(4096) = 4GB`。

在现代的64位系统启动模式中，GDT仅用于从保护模式转入长模式。因此只需要写一个能用的就可以了。

#### 4.3.2 LDT
`略，反正以后也用不到了。`

```
“Long mode ignores most segmentation, including LDT.”
  - Intel® 64 and IA-32 Architectures SDM Vol.3
```

#### 4.3.3 页表
页表的单位通常是以**4kB**为页单位，且每一张页表中的每一个条目（页）都用uint64表示。

按照Intel厂商的规定，在每一个uint64中，各位代表如下：

| 位号  | 名称                           | 说明                                        |
| ----- | ------------------------------ | ------------------------------------------ |
| 0     | Present (P)                    | 页是否存在于物理内存中。若为0，则忽略其余位。 |
| 1     | Read/Write (R/W)               | 1表示可写；0表示只读。|
| 2     | User/Supervisor (U/S)          | 1表示用户态可访问；0表示仅内核态可访问。 |
| 3     | Page-level Write Through (PWT) | 控制缓存写策略。 |
| 4     | Page-level Cache Disable (PCD) | 禁止对该页缓存。 |
| 5     | Accessed (A)                   | 由CPU设置，表示该页是否被访问过。|
| 6     | Dirty (D)                      | 仅在1级页表有效；表示页面是否被写入过。|
| 7     | Page Size (PS)                 | 仅在2、3级页表有效，1=大页（2MB/1GB），建议写1。|
| 8     | Global (G)                     | 如果设置，该页对所有进程通用（不被CR3刷新影响）。 |
| 9-11  | Ignored (可用)                 | 可由操作系统自定义使用。 |
| 12–51 | Physical Address               | 指向下一页表（或页）的物理地址的高40位。必须是页对齐（低12位为0）|
| 52–58 | Ignored (可用)                 | 可由OS自由使用，部分系统用于标记用途，如页属性。  |
| 59    | Protection Key (PK)            | 与内存保护键相关（仅在支持 PKU 的系统上启用）。 |
| 60-62 | Reserved                       | 保留，必须为0（依具体实现可能变化）。  |
| 63    | No Execute (NX)                | 仅在1~3级页表有效。如果支持该功能，1表示该页不可执行。需要在EFER.NXE=1时生效。详见[EFER寄存器说明](#426-efer寄存器)。 |

在x86_64系统内，页表可以为LA48或LA57（取决于CPU是否使用扩展，开启LA57需设置**CR4.LA57（第13位）**=1。）。其中各级页表分别分层为：
- 5级（启动LA57时可用），每页映射256TB。
- 4级，每页映射512GB。
- 3级，每页映射1GB，必须设置PS=1。
- 2级，每页映射2MB，必须设置PS=1。
- 1级，每页映射4kB。

注意：除顶级页表外，其他次级页表在内存中数量**并不唯一**。以及**页存在 != 页完整**。

在该页表规则下，虚拟内存的地址情况是：
| 位号 | 意义 |
| ---- | --------- |
| 0-11


### 4.5 ABI调用约定

| 项目 | System V AMD64 ABI (Linux/macOS等) | Microsoft x64 ABI (Windows) |
| --- | ------------- | ------------------------ |
| **参数传递寄存器**   | 第1\~6个整数/指针参数依次用：<br>RDI, RSI, RDX, RCX, R8, R9 | 第1\~4个整数/指针参数依次用：<br>RCX, RDX, R8, R9 |
| **浮点参数传递寄存器** | XMM0 \~ XMM7 | XMM0 \~ XMM3 |
| **返回值寄存器**    | 整数用RAX，浮点用XMM0 | 整数用RAX，浮点用XMM0 |
| **栈对齐要求** | 调用点前，栈指针需对齐到16字节   | 调用点前，栈指针需对齐到16字节  |
| **调用者保存寄存器（caller-saved）**  | RAX, RCX, RDX, RSI, RDI, R8-R11 | RAX, RCX, RDX, R8, R9, R10, R11 |
| **被调用者保存寄存器（callee-saved）** | RBX, RBP, R12-R15, RSP  | RBX, RBP, RDI, RSI, R12-R15, RSP |
| **栈参数传递** | 超过6个整数参数通过栈传递  | 超过4个整数参数通过栈传递 |
| **栈帧**  | 可选，一般用RBP作为帧指针  | 一般禁用RBP作为帧指针（除非用`/Oy-`） |
| **特殊区域** | “红区”（Red Zone） — 栈顶下方128字节，调用者可安全使用，无需栈调整 | 无红区，调用前必须保证栈空间 |

