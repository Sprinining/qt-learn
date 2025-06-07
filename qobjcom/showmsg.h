#ifndef SHOWMSG_H
#define SHOWMSG_H

#include <QObject>

class ShowMsg : public QObject {
    Q_OBJECT
public:
    explicit ShowMsg(QObject* parent = nullptr);

public slots:
    void recvMsg(QString str);
};

#endif // SHOWMSG_H
