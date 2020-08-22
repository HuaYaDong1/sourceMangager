#ifndef DOWNLOADSPENDREFRESHTHREAD_H
#define DOWNLOADSPENDREFRESHTHREAD_H

#include <QThread>
#include <QLabel>
#include <QListWidget>
#include <QObject>

class sourceInterface;
class downloadSpendRefreshThread : public QThread
{
public:
    downloadSpendRefreshThread(QListWidget *listWidget, int num);

private:

    virtual void run();//任务处理线程
    QListWidget *ListWidget;
    int Num;

public:
    int workCount;//计数
    sourceInterface  *sourceinterface;
    void downloadOverSlot(QString speed);


signals:

public slots:
    void downloadover(QString );
};

#endif // DOWNLOADSPENDREFRESHTHREAD_H
