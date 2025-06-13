#ifndef WIDGET_H
#define WIDGET_H

#include <QTreeWidget>             //树形控件
#include <QTreeWidgetItem>         //树形条目
#include <QTreeWidgetItemIterator> //树形迭代器
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

  private slots:
    void on_btnPreorder_clicked();

    void on_btnPostorder_clicked();

    void on_btnMidorder_clicked();

    void on_btnLevelorder_clicked();

    void on_btnIterator_clicked();

  private:
    Ui::Widget* ui;
    // 先序遍历递归函数，只打印字符，不需要返回值
    void preorderTraversal(QTreeWidgetItem* curItem);
    // 后序遍历递归函数
    void postorderTraversal(QTreeWidgetItem* curItem);
    // 中序遍历递归函数
    void midorderTraversal(QTreeWidgetItem* curItem);
    // 迭代器遍历
    void iteratorTraversal(QTreeWidgetItem* curItem);
    // 按层遍历
    void levelorderTraversal(QTreeWidgetItem* curItem);
};
#endif // WIDGET_H
