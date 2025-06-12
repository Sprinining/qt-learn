#ifndef CALCPI_H
#define CALCPI_H

#include <QFile> // 支持文件写入功能，配合 Qt 使用

// 计算圆周率类 CalcPI
class CalcPI {
  public:
    // 构造函数
    CalcPI();

    // 析构函数
    ~CalcPI();

    // 主函数：计算圆周率到指定长度
    void Calc(int length);

    // 打印结果到命令行（例如 qDebug）
    void PrintPI();

    // 写入计算结果到文件
    void WriteToFile(QFile& fileOut);

  private:
    int L; // 要计算的 π 的位数（精度）
    int N; // 内部数组的实际长度（与位数 L 有关，可能大于 L）

    // 用于高精度计算的整数数组
    int* s; // 最终结果数组
    int* w; // 临时工作数组
    int* v; // 中间计算用数组
    int* q; // 辅助数组

    // 初始化数组，分配内存并设置初值
    void InitArrays(int length);

    // 清理所有动态数组，释放内存
    void ClearAll();

    // 高精度加法：res = a + b
    void add(int* a, int* b, int* res);

    // 高精度减法：res = a - b
    void sub(int* a, int* b, int* res);

    // 高精度除法：res = a / b（b 是普通整数）
    void div(int* a, int b, int* res);
};

#endif // CALCPI_H
