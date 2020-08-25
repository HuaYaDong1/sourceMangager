#include "softsourcemanager.h"
#include <QDebug>

softSourceManager::softSourceManager()
{

}
void softSourceManager::addSource(QVariantList sourceInfo)
{
    QString sourceFileName = sourceInfo.at(0).toString();
    QString sourceName = sourceInfo.at(1).toString();

    QFile file(sourceFileName);
    file.open(QIODevice::ReadWrite | QIODevice::Append);

    file.write(QString(sourceName + "\n").toLatin1());
    file.close();

}
void softSourceManager::deleteSource(QVariantList sourceInfo)
{
    QString sourceFileName = sourceInfo.at(0).toString();
    QString sourceName = sourceInfo.at(1).toString();

    QFile file(sourceFileName);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QString s;
    QTextStream t(&file);
    QString line;
    while(!t.atEnd())
    {
        line = t.readLine();
        if(line.compare(sourceName) != 0){
            s.append(line + "\n");
        }
    }
    file.resize(0);
    t << s;
    file.close();
}

void softSourceManager::updateSource()
{
    qDebug()<<"开始更新";
    QProcess process;
    process.start("apt-get update");
    process.waitForFinished(-1);
    qDebug()<<"更新完成";

    QDBusMessage msg = QDBusMessage::createSignal("/citos/client/path", "com.client.test","updateOver");
    msg<<"更新成功";
    bool sendResult = QDBusConnection::systemBus().send(msg);
    qDebug()<<sendResult;
}
void softSourceManager::setMainSource(QVariantList sourceFileName)
{
    qDebug()<<"设置开始";
    QString cmd = QString("cp %1 /etc/apt/sources.list").arg(sourceFileName.at(0).toString());
    QProcess process;
    process.start(cmd);
    process.waitForFinished(-1);

    qDebug()<<"设置完成";

    QDBusMessage msg = QDBusMessage::createSignal("/citos/client/path", "com.client.test","updateOver");
    msg<<"主源设置完成";
    bool sendResult = QDBusConnection::systemBus().send(msg);
    qDebug()<<sendResult;
}

//arg 1 src  arg 2  dest arg 3 filename
void softSourceManager::changedSource(QVariantList sourceInfo)
{
    QString srcSourceName = sourceInfo.at(0).toString();
    QString destSourceName = sourceInfo.at(1).toString();
    QString sourceFileName = sourceInfo.at(2).toString();


    QString strAll;
    QStringList strList;
    QFile readFile(sourceFileName);		//PATH是自定义读取文件的地址
    if(readFile.open(QIODevice::ReadOnly))
    {
        //把文件所有信息读出来
        QTextStream stream(&readFile);
        strAll=stream.readAll();
    }
    readFile.close();

    QFile writeFile(sourceFileName);

    if(!writeFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        qDebug()<<"open failed!";
        return ;
    }

    QTextStream stream(&writeFile);
    strList = strAll.split("\n");
    qDebug()<<"strList :"<<strList;
    for(int i=0;i<strList.count();i++){
        if((strList.at(i).compare(srcSourceName)) == 0)    //"123456789"是要修改的内容
        {
            QString tempStr=strList.at(i);
            tempStr.replace(0,tempStr.length(),destSourceName);   //"Hello!"是要替换的内容
            stream<<tempStr<<'\n';

        }
        //如果没有找到要替换的内容，照常写入
        else
        {
            if(i==strList.count()-1)
            {
                stream<<strList.at(i);
            }
            else
            {
                stream<<strList.at(i)<<'\n';
            }
        }
    }
    writeFile.close();
}


//arg1 srclist arg2 filename
void softSourceManager::addExtensionSource(QVariantList sourceInfo)
{
    QStringList sourceNameList = sourceInfo.at(0).toStringList();
    QString sourceFileName = sourceInfo.at(1).toString();

    QFile writeFile(sourceFileName);

    if(!writeFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<<"open file failed!";
        return ;
    }
    QTextStream stream(&writeFile);
    for(int i = 0;i<sourceNameList.count();i++){
        QString tempStr=sourceNameList.at(i);
        stream<<tempStr<<'\n';
    }
    writeFile.close();
}

