#include <stdio.h>
#include <limits.h>
int main() {
    {
        int outer = 3;    // 在作用域外定义3
        int* pointer;
        {
            int inner = 5;  // 在作用域内定义5
            pointer = &inner;
        }
        short overwrite = 8;
        outer = *pointer;    // 该操作是不被允许的。
        printf("%d\n", outer);
    }

    // 整数，也是内存的储存单位
    char c = 2;     // 字节
    short s = 4;    // 短整数（字，16位）
    int a = 3;      // 整数，变位（随平台和编译器实现而改变）
    long b = 4;     // 长整数，变位
    long long d = 5;    // 超长整数，变位
    unsigned int ua = 199;  // 无符号整数，变位

    // 小数
    float fp = 114.514;  // 32位浮点
    double dp = 114514.1919810;  // 64位浮点数
    long double lfp = 114514.1919810893931L;  // 超长浮点数，变位
    
    // 指针相关
    char* ptr_char = &c;    // 指向C

    // 常量指针
    const long* ptr_c_long = &b;    // 指向b的地址，编译器规定不能使用该值修改b
}
