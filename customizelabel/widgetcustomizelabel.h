#ifndef WIDGETCUSTOMIZELABEL_H
#define WIDGETCUSTOMIZELABEL_H

#include <QColorDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class WidgetCustomizeLabel;
}
QT_END_NAMESPACE

// 自定义控件类：用于实时调整 QLabel 的样式，如字体、前景色、背景色、文本等
class WidgetCustomizeLabel : public QWidget {
    Q_OBJECT

  public:
    // 构造函数，初始化界面和控件
    WidgetCustomizeLabel(QWidget* parent = nullptr);
    // 析构函数，释放资源
    ~WidgetCustomizeLabel();

  public slots:
    // 自定义槽函数：接收颜色变化信号，动态设置前景色
    void recvAndSetForegroundColor(QColor color);

  private slots:
    // 点击“前景色”按钮触发的槽函数，显示颜色选择对话框
    void on_pushButtonForeground_clicked();

    // 点击“背景色”按钮触发的槽函数，弹出颜色对话框并应用背景色
    void on_pushButtonBackground_clicked();

    // 点击“字体”按钮触发的槽函数，打开字体选择对话框
    void on_pushButtonFont_clicked();

    // 点击“文本”按钮触发的槽函数，弹出多行输入对话框修改文本
    void on_pushButtonText_clicked();

  private:
    Ui::WidgetCustomizeLabel* ui; // UI 控件对象指针
    QColor m_clrForeground;       // 保存前景色（字体颜色）
    QColor m_clrBackground;       // 保存背景色
    QFont m_font;                 // 保存当前字体样式
    QString m_strText;            // 保存标签显示的文本

    QColorDialog* m_pDlgForeground; // 自定义的颜色选择对话框（用于前景色）

    void init(); // 初始化函数，设置默认样式并连接信号槽
};

#endif // WIDGETCUSTOMIZELABEL_H
