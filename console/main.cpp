#include <QCoreApplication>
#include <QTextStream>

// 函数声明：根据功能代码执行对应逻辑
void funcs(int nCode, QTextStream& tsIn, QTextStream& tsOut);

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    QTextStream tsIn(stdin);   // 用于从标准输入读取（相当于 cin）
    QTextStream tsOut(stdout); // 用于向标准输出写入（相当于 cout）

    while (true) {
        // 显示功能菜单
        QString strFuns = a.tr("功能代码：\n"
                               "1. 输入整型数\n"
                               "2. 输入浮点数\n"
                               "3. 输入单词\n"
                               "4. 输入整行句子\n"
                               "9. 退出程序\n"
                               "请输入功能代码： ");
        tsOut << strFuns << Qt::flush; // 显示菜单并立即刷新缓冲区（避免不显示）

        int nCode;
        tsIn >> nCode; // 读取用户输入的功能编号

        if (nCode == 9) {
            tsOut << a.tr("程序结束。") << Qt::endl;
            return 0; // 用户选择退出
        } else {
            funcs(nCode, tsIn, tsOut); // 执行对应功能
        }
    }

    return a.exec(); // 实际不会执行到这里
}

// 功能函数，根据编号执行对应任务
void funcs(int nCode, QTextStream& tsIn, QTextStream& tsOut) {
    tsOut << Qt::endl; // 开始前空一行

    QString strOut;  // 输出用字符串
    QString strIn;   // 用于接收单词或整行输入
    int nNum;        // 用于接收整数
    double dblValue; // 用于接收浮点数

    switch (nCode) {
    case 1: {
        // 输入整数
        strOut = qApp->tr("请输入整数： ");
        tsOut << strOut << Qt::flush;
        tsIn >> nNum;
        strOut = qApp->tr("您刚输入的是：%1").arg(nNum);
        tsOut << strOut << Qt::endl;
        break;
    }
    case 2: {
        // 输入浮点数
        strOut = qApp->tr("请输入浮点数： ");
        tsOut << strOut << Qt::flush;
        tsIn >> dblValue;
        strOut = qApp->tr("您刚输入的是：%1").arg(dblValue);
        tsOut << strOut << Qt::endl;
        break;
    }
    case 3: {
        // 输入单词（空格分隔的一个词）
        strOut = qApp->tr("请输入一个单词： ");
        tsOut << strOut << Qt::flush;
        tsIn >> strIn;
        strOut = qApp->tr("您刚输入的是： %1").arg(strIn);
        tsOut << strOut << Qt::endl;
        break;
    }
    case 4: {
        // 输入一整行
        strOut = qApp->tr("请输入一行字符串： ");
        tsOut << strOut << Qt::flush;
        tsIn.skipWhiteSpace();   // 跳过前面回车等空白字符
        strIn = tsIn.readLine(); // 读取整行（包括空格）
        strOut = qApp->tr("您刚输入的是： %1").arg(strIn);
        tsOut << strOut << Qt::endl;
        break;
    }
    default: {
        // 未知功能代码
        strOut = qApp->tr("未知功能代码 %1 ，不处理。").arg(nCode);
        tsOut << strOut << Qt::endl;
        break;
    }
    }

    tsOut << Qt::endl; // 功能处理完后空一行

    // 错误处理：如果输入出错，比如输入字母给数字字段
    if (tsIn.status() != QTextStream::Ok) {
        tsIn.readLine();    // 跳过当前输入行，防止错误遗留
        tsIn.resetStatus(); // 重置输入流状态为正常
    }
}
