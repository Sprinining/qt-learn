#include "showchanges.h"
#include <QDebug>

ShowChanges::ShowChanges(QObject* parent) : QObject{parent} {
}

void ShowChanges::recvValue(double v) {
    qDebug() << "recvValue: " << v;
}

void ShowChanges::recvNickName(const QString& strNewName) {
    qDebug() << "recvNickName: " << strNewName;
}

void ShowChanges::recvCount(int nNewCount) {
    qDebug() << "recvCount: " << nNewCount;
}
