#ifndef WIDGET_H
#define WIDGET_H

#include <QFileInfo>
#include <QImage>
#include <QImageWriter> //获取转换输出支持的图片格式
#include <QLabel>
#include <QPixmap>
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
    void initControls();

  private slots:
    void on_pushButtonOpen_clicked();

    void on_pushButtonConvert_clicked();

  private:
    Ui::Widget* ui;
    // 图片预览标签
    QLabel* m_pLabelPreview;
    // 图片文件名
    QString m_strImageName;
    // 加载图片对象
    QImage m_image;
};
#endif // WIDGET_H
