#ifndef DOWNLOADSPENDREFRESHTHREAD_H
#define DOWNLOADSPENDREFRESHTHREAD_H

#include <QThread>
#include <QLabel>
#include <QListWidget>
#include <QObject>
#include <QMessageBox>

class sourceInterface;
class testingUpdateThread : public QThread
{
public:
    testingUpdateThread(QListWidget *listWidget);

private:

    virtual void run();//任务处理线程
    QListWidget *ListWidget;

public:
    sourceInterface  *sourceinterface;


signals:

public slots:
};

#endif // DOWNLOADSPENDREFRESHTHREAD_H
