## 0. 引言
Github: 月と猫 - LunaNeko https://github.com/LunaticLegacy

CC-BY-NC-SA 4.0

本文为基于C的RVV SIMD指令教程。

- 我为什么要学习这个？
  - 一句话：如果你不是RISC-V开发者，且不是来**凹性能**的，可以不看这个。
  - SIMD（Single Instruction Multiple Data，单指令多数据）指令比起普通指令，在计算期间可省去中途的取指令开销。因此计算效率会更快。

### 0.1 准备工作

笔者使用的环境为WSL2 Ubuntu，编译器：`riscv64-linux-gnu-gcc`，宿主机CPU型号为`Intel Core Ultra 9 275HX`.

在确保自己的编译器支持扩展“`V`ector”时，启用：

```C
#include <riscv_vector.h>
```

以使用RVV SIMD函数。

以及，前排提醒：`F[屏蔽]k IntelliSense`。这东西有时候是真的不会提醒RISC-V的向量函数，包括数据类型。

## 1. RVV的基础概念和向量创建

### 1.1 向量类型
支持RVV扩展的RISCV芯片上包含向量寄存器组`v0~v31`。

*与x86和ARM的固定长度向量不同，这里的向量长度是可变的，且被储存在向量长度寄存器`vl`内。*

- 每个寄存器的物理长度取决于硬件最大向量长度（MVL），向量寄存器宽度**参考目标CPU架构情况**。
- 向量长度（VL）决定一次操作实际应用的元素数。该长度在运行时被[动态设置](#12-设置向量长度)。
- 寄存器组合倍数（LMUL）决定一次使用多少个寄存器，从而在一次操作中使用更多元素。

C语言中，签名格式为：
```C
v{type}m{x}_t
```
其中：
- `type`项为数据类型支持：`float32, float64, (u)int8, (u)int16, (u)int32, (u)int64, bool8, bool16`。
  - `bool8`和`bool16`类型的向量用于**掩码操作**。
- `m{x}`意为LMUL变量为`x`倍。可一次处理`x`条数据。有效值为：`f8, f4, f2, 1, 2, 4, 8`。
  - 其中`f{k}`的意思是`1/k`倍，使用小于一个寄存器长度的内容。
  - 在底层处理中，`m{x}`将使用`x`个浮点数寄存器。

例如：
```C
vfloat32m4_t
```
意为寄存器组合长度为4的float32向量类型，可储存`VL x 4`个元素。

### 1.2 设置向量长度

RVV提供了一个函数设置向量长度，形式通常为：
```C
size_t vsetvl_e{SEW}m{LMUL}(size_t avl);
```
其中：
- `SEW`为元素宽度（Standard Element Width），有效值：`8, 16, 32, 64`，可理解为“位宽”。
- `LMUL`为寄存器组合倍数，见1.1中对`m{x}`的说明。

```C
#include <riscv_vector.h>

// 想一次处理16个元素
size_t avl = 16;

// 确认实际生效的长度。
// 设置当前CPU使用的VL，如果avl>MVL，将被截断到MVL。否则返回avl的值。
// 注意：这一步必须做，否则可能会UB。
size_t vl = __riscv_vsetvl_e32m1(avl);

// 创建一个包含VL个元素的float32向量，元素的值均为1.0。
vfloat32m1_t vec = __riscv_vmv_v_x_f32m1(1.0f, vl);  
```

### 1.3 向量的创建（及与内存间的IO）

在创建特定类型的向量之前，**必须**[设置向量长度](#12-设置向量长度)。否则可能会导致未定义行为发生。

#### 1.3.0 用于向量创建及其与内存之间的IO的所有C函数

以下是RVV扩展中用于向量创建和内存IO的主要C函数列表：
- ⚠ 以下全部为模板名，请在实际使用时根据模板内容写自己需求的函数。例：`__riscv_vle32_v_i32m4`

| 返回值 | 函数名 | 参数 | 助记 | 函数作用 |
|--------|--------|------|------|----------|
| `size_t` | `__riscv_vsetvl_e{SEW}m{x}` | `(size_t avl)` | Set Vector Length | 设置向量长度和配置，返回实际设置的向量长度。<br>**建议在正式使用之前，运行本函数，获取当前设备真实向量长度及其配置。** |
| `v{ctype}m{x}_t` | `__riscv_vmv_v_x_{dtype}m{x}` | `({scalar_type} scalar, size_t vl)` | Move `Vector from Scalar` | 用标量值广播创建向量，所有元素相同 |
| `v{ctype}m{x}_t` | `__riscv_vmv_v_v_{dtype}m{x}` | `(v{type}m{x}_t src, size_t vl)` | Move `Vector from Vector` | 向量复制，从一个向量复制到另一个向量 |
| `v{ctype}m{x}_t` | `__riscv_vle{SEW}_v_{dtype}m{x}` | `(const {scalar_type} *ptr, size_t vl)` | Load Elements | 从内存加载连续元素到向量寄存器 |
| `void` | `__riscv_vse{SEW}_v_{dtype}m{x}` | `({scalar_type} *ptr, v{type}m{x}_t value, size_t vl)` | Store Elements | 将向量元素连续存储到内存 |
| `v{ctype}m{x}_t` | `__riscv_vlse{SEW}_v_{dtype}m{x}` | `(const {scalar_type} *ptr, ptrdiff_t stride, size_t vl)` | Load Strided Elements | 从内存加载跨步元素到向量，元素间隔为stride |
| `void` | `__riscv_vsse{SEW}_v_{dtype}m{x}` | `({scalar_type} *ptr, ptrdiff_t stride, v{type}m{x}_t value, size_t vl)` | Store Strided Elements | 将向量元素跨步存储到内存，元素间隔为stride |
| `v{ctype}m{x}_t` | `__riscv_vluxei{SEW}_v_{dtype}m{x}` | `(const {scalar_type} *ptr, vuint{SEW}m{x}_t index, size_t vl)` | Load Unordered Elements Indexed | 使用索引向量从内存加载元素到向量（聚集操作） |
| `void` | `__riscv_vsoxei{SEW}_v_{dtype}m{x}` | `({scalar_type} *ptr, vuint{SEW}m{x}_t index, v{type}m{x}_t value, size_t vl)` | Store Ordered Indexed Elements | 使用索引向量将向量元素存储到内存（散射操作） |
| `v{ctype}m{x}_t` | `__riscv_vle{SEW}ff_v_{dtype}m{x}` | `(const {scalar_type} *ptr, size_t *new_vl, size_t vl)` | Load Whole Register | 连续加载元素，在遇到第一个会触发异常的元素时停止，并通过new_vl返回已成功加载的元素数量。 |
| `vbool{maskindex}_t` | `__riscv_vlm_v_b{maskindex}` | `(const uint8_t *ptr, size_t vl)` | Load Mask | 从内存加载掩码向量。 |
| `void` | `__riscv_vsm_v_b{maskindex}` | `(uint8_t *ptr, vbool{SEW}_t value, size_t vl)` | Store Mask | 将掩码向量存储到内存。 |

参数说明：
- `{SEW}`: 元素宽度（8, 16, 32, 64）
- `{dtype}`: 数据类型简写
  - 整数：`i8, u8, i16, u16, i32, u32, i64, u64`
  - 浮点：`f16, f32, f64`
- `{ctype}`: 数据类型全称。
  - 整数：`int8, uint8, int16, uint16, int32, uint32, int64, float64`
  - 浮点：`_Float16/__fp16, float32, float64`
- `{x}`: 寄存器分组标识（f8, f4, f2, 1, 2, 4, 8）
  - 其中，`f2`意为`1/2`，`f4`和`f8`同理。
- `{scalar_type}`: 对应的标量类型。
  - 整数：int8_t, uint8_t, int16_t, …, uint64_t
  - 浮点：通常为 _Float16 / __fp16（16位）、float（32位）、double（64位），具体取决于编译器实现。
- `{maskindex}`: 掩码。计算公式：maskindex = SEW / 8，取整数。
  - 示例：
  - SEW = 8  → maskindex = 1 → vbool1_t / b1
  - SEW = 16 → maskindex = 2 → vbool2_t / b2
  - SEW = 32 → maskindex = 4 → vbool4_t / b4
  - SEW = 64 → maskindex = 8 → vbool8_t / b8
- `vl`: 向量长度，由`__riscv_vsetvl_e{SEW}m{x}`函数设置。
  - `avl`: 在函数`__riscv_vsetvl_e{SEW}m{x}`中，申请的向量长度，作为参数。
- `stride`: 元素之间的步长（以字节为单位）
- `index`: 索引向量，包含内存访问的偏移量
- `new_vl`: 指向存储新向量长度的变量的指针

注意：
- 老生常谈喵，`*ptr`是数组喵，数组退化喵。

使用示例：
- 在阅读使用示例时，请认真读，或者立即基于QEMU建立一个RISC-V环境。
  - *（小声）建立RISC-V环境的过程可参考[我在甲辰计划工作期间发的Repo](https://github.com/LunaticLegacy/OpencloudOsLaunch)。*

#### 1.3.1 生成单值向量
```C
#include <riscv_vector.h>

size_t avl = 16;                          
size_t vl = __riscv_vsetvl_e32m1(avl);            

// 创建一个包含vl个元素的float32向量，每个元素都是1.0
vfloat32m1_t vec = __riscv_vmv_v_x_f32m1(1.0f, vl);
```

#### 1.3.2 从数组加载

```C
#include <riscv_vector.h>
#include <stdint.h>

size_t avl = 8;
size_t vl = __riscv_vsetvl_e32m2(avl);

int32_t data[8] = {1, 1, 4, 5, 1, 4, 1, 9};  // 好臭的数组（恼）

vint32m2_t vec = __riscv_vle32_v_i32m2(data, vl);
```

#### 1.3.3 从现有的向量复制
```C
#include <riscv_vector.h>
#include <stdint.h>

size_t avl = 8;
size_t vl = __riscv_vsetvl_e32m2(avl);

int32_t data[8] = {1, 1, 4, 5, 1, 4, 1, 9};

vint32m2_t vec1 = __riscv_vle32_v_i32m2(data, vl);

// 复制向量。
vint32m2_t vec2 = __riscv_vmv_v_v_i32m2(vec1, vl);
```

#### 1.3.4 将向量的值复制到内存中
```C
#include <riscv_vector.h>
#include <stdint.h>

size_t avl = 8;
size_t vl = __riscv_vsetvl_e32m2(avl);

int32_t data[8] = {1, 1, 4, 5, 1, 4, 1, 9};
int32_t out[8];  // 预先留8个int32的空间。

vint32m2_t vec = __riscv_vle32_v_i32m2(data, vl);

// 将vec里的东西复制到out里，控制个数为vl。
__riscv_vse32_v_i32m2(out, vec, vl);
```

#### 1.3.5 跨步加载
跨步加载，即规定步长，只加载数组内部分元素进向量。
```C
#include <riscv_vector.h>
#include <stdint.h>

size_t avl = 4;
size_t vl = __riscv_vsetvl_e32m1(avl);

float data[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};

// 跨步加载，步长为8字节（2个float元素）
// 将加载data[0], data[2], data[4], data[6]
vfloat32m1_t strided_vec = __riscv_vlse32_v_f32m1(data, 8, vl);

// 将结果存储到连续内存中
float result[4];
__riscv_vse32_v_f32m1(result, strided_vec, vl);
```

#### 1.3.6 跨步存储
类似于1.3.5。
```C
#include <riscv_vector.h>
#include <stdint.h>

size_t avl = 4;
size_t vl = __riscv_vsetvl_e32m1(avl);

// 创建向量 [1.0, 2.0, 3.0, 4.0]
float init[4] = {1.0, 2.0, 3.0, 4.0};
vfloat32m1_t vec = __riscv_vle32_v_f32m1(init, vl);

// 初始化输出数组
float output[8] = {0};

// 跨步存储，步长为8字节（2个float元素）。此时映射为：
// output[0] = vec[0], output[2] = vec[1], output[4] = vec[2], output[6] = vec[3]。
__riscv_vsse32_v_f32m1(output, 8, vec, vl);

// 此时output的输出为：{1.0, 0.0, 2.0, 0.0, 3.0, 0.0, 4.0, 0.0}
```

#### 1.3.7 索引加载（聚集）
```C
#include <riscv_vector.h>
#include <stdint.h>

size_t avl = 4;
size_t vl = __riscv_vsetvl_e32m1(avl);

// 基础数据数组
float data[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};

// 创建索引向量 [0, 2, 4, 6]（元素索引，不是字节偏移）
uint32_t indices[4] = {0, 2, 4, 6};
vuint32m1_t index_vec = __riscv_vle32_v_u32m1(indices, vl);

// 索引加载（聚集操作），输入索引向量，从data中读取内容。
vfloat32m1_t gathered = __riscv_vluxei32_v_f32m1(data, index_vec, vl);

// 存储结果
float result[4];
__riscv_vse32_v_f32m1(result, gathered, vl);

// 此时，result内的元素：{1.0, 3.0, 5.0, 7.0}。
```

#### 1.3.8 索引存储（散射）
```C
#include <riscv_vector.h>
#include <stdint.h>

size_t avl = 4;
size_t vl = __riscv_vsetvl_e32m1(avl);

// 创建向量 [10.0, 20.0, 30.0, 40.0]
float init[4] = {10.0, 20.0, 30.0, 40.0};
vfloat32m1_t vec = __riscv_vle32_v_f32m1(init, vl);

// 初始化输出数组
float output[8] = {0};

// 创建索引向量 [1, 3, 5, 7]（元素索引，不是字节偏移）
uint32_t indices[4] = {1, 3, 5, 7};
vuint32m1_t index_vec = __riscv_vle32_v_u32m1(indices, vl);

// 索引存储（散射操作）
__riscv_vsoxei32_v_f32m1(output, index_vec, vec, vl);

// 此时output数组为[0.0, 10.0, 0.0, 20.0, 0.0, 30.0, 0.0, 40.0]
```


#### 1.3.9 掩码的I/O
```C
#include <riscv_vector.h>
#include <stdint.h>

size_t avl = 4;
size_t vl = __riscv_vsetvl_e32m1(avl);

// 创建掩码数据（每个字节代表一个掩码位）
uint8_t mask_data[4] = {1, 0, 1, 0}; // 只有第0和第2元素启用

// 加载掩码
vbool32_t mask = __riscv_vlm_v_b32(mask_data, vl);  // 这里为什么要b32，而不是b8？答案就在表格里。

// 数据数组
float data[4] = {1.0, 2.0, 3.0, 4.0};

// 使用掩码加载数据
// 只有掩码为1的元素会被加载，其他元素保持未定义或清零（取决于实现）
vfloat32m1_t vec = __riscv_vle32_v_f32m1_m(mask, data, vl);

// 存储掩码
uint8_t mask_out[4];
__riscv_vsm_v_b32(mask_out, mask, vl);
```


#### 1.3.10 故障容忍加载
这个函数使用比较偏向操作系统层级，平时很少用。仅作了解即可。
```C
#include <riscv_vector.h>
#include <stdint.h>

size_t avl = 8;
size_t vl = __riscv_vsetvl_e32m1(avl);

// 假设我们有一个可能包含受保护页面的数组
float data[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};

size_t new_vl;

// 故障容忍加载，遇到第一个会触发异常的元素时停止
vfloat32m1_t vec = __riscv_vle32ff_v_f32m1(data, &new_vl, vl);

// new_vl将保存成功加载的元素数量。
// 如果没有异常，(new_vl == vl) = true。
// 如果有异常，new_vl表示成功加载的元素数量。
```

## 2. 向量算子函数

警告：前方终极算子核能。实际上经常用到的算子也就那么几个，这里列举出全部算子。

整数算子：

| 返回值 | 函数名 | 参数 | 助记 | 函数作用 |
|--------|--------|------|------|----------|
| `v{type}m{x}_t` | `__riscv_vadd_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Add Vector-Vector | 向量逐元素相加 |
| `v{type}m{x}_t` | `__riscv_vadd_vx_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} b, size_t vl)` | Add Vector-Scalar | 向量与标量相加 |
| `v{type}m{x}_t` | `__riscv_vsub_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Sub Vector-Vector | 向量逐元素相减 |
| `v{type}m{x}_t` | `__riscv_vsub_vx_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} b, size_t vl)` | Sub Vector-Scalar | 向量减标量 |
| `v{type}m{x}_t` | `__riscv_vrsub_vx_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} b, size_t vl)` | Reverse Sub Vector-Scalar | 标量减向量 |
| `v{type}m{x}_t` | `__riscv_vmul_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Mul Vector-Vector | 向量逐元素相乘 |
| `v{type}m{x}_t` | `__riscv_vmul_vx_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} b, size_t vl)` | Mul Vector-Scalar | 向量与标量相乘 |
| `v{type}m{x}_t` | `__riscv_vdiv_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Div Vector-Vector | 向量逐元素相除 |
| `v{type}m{x}_t` | `__riscv_vdiv_vx_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} b, size_t vl)` | Div Vector-Scalar | 向量与标量相除 |
| `v{type}m{x}_t` | `__riscv_vrem_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Remainder Vector-Vector | 向量逐元素取余 |
| `v{type}m{x}_t` | `__riscv_vrem_vx_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} b, size_t vl)` | Remainder Vector-Scalar | 向量与标量取余 |
| `v{type}m{x}_t` | `__riscv_vmin_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Min Vector-Vector | 向量逐元素求最小值 |
| `v{type}m{x}_t` | `__riscv_vmin_vx_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} b, size_t vl)` | Min Vector-Scalar | 向量与标量求最小值 |
| `v{type}m{x}_t` | `__riscv_vmax_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Max Vector-Vector | 向量逐元素求最大值 |
| `v{type}m{x}_t` | `__riscv_vmax_vx_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} b, size_t vl)` | Max Vector-Scalar | 向量与标量求最大值 |
| `v{type}m{x}_t` | `__riscv_vand_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | And Vector-Vector | 向量间按位与 |
| `v{type}m{x}_t` | `__riscv_vor_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Or Vector-Vector | 向量间按位或 |
| `v{type}m{x}_t` | `__riscv_vxor_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Xor Vector-Vector | 向量间按位异或 |
| `v{type}m{x}_t` | `__riscv_vnot_v_{type}m{x}` | `(v{type}m{x}_t a, size_t vl)` | Not Vector | 向量间按位取反 |
| `v{type}m{x}_t` | `__riscv_vsll_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Shift Left Vector-Vector | 向量间辑左移 |
| `v{type}m{x}_t` | `__riscv_vsrl_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Shift Right Logical Vector-Vector | 向量间逻辑右移 |
| `v{type}m{x}_t` | `__riscv_vsra_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Shift Right Arithmetic Vector-Vector | 向量间算术右移 |
| `v{type}m{x}_t` | `__riscv_vmerge_vvm_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, vbool{SEW}_t mask, size_t vl)` | Merge Vector-Vector | 按掩码合并两个向量 |
| `v{type}m{x}_t` | `__riscv_vmerge_vxm_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} b, vbool{SEW}_t mask, size_t vl)` | Merge Vector-Scalar | 按掩码合并向量和标量 |
| `v{type}m{x}_t` | `__riscv_vmacc_vv_{type}m{x}` | `(v{type}m{x}_t acc, v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Multiply-Accumulate Vector-Vector | acc + a * b |
| `v{type}m{x}_t` | `__riscv_vmacc_vx_{type}m{x}` | `(v{type}m{x}_t acc, {scalar_type} a, v{type}m{x}_t b, size_t vl)` | Multiply-Accumulate Vector-Scalar | acc + a * b |
| `v{type}m{x}_t` | `__riscv_vnmsac_vv_{type}m{x}` | `(v{type}m{x}_t acc, v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Negative Multiply-Subtract Vector-Vector | acc - a * b |
| `v{type}m{x}_t` | `__riscv_vwmul_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Widen Multiply Vector-Vector | 宽乘法，结果位宽加倍 |
| `v{type}m{x}_t` | `__riscv_vwmul_vx_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} b, size_t vl)` | Widen Multiply Vector-Scalar | 宽乘法，结果位宽加倍 |
| `vbool{SEW}_t` | `__riscv_vmseq_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Compare Equal Vector-Vector | 比较相等，返回掩码向量 |
| `vbool{SEW}_t` | `__riscv_vmsne_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Compare Not Equal Vector-Vector | 比较不等 |
| `vbool{SEW}_t` | `__riscv_vmslt_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Compare Less Than Vector-Vector | 比较小于 |
| `vbool{SEW}_t` | `__riscv_vmsle_vv_{type}m{x}` | `(v{type}m{x}_t a, v{type}m{x}_t b, size_t vl)` | Compare Less Equal Vector-Vector | 比较小于等于 |
| `vbool{SEW}_t` | `__riscv_vmsgt_vx_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} b, size_t vl)` | Compare Greater Than Vector-Scalar | 比较大于（与标量） |
| `vbool{SEW}_t` | `__riscv_vmsge_vx_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} b, size_t vl)` | Compare Greater Equal Vector-Scalar | 比较大于等于（与标量） |
| `{scalar_type}` | `__riscv_vredsum_vs_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} init, size_t vl)` | Reduction Sum | 向量所有元素求和（缩减） |
| `{scalar_type}` | `__riscv_vredmax_vs_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} init, size_t vl)` | Reduction Max | 求最大值（缩减） |
| `{scalar_type}` | `__riscv_vredmin_vs_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} init, size_t vl)` | Reduction Min | 求最小值（缩减） |
| `{scalar_type}` | `__riscv_vredand_vs_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} init, size_t vl)` | Reduction And | 所有元素按位与（缩减） |
| `{scalar_type}` | `__riscv_vredor_vs_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} init, size_t vl)` | Reduction Or | 所有元素按位或（缩减） |
| `{scalar_type}` | `__riscv_vredxor_vs_{type}m{x}` | `(v{type}m{x}_t a, {scalar_type} init, size_t vl)` | Reduction Xor | 所有元素按位异或（缩减） |
| `vfloat{SEW}m{x}_t` | `__riscv_vfcvt_f_x_{type}m{x}` | `(vint{SEW}m{x}_t a, size_t vl)` | Convert Int→Float | 整数向量转浮点向量 |
| `vint{SEW}m{x}_t` | `__riscv_vfcvt_x_f_{type}m{x}` | `(vfloat{SEW}m{x}_t a, size_t vl)` | Convert Float→Int | 浮点向量转整数向量 |
| `vfloat64m{x}_t` | `__riscv_vfwcvt_f_f_v_f64m{x}` | `(vfloat32m{x}_t a, size_t vl)` | Widen Float | 单精度扩展为双精度 |
| `vfloat32m{x}_t` | `__riscv_vfncvt_f_f_w_f32m{x}` | `(vfloat64m{x}_t a, size_t vl)` | Narrow Float | 双精度收窄为单精度 |
| `vint64m{x}_t` | `__riscv_vwcvt_x_x_v_i64m{x}` | `(vint32m{x}_t a, size_t vl)` | Widen Int | 32位整数扩展为64位 |
| `vint32m{x}_t` | `__riscv_vncvt_x_x_w_i32m{x}` | `(vint64m{x}_t a, size_t vl)` | Narrow Int | 64位整数收窄为32位 |

参数说明：
- `{SEW}`: 元素宽度（8, 16, 32, 64）
- `{dtype}`: 数据类型简写
  - 整数：`i8, u8, i16, u16, i32, u32, i64, u64`
  - 浮点：`f16, f32, f64`
- `{ctype}`: 数据类型全称。
  - 整数：`int8, uint8, int16, uint16, int32, uint32, int64, float64`
  - 浮点：`_Float16/__fp16, float32, float64`
- `{x}`: 寄存器分组标识（f8, f4, f2, 1, 2, 4, 8）
  - 其中，`f2`意为`1/2`，`f4`和`f8`同理。
- `{scalar_type}`: 对应的标量类型。
  - 整数：int8_t, uint8_t, int16_t, …, uint64_t
  - 浮点：通常为 _Float16 / __fp16（16位）、float（32位）、double（64位），具体取决于编译器实现。
- `{maskindex}`: 掩码。计算公式：maskindex = SEW / 8，取整数。
  - 示例：
  - SEW = 8  → maskindex = 1 → vbool1_t / b1
  - SEW = 16 → maskindex = 2 → vbool2_t / b2
  - SEW = 32 → maskindex = 4 → vbool4_t / b4
  - SEW = 64 → maskindex = 8 → vbool8_t / b8
- `vl`: 向量长度，由`__riscv_vsetvl_e{SEW}m{x}`函数设置。


浮点数算子：

| 返回值 | 函数名 | 参数 | 助记 | 函数作用 |
|--------|--------|------|------|----------|
| `vfloat{SEW}m{x}_t` | `__riscv_vfadd_vv_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | Add Float Vector-Vector | 浮点向量逐元素相加  |
| `vfloat{SEW}m{x}_t` | `__riscv_vfadd_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | Add Float Vector-Scalar | 向量与浮点标量相加  |
| `vfloat{SEW}m{x}_t` | `__riscv_vfsub_vv_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | Sub Float Vector-Vector | 向量逐元素相减（a - b） |
| `vfloat{SEW}m{x}_t` | `__riscv_vfsub_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | Sub Float Vector-Scalar | 向量减标量（a - b） |
| `vfloat{SEW}m{x}_t` | `__riscv_vfrsub_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | Reverse Sub Float | 反向相减（b - a），常用于做 `c - x[i]` 这种形式 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfmul_vv_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | Mul Float Vector-Vector | 浮点向量逐元素相乘 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfmul_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | Mul Float Vector-Scalar | 向量与标量相乘 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfdiv_vv_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | Div Float Vector-Vector | 向量逐元素相除（a / b）  |
| `vfloat{SEW}m{x}_t` | `__riscv_vfdiv_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | Div Float Vector-Scalar | 向量除以标量（a / b） |
| `vfloat{SEW}m{x}_t` | `__riscv_vfrdiv_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | Reverse Div Float | 反向相除（b / a），写成向量形式方便编译器匹配指令 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfmacc_vv_f{SEW}m{x}`  | `(vfloat{SEW}m{x}_t acc, vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | Fused Mul-Acc VV | `acc + a * b`，典型 MAC（向量×向量） |
| `vfloat{SEW}m{x}_t` | `__riscv_vfmacc_vf_f{SEW}m{x}`  | `(vfloat{SEW}m{x}_t acc, {fscalar_type} a, vfloat{SEW}m{x}_t b, size_t vl)` | Fused Mul-Acc VF | `acc + a * b`，标量×向量 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfmsac_vv_f{SEW}m{x}`  | `(vfloat{SEW}m{x}_t acc, vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | Fused Mul-Sub VV | `acc - a * b` |
| `vfloat{SEW}m{x}_t` | `__riscv_vfmsac_vf_f{SEW}m{x}`  | `(vfloat{SEW}m{x}_t acc, {fscalar_type} a, vfloat{SEW}m{x}_t b, size_t vl)` | Fused Mul-Sub VF | 同上，标量×向量 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfnmacc_vv_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t acc, vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | Neg Fused Mul-Acc VV | `-acc + a * b` |
| `vfloat{SEW}m{x}_t` | `__riscv_vfnmacc_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t acc, {fscalar_type} a, vfloat{SEW}m{x}_t b, size_t vl)` | Neg Fused Mul-Acc VF | `-acc + a * b` |
| `vfloat{SEW}m{x}_t` | `__riscv_vfnmsac_vv_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t acc, vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | Neg Fused Mul-Sub VV | `-acc - a * b` |
| `vfloat{SEW}m{x}_t` | `__riscv_vfnmsac_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t acc, {fscalar_type} a, vfloat{SEW}m{x}_t b, size_t vl)` | Neg Fused Mul-Sub VF | `-acc - a * b` |
| `vfloat{SEW}m{x}_t` | `__riscv_vfmin_vv_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | Min Float VV | 浮点逐元素最小值 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfmin_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | Min Float VF | 向量与标量最小值 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfmax_vv_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | Max Float VV | 浮点逐元素最大值 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfmax_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | Max Float VF | 向量与标量最大值 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfneg_v_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, size_t vl)` | Neg Float | 逐元素取负号，相当于 `-a`（内部是 `vfsgnjn` 伪指令）|
| `vfloat{SEW}m{x}_t` | `__riscv_vfabs_v_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, size_t vl)` | Abs Float | 逐元素 `fabs`，内部用 `vfsgnjx` 实现 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfsgnj_vv_f{SEW}m{x}`  | `(vfloat{SEW}m{x}_t mag, vfloat{SEW}m{x}_t sign, size_t vl)` | Sign Inject VV | 保留 `mag` 的绝对值，注入 `sign` 的符号 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfsgnj_vf_f{SEW}m{x}`  | `(vfloat{SEW}m{x}_t mag, {fscalar_type} sign, size_t vl)` | Sign Inject VF | 向量幅值 + 标量符号 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfsgnjn_vv_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t mag, vfloat{SEW}m{x}_t sign, size_t vl)` | Neg Sign Inject VV | 和上面类似，但取反符号 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfsgnjn_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t mag, {fscalar_type} sign, size_t vl)` | Neg Sign Inject VF | 同上，VF 版本 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfsgnjx_vv_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | XOR Sign VV | 符号位异或 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfsgnjx_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | XOR Sign VF | 符号位异或（VF） |
| `vfloat{SEW}m{x}_t` | `__riscv_vfsqrt_v_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, size_t vl)` | Sqrt Float | 逐元素平方根 `sqrt(a)` |
| `vfloat{SEW}m{x}_t` | `__riscv_vfrec7_v_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, size_t vl)` | Reciprocal Approx | 倒数近似（约 7 bit 精度），用于迭代求倒数 |
| `vfloat{SEW}m{x}_t` | `__riscv_vfrsqrt7_v_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, size_t vl)` | Rsqrt Approx | 1/√x 近似（约 7 bit 精度），用于迭代 sqrt 加速 |
| `vbool{maskindex}_t` | `__riscv_vmfeq_vv_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | Float Equal VV| 比较 `a == b`，结果为掩码向量 |
| `vbool{maskindex}_t` | `__riscv_vmfeq_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | Float Equal VF | `a[i] == b` |
| `vbool{maskindex}_t` | `__riscv_vmfne_vv_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | Float NotEqual VV | `a != b` |
| `vbool{maskindex}_t` | `__riscv_vmfne_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | Float NotEqual VF | `a[i] != b` |
| `vbool{maskindex}_t` | `__riscv_vmflt_vv_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | Float Less VV | `a < b` |
| `vbool{maskindex}_t` | `__riscv_vmflt_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | Float Less VF | `a[i] < b` |
| `vbool{maskindex}_t` | `__riscv_vmfle_vv_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, vfloat{SEW}m{x}_t b, size_t vl)` | Float LessEqual VV | `a ≤ b` |
| `vbool{maskindex}_t` | `__riscv_vmfle_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | Float LessEqual VF | `a[i] ≤ b` |
| `vbool{maskindex}_t` | `__riscv_vmfgt_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | Float Greater VF | `a[i] > b`（只定义 VF   |
| `vbool{maskindex}_t` | `__riscv_vmfge_vf_f{SEW}m{x}` | `(vfloat{SEW}m{x}_t a, {fscalar_type} b, size_t vl)` | Float GreaterEqual VF | `a[i] ≥ b`（只定义 VF 形式） |


参数说明：
- `{SEW}`: 元素宽度（8, 16, 32, 64）
- `{x}`: 寄存器分组标识（f8, f4, f2, 1, 2, 4, 8）
  - 其中，`f2`意为`1/2`，`f4`和`f8`同理。
- `{fscalar_type}`: 对应的标量类型。
  - 浮点：通常为 _Float16 / __fp16（16位）、float（32位）、double（64位），具体取决于编译器实现。
- `{maskindex}`: 掩码。计算公式：maskindex = SEW / 8，取整数。
  - 示例：
  - SEW = 8  → maskindex = 1 → vbool1_t / b1
  - SEW = 16 → maskindex = 2 → vbool2_t / b2
  - SEW = 32 → maskindex = 4 → vbool4_t / b4
  - SEW = 64 → maskindex = 8 → vbool8_t / b8
- `vl`: 向量长度，由`__riscv_vsetvl_e{SEW}m{x}`函数设置。

返回值说明：
- `vfloat{SEW}m{x}_t`: RVV浮点数向量类型。

## 3. 作业

作业：（来自Python库：[noise](https://github.com/LunaticLegacy/noise)的源代码）
```C
#include <math.h>

static inline float
grad2(const int hash, const float x, const float y)
{
	const int h = hash & 15;
	return x * GRAD3[h][0] + y * GRAD3[h][1];
}

float
noise2(float x, float y, const float repeatx, const float repeaty, const int base)
{
	float fx, fy;
	int A, AA, AB, B, BA, BB;
	int i = (int)floorf(fmodf(x, repeatx));
	int j = (int)floorf(fmodf(y, repeaty));
	int ii = (int)fmodf(i + 1, repeatx);
	int jj = (int)fmodf(j + 1, repeaty);
	i = (i & 255) + base;
	j = (j & 255) + base;
	ii = (ii & 255) + base;
	jj = (jj & 255) + base;

	x -= floorf(x); y -= floorf(y);
	fx = x*x*x * (x * (x * 6 - 15) + 10);
	fy = y*y*y * (y * (y * 6 - 15) + 10);

	A = PERM[i];
	AA = PERM[A + j];
	AB = PERM[A + jj];
	B = PERM[ii];
	BA = PERM[B + j];
	BB = PERM[B + jj];
		
	return lerp(fy, lerp(fx, grad2(PERM[AA], x, y),
							 grad2(PERM[BA], x - 1, y)),
					lerp(fx, grad2(PERM[AB], x, y - 1),
							 grad2(PERM[BB], x - 1, y - 1)));
}
```
任务：
- 将上述函数改写为支持RVV向量运算的函数。
