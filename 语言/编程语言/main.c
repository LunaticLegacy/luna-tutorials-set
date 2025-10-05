#include <stdio.h>
#include <limits.h>
#include <pthread.h>
#include <memory.h>
#include <stdlib.h>
#include <fileapi.h>

void basic_field() {
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

    // 算数
    int alpha = a + b;
    long bravo = d - b;
    long long charlie = d * ua;
    long long delta = a / c;
    char echo = s % c;

}

// 作用域内存覆写测试
int scope_test() {
    puts("Enter scope_test.");
    int outer = 3;    // 在作用域外定义3
    volatile int* pointer;  // volatile：告诉编译器不要优化掉它
    {
        int inner = 5;  // 在作用域内定义5
        pointer = &inner;   // 写入指针
    }
    short overwrite = 8;
    printf("%d\n", *pointer);
    puts("Exit scope_test.");
}

// 函数指针测试

int fibonacci(int n) {
    if (n < 2) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int funcptr_test() {
    puts("Enter funcptr_test.");
    int (*fib)(int) = &fibonacci;
    
    int result1 = (*fib)(5), result2 = fib(5);
    printf("%d, %d\n", result1, result2);

    puts("Exit funcptr_test.");
}


// 多线程工作用线程
void *worker(void *arg) {
    int id = *(int*)arg;
    printf("Executing thread %d...\n", id);
    return NULL;
}

void thread_test() {
    puts("Enter thread_test.");
    // 线程测试
    // 批量在堆上创建变量。
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;
    pthread_create(&t1, NULL, worker, &id1);
    pthread_create(&t2, NULL, worker, &id2);
    pthread_join(t1, NULL); // join函数会阻塞时间，
    pthread_join(t2, NULL);
    puts("Exit scope_test.");
}

// 内存对齐测试

#pragma pack(push, 1)  // 1字节对齐
struct MyStruct {
    char a;
    int b;
};
#pragma pack(pop)

#pragma pack(push, 2)  // 2字节对齐
struct YourStruct {
    char a;
    int b;
};
#pragma pack(pop)

#pragma pack(push, 4)  // 4字节对齐
struct HisStruct {
    char a;
    int b;
};
#pragma pack(pop)

#pragma pack(push, 8)  // 8字节对齐
struct HerStruct {
    char a;
    int b;
};
#pragma pack(pop)

#pragma pack(push, 16)  // 16字节对齐……呃，好像太大了点吧。
struct ItsStruct {
    char a;
    int b;
};
#pragma pack(pop)

void pack_test() {
    puts("Enter pack_test.");
    printf("sizeof(MyStruct) = %zu\n", sizeof(struct MyStruct));    // 5
    printf("sizeof(YourStruct) = %zu\n", sizeof(struct YourStruct)); // 6
    printf("sizeof(HisStruct) = %zu\n", sizeof(struct HisStruct));  // 8
    printf("sizeof(HerStruct) = %zu\n", sizeof(struct HerStruct));  // 8
    printf("sizeof(ItsStruct) = %zu\n", sizeof(struct ItsStruct));  // 8
    puts("Exit pack_test.");
}

void ptr_alg_test() {
    puts("Enter ptr_alg_test.");
    int* k = (int*)1000;  // 直接写一个访问内存位置1000（0x3E8）的东西——实际极度不推荐这样写
    int* m = k + 1;
    int* n = k - 1;
    printf("%d, %d\n", m, n);
    int* diff = (int*)(m - n);
    printf("%d\n", diff);
    puts("Exit ptr_alg_test.");
}

int main() {
    // 基础内容和作用域。
    basic_field();
    // 作用域
    scope_test();
    // 函数指针测试
    funcptr_test();
    // 线程测试
    thread_test();
    // 内存对齐测试
    pack_test();
    // 指针算术测试
    ptr_alg_test();
    return 0;
}


