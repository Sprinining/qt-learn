#ifndef WIDGET_H
#define WIDGET_H

#include <QDataStream>     //用数据流保存树
#include <QFile>           //文件类
#include <QTreeWidget>     //树形控件
#include <QTreeWidgetItem> //树形条目
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
    void on_btnAddTop_clicked();

    void on_btnAddChild_clicked();

    void on_btnEditHeader_clicked();

    void on_btnSaveFile_clicked();

    void on_btnClearTree_clicked();

    void on_btnLoadFile_clicked();

  private:
    Ui::Widget* ui;
    // 文件对象，用于保存或打开
    QFile m_file;
    // 数据流对象
    QDataStream m_ds;
    // 保存树的先序递归函数，自顶向下保存
    void saveTree(QTreeWidgetItem* curItem);
    // 加载树的先序递归函数，自顶向下创建树形结构
    void loadFile(QTreeWidgetItem* curItem);
    // 加载时的列数限制
    static const int MAX_COLS = 1000;
};
#endif // WIDGET_H
