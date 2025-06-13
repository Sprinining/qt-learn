#ifndef WIDGET_H
#define WIDGET_H

// Qt 基础控件与布局
#include <QHBoxLayout>   // 水平布局器，用于主界面布局
#include <QLabel>        // 用于显示地图图片
#include <QList>         // 点的列表
#include <QPixmap>       // 图片对象
#include <QPoint>        // 坐标点类型
#include <QRadioButton>  // 右侧用于省份选择的单选按钮
#include <QScrollArea>   // 滚动区域容器，用于实现视图滚动
#include <QSignalMapper> // 信号映射器，用于简化多个按钮信号的统一处理
#include <QStringList>   // 字符串列表（省份名）
#include <QVBoxLayout>   // 垂直布局器，用于按钮区域布局

#include <QWidget> // 所有可视界面类的基类 QWidget

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

// 主窗口类 Widget，继承自 QWidget
class Widget : public QWidget {
    Q_OBJECT

  public:
    // 构造函数和析构函数
    Widget(QWidget* parent = nullptr);
    ~Widget();

    // 初始化控件和界面布局
    void initControls();

  public slots:
    // 槽函数：当某个省份按钮被点击后调用，滚动地图到该省份位置
    void showProvince(int index);

  private:
    Ui::Widget* ui; // UI 指针（Qt Designer 生成）

    // ====== 数据成员 ======

    // 省份名称列表
    QStringList m_listProvinces;

    // 省份对应的坐标点列表
    QList<QPoint> m_listPoints;

    // 地图图片资源
    QPixmap m_map;

    // 加载地图图片和省份文本数据的私有函数
    void loadData();

    // ====== 控件成员 ======

    // 显示地图的 QLabel 控件
    QLabel* m_labelMap;

    // 左边滚动区域：包裹地图 QLabel
    QScrollArea* m_saLeftMap;

    // 右边滚动区域：包裹省份按钮列表
    QScrollArea* m_saRightButtons;
};

#endif // WIDGET_H
