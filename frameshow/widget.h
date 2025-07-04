#ifndef WIDGET_H
#define WIDGET_H

#include <QFrame>
#include <QList>
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
    void on_radioButtonNull_clicked();

    void on_radioButtonRed_clicked();

    void on_radioButtonBlue_clicked();

    void on_radioButtonGreen_clicked();

    void on_spinBoxLineWidth_valueChanged(int arg1);

    void on_spinBoxMidLineWidth_valueChanged(int arg1);

    void on_comboBoxFrameShape_currentIndexChanged(int index);

    void on_comboBoxFrameShadow_currentIndexChanged(int index);

  private:
    Ui::Widget* ui;
    // 保存 FrameShape
    QList<QFrame::Shape> m_listFrameShape;
    // 保存 FrameShadow
    QList<QFrame::Shadow> m_listFrameShadow;
};
#endif // WIDGET_H
