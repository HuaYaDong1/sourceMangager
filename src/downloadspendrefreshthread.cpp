#include "downloadspendrefreshthread.h"
#include "sourceinformationwidget.h"
#include "ui_sourceinformationwidget.h"
#include "sourceinterface.h"

downloadSpendRefreshThread::downloadSpendRefreshThread(QListWidget *listWidget, int num)
{
    sourceinterface = new sourceInterface;
    ListWidget = listWidget;
    Num = num;
    connect(sourceinterface,SIGNAL(downloadover(QString)),this,SLOT(downloadOverSlot(QString)));
}

void downloadSpendRefreshThread::run()
{
    for(int i = 1; i < Num+1; i++){
        QListWidgetItem *item = ListWidget->item(i);
        sourceInformationWidget* pwig = static_cast<sourceInformationWidget*> (ListWidget->itemWidget(item));
        sourceinterface->getDownloadSpeedFromSource(pwig->ui->address_Label->text());
        //pwig->ui->spend_Label->setText(spend);
    }
    return;
}

void downloadSpendRefreshThread::downloadOverSlot(QString speed)
{
    qDebug()<<"------speed"<<speed;
}
