#include "showmsg.h"
#include <QMessageBox>

ShowMsg::ShowMsg(QObject* parent) : QObject{parent} {
}

void ShowMsg::recvMsg(QString str){
    // 第一个参数是父窗口指针，设置为 NULL，代表没有父窗口，就是在系统桌面直接弹窗
    QMessageBox::information(NULL, tr("Show"), str);
}
