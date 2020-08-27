#include "testingupdatethread.h"
#include "sourceinformationwidget.h"
#include "ui_sourceinformationwidget.h"
#include "sourceinterface.h"

testingUpdateThread::testingUpdateThread(QListWidget *listWidget)
{
    sourceinterface = new sourceInterface;
    ListWidget = listWidget;
}

void testingUpdateThread::run()
{
    QString name = ListWidget->objectName();
    QStringList sourcelist1 = sourceinterface->getSourceAddressList("/etc/apt/sources.list");
    QStringList sourcelist2 = sourceinterface->getSourceAddressList("/etc/apt/mainsources/"+name);
    if(sourcelist1.count() != sourcelist2.count())
    {
//        QMessageBox::StandardButton reply;
//        reply = QMessageBox::warning(this, tr("warning"),  tr("需要更新"), QMessageBox::Yes);
//        if(reply == QMessageBox::Yes)
//        {
//            return ;
//        }
        qDebug() << "~~~~~~~~~~~~~~~~~~~~~数量不同需要更新~~~~~~~~~~~~~~~~~~~~~~~";
        return ;
    }
    else
    {
        for(int i = 0; i<sourcelist1.count(); i++)
        {
            bool flag = false;
            QString source1 = sourcelist1.at(i);
            for(int j = 0; j<sourcelist2.count(); j++)
            {
                QString source2 = sourcelist2.at(j);
                if(source1.compare(source2) == 0)
                {
                    flag = true;
                    break;
                }
            }
            if(!flag){
//                QMessageBox::StandardButton reply;
//                reply = QMessageBox::warning(this, tr("warning"),  tr("需要更新"), QMessageBox::Yes);
//                if(reply == QMessageBox::Yes)
//                {
//                    return ;
//                }
                qDebug() << "~~~~~~~~~~~~~~~~~~~~~内容不同需要更新~~~~~~~~~~~~~~~~~~~~~~~";
                return ;
            }
        }
    }
//    QMessageBox::StandardButton reply;
//    reply = QMessageBox::warning(this, tr("warning"),  tr("不需要更新"), QMessageBox::Yes);
//    if(reply == QMessageBox::Yes)
//    {
//        return ;
//    }
    qDebug() << "~~~~~~~~~~~~~~~~~~~~~完全相同不需要更新~~~~~~~~~~~~~~~~~~~~~~~";
    return ;
}
