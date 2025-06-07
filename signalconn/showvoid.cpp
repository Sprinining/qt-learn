#include "showvoid.h"
#include <QMessageBox>

ShowVoid::ShowVoid(QObject* parent) : QObject{parent} {
}

void ShowVoid::recvVoid(){
    QMessageBox::information(NULL, tr("Show"), tr("Just void."));
}
