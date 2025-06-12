#include "calcpi.h"
#include <QByteArray>
#include <QString>
#include <cstdio>
#include <cstdlib>
using namespace std;

// 构造函数：初始化所有成员变量为默认状态
CalcPI::CalcPI() {
    L = 0;
    N = 0;
    s = nullptr;
    w = nullptr;
    v = nullptr;
    q = nullptr;
}

// 析构函数：释放动态分配的数组
CalcPI::~CalcPI() {
    ClearAll();
}

// 主计算函数，计算圆周率到指定长度
void CalcPI::Calc(int length) {
    if (length < 1) return; // 长度非法则返回

    ClearAll();         // 释放之前的数组
    InitArrays(length); // 重新初始化数组

    // 根据 Machin 公式计算 π：
    // π = 16 * arctan(1/5) - 4 * arctan(1/239)
    int n = static_cast<int>(L / 1.39793 + 1); // 推测需要多少项级数展开
    int k;

    w[0] = 16 * 5;  // 表示 16 / 5
    v[0] = 4 * 239; // 表示 4 / 239

    for (k = 1; k <= n; k++) {
        div(w, 25, w);        // w /= 25 => 相当于 1/(5^2)
        div(v, 239, v);       // v /= 239
        div(v, 239, v);       // v /= 239 再次除 => 相当于 1/(239^2)
        sub(w, v, q);         // q = w - v
        div(q, 2 * k - 1, q); // q /= (2k - 1)

        if (k % 2) {
            add(s, q, s); // 奇数项加
        } else {
            sub(s, q, s); // 偶数项减
        }
    }
}

// 释放所有动态分配的数组
void CalcPI::ClearAll() {
    delete[] s;
    s = nullptr;
    delete[] w;
    w = nullptr;
    delete[] v;
    v = nullptr;
    delete[] q;
    q = nullptr;
}

// 初始化所有数组，并清零
void CalcPI::InitArrays(int length) {
    L = length;    // 要计算的位数
    N = L / 4 + 1; // 每个数组元素存储 4 位十进制数，因此长度为 L/4 + 1

    s = new int[N + 3]; // 多分配几个用于安全边界
    w = new int[N + 3];
    v = new int[N + 3];
    q = new int[N + 3];

    for (int i = 0; i < N + 3; i++) {
        s[i] = 0;
        w[i] = 0;
        v[i] = 0;
        q[i] = 0;
    }
}

// 高精度加法：c = a + b
void CalcPI::add(int* a, int* b, int* c) {
    int carry = 0;
    for (int i = N + 1; i >= 0; i--) {
        c[i] = a[i] + b[i] + carry;
        if (c[i] < 10000) {
            carry = 0;
        } else {
            c[i] -= 10000;
            carry = 1;
        }
    }
}

// 高精度减法：c = a - b
void CalcPI::sub(int* a, int* b, int* c) {
    int borrow = 0;
    for (int i = N + 1; i >= 0; i--) {
        c[i] = a[i] - b[i] - borrow;
        if (c[i] >= 0) {
            borrow = 0;
        } else {
            c[i] += 10000;
            borrow = 1;
        }
    }
}

// 高精度整数除法：c = a / b
void CalcPI::div(int* a, int b, int* c) {
    int remain = 0;
    for (int i = 0; i <= N + 1; i++) {
        int tmp = a[i] + remain;
        c[i] = tmp / b;
        remain = (tmp % b) * 10000; // 保留余数乘进位
    }
}

// 打印 π 到控制台（如终端）
void CalcPI::PrintPI() {
    if (L < 1) return;
    printf("%d.", s[0]); // 整数部分
    for (int k = 1; k < N; k++) {
        printf("%04d", s[k]); // 每段补齐4位
    }
    printf("\n");
}

// 将 π 写入到指定的 QFile 文件
void CalcPI::WriteToFile(QFile& fileOut) {
    if (L < 1) return;

    // 打头的整数部分 + 小数点
    QString strTemp = QFile::tr("%1.").arg(s[0]);
    fileOut.write(strTemp.toUtf8());

    // 写入小数部分
    for (int k = 1; k < N; k++) {
        strTemp = QFile::tr("%1").arg(s[k], 4, 10, QChar('0')); // 补足4位，不足前导0
        fileOut.write(strTemp.toUtf8());
    }
}
