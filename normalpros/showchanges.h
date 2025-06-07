#ifndef SHOWCHANGES_H
#define SHOWCHANGES_H

#include <QObject>

class ShowChanges : public QObject {
    Q_OBJECT
public:
    explicit ShowChanges(QObject* parent = nullptr);

public slots:
    void recvValue(double v); // 用于接收 valueChanged 信号的槽函数
    void recvNickName(const QString& strNewName);
    void recvCount(int nNewCount);
};

#endif // SHOWCHANGES_H
