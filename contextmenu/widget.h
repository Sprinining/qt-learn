#ifndef WIDGET_H
#define WIDGET_H

#include <QAction>     //菜单项
#include <QListWidget> //列表控件
#include <QMenu>       //菜单
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

  public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

  public slots:
    // 弹出右键菜单的槽函数
    void onCustomContextMenuRequested(const QPoint& pos);
    // 添加条目菜单项的槽函数
    void onAddItemTriggered();
    // 编辑条目菜单项的槽函数
    void onEditItemTriggered();
    // 删除条目菜单项的槽函数
    void onDelItemTriggered();
    // 清空所有条目的菜单项槽函数
    void onClearAllTriggered();

  private:
    Ui::Widget* ui;
    // 保存右键菜单的指针
    QMenu* m_menuContext;
    // 创建菜单并关联信号和槽函数
    void createMenu();
};
#endif // WIDGET_H
