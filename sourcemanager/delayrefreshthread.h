#ifndef DELAYREFRESHTHREAD_H
#define DELAYREFRESHTHREAD_H

#include <QThread>
#include <QLabel>

class sourceInterface;
class delayRefreshThread : public QThread
{
public:
    delayRefreshThread(QLabel *spend_Label, QLabel *delay_Label, QString address);

private:

    virtual void run();//任务处理线程

    QLabel *Spend_Label;
    QLabel *Delay_Label;
    QString Address;

public:
    int workCount;//计数
    sourceInterface  *sourceinterface;

signals:

public slots:
};

#endif // DELAYREFRESHTHREAD_H
