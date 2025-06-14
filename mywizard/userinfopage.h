#ifndef USERINFO_PAGE_H
#define USERINFO_PAGE_H

#include <QLineEdit>
#include <QWizardPage>

// UserInfoPage 继承自 QWizardPage，表示向导中的“用户信息”页面
class UserInfoPage : public QWizardPage {
    Q_OBJECT
  public:
    // 构造函数，允许传入父窗口指针，默认 nullptr
    explicit UserInfoPage(QWidget* parent = nullptr);

  private:
    QLineEdit* nameEdit; // 用于输入用户名的文本编辑控件
};

#endif // USERINFO_PAGE_H
