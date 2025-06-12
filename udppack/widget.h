#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

// 设置结构体按 1 字节对齐，避免编译器自动填充字节造成结构体大小不一致
#pragma pack(1)

// 定义一个 UDP 报文的结构体，用于模拟 UDP 数据包的头部与数据部分
struct UDPPacker {
    quint16 m_srcPort;  // 源端口号
    quint16 m_dstPort;  // 目标端口号
    quint16 m_length;   // 报文长度（包含头部8字节 + 数据长度）
    quint16 m_checksum; // UDP 校验和（本示例中未进行实际计算，默认为0）
    QByteArray m_data;  // 报文数据内容

    // 声明两个友元函数，用于支持 QDataStream 的序列化和反序列化操作
    friend QDataStream& operator<<(QDataStream& stream, const UDPPacker& udp);
    friend QDataStream& operator>>(QDataStream& stream, UDPPacker& udp);
};

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget; // 前向声明 UI 类，避免包含 ui_widget.h（由 uic 自动生成）
}
QT_END_NAMESPACE

// 主窗口类，继承自 QWidget
class Widget : public QWidget {
    Q_OBJECT

  public:
    // 构造函数和析构函数
    Widget(QWidget* parent = nullptr);
    ~Widget();

  private slots:
    // 添加一个 UDP 报文到列表
    void on_btnAddUDP_clicked();

    // 删除选中的 UDP 报文
    void on_btnDelUDP_clicked();

    // 保存 UDP 报文列表到文件
    void on_btnSave_clicked();

    // 从文件加载 UDP 报文列表
    void on_btnLoad_clicked();

    // 当前列表选中项变化时触发，用于更新显示到输入框
    void on_listWidget_currentRowChanged(int currentRow);

  private:
    Ui::Widget* ui; // UI 指针，用于访问界面控件
};

#endif // WIDGET_H
