#ifndef FINAL_PAGE_H
#define FINAL_PAGE_H

#include <QLabel>
#include <QWizardPage>

// FinalPage 继承自 QWizardPage，表示向导中的“完成”页面
class FinalPage : public QWizardPage {
    Q_OBJECT
  public:
    // 构造函数，允许传入父窗口指针，默认 nullptr
    explicit FinalPage(QWidget* parent = nullptr);

    // 重写 QWizardPage 的 initializePage() 方法
    // 每次页面显示时会调用，用于初始化页面内容
    void initializePage() override;

  private:
    QLabel* infoLabel; // 用于显示欢迎信息的标签控件
};

#endif // FINAL_PAGE_H
