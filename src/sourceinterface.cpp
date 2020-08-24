#include "sourceinterface.h"
#include <stdlib.h>
#include <sys/wait.h>


sourceInterface::sourceInterface()
{



}
sourceInterface::~sourceInterface()
{

}
QStringList sourceInterface::getSourceName()
{
    QStringList sourceNameList;
    QDir sourceDir = QDir("/etc/apt");
    if(!sourceDir.exists()){
        qDebug()<<"source dir not exist!";
        return sourceNameList;
    }
    QStringList fileList =  sourceDir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
    int i = 0;
    for(i=0;i<fileList.size();i++){
        if((QString("sources.list.d").compare(fileList.at(i)) == 0)){
            QDir sourcedDir("/etc/apt/sources.list.d");
            QStringList filedList =  sourcedDir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
            int j;
            for(j=0; j<filedList.size() ;j++){
                QString str = filedList.at(j);
                QString liststr = str.right(5);
                if(liststr.compare(".list") ==0 ){
                    sourceNameList << filedList.at(j);
                }
            }
        }
    }
    return sourceNameList;
}

QStringList sourceInterface::getSourceAddressList(QString fileName)
{

    QStringList sourceList;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<fileName<<"----file open failed!";
        return sourceList;
    }
    QTextStream in(&file);
    QString line = in.readLine();
    if((line.left(1).compare("#") !=0) && (line.compare("") !=0)){
        sourceList<<line;
    }
    while(!line.isNull())//字符串有内容
    {
        line=in.readLine();//循环读取下行
        if((line.left(1).compare("#") !=0) && (line.compare("") !=0)){
            if(line.contains(" "))
            {
                //                QStringList list = line.split(" ");
                sourceList << line;
            }
        }
    }
    file.close();
    return sourceList;
}
QStringList sourceInterface::getSourceTypeList(QString fileName)
{
    QStringList typeList;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<fileName<<"----file open failed!";
        return typeList;
    }
    QTextStream in(&file);
    QString line = in.readLine();
    if((line.left(1).compare("#") !=0) && (line.compare("") !=0)){
        if(line.contains(" "))
        {
            QStringList list = line.split(" ");
            qDebug()<<list<<"----";
            typeList << list.at(0);
        }
    }
    while(!line.isNull())//字符串有内容
    {
        line=in.readLine();//循环读取下行
        if((line.left(1).compare("#") !=0) && (line.compare("") !=0)){
            if(line.contains(" "))
            {
                QStringList list2 = line.split(" ");
                typeList << list2.at(0);
            }
        }
    }
    file.close();
    return typeList;
}
QString sourceInterface::setPingToWidget(QString sourceName)
{
    qDebug()<<"before";
    QString ret;
    QStringList list;
    QString strTmp;
    QString urlStr;
    QString url;
    if(sourceName.contains("http://"))
    {
        list = sourceName.split("http://");
        strTmp = list.at(1);
        if(strTmp.contains("/")){
            urlStr = strTmp.split("/").at(0);
        }
    }
    qDebug()<<"----------"<<urlStr;
    if(urlStr.contains(":")){
        url=urlStr.split(":").at(0);
    }
    else{
        url = urlStr;
    }
    QHostInfo info = QHostInfo::fromName(url);


    QProcess ping;
    QString str = QString("ping -c 1 -w 1 ")+url;
    qDebug()<<str;

    QTextCodec *codec = QTextCodec::codecForName("GBK");
    ping.start(str);//执行ping


    ping.waitForFinished(-1);
    QString pingStr =  ping.readAll();
    if(pingStr.contains("time=")){
        QString str = pingStr.split("time=").at(1);
        if(str.contains("\n")){
            QStringList list2 = str.split("ms");
            ret = list2.at(0) + "ms";
            //            qDebug()<<str.remove("\n")<<"======";
        }
    }
    else{
        ret.append("N/A");
    }

    qDebug()<<"ret="<<ret;
    return ret;
}
void sourceInterface::getDownloadSpeedFromSource1(QString sourceName)
{
    pid_t pid;
    if ((pid=fork()) < 0)
        return ;
    else if (pid > 0)
    {
        getDownloadSpeedFromSource1(sourceName);
    }
    else{
    qDebug()<<"sssssssssssssss";
    }
}

void sourceInterface::getDownloadSpeedFromSource(QString sourceName, QListWidget *listwidget, int num)
{
    Listwidget = listwidget;
    Num = num;
    SourceName = sourceName;
    qDebug()<<"download start" <<sourceName;

    QStringList list = sourceName.split(" ");
    QString versionDir;
    QString dir;
    QString httpStr;
    QString archStr;


    //get arch
    QProcess process;
    process.start("dpkg --print-architecture");
    process.waitForFinished(-1);
    QString str =  process.readLine();
    archStr=str.remove("\n");


    int i = 0;
    for(i=0;i<list.size();i++){
        QString str = list.at(i);
        if(str.contains("http:")){
            if(i < list.size() -2){
                httpStr = list.at(i);
                versionDir = list.at(i+1);
                dir = list.at(i+2);
            }
        }
    }
    if(httpStr.right(1).compare("/") != 0){
        httpStr.append("/");
    }
    if(dir.compare("multiverse") == 0 || dir.compare("universe") == 0 || dir.compare("restricted") == 0)
    {
        dir.clear();
        dir = "main";
    }

    QString url = QString("%1dists/%2/%3/binary-%4/Packages.gz").arg(httpStr).arg(versionDir).arg(dir).arg(archStr);
    qDebug()<<"url :"<<url;

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    manager = new QNetworkAccessManager();
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(downloadFinish(QNetworkReply*)));
    QNetworkRequest request(url);
    qDebug()<<"download start" <<sourceName<<"-------------------";
    downreply = manager->get(request);
    connect(downreply, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));
    timer->setInterval(1);
    timenum = 1 ;

    qDebug()<<"speedstr : "<<speedstr;

    return ;
}
void sourceInterface::stopdownload()
{
    disconnect(downreply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinish(QNetworkReply*)));
    downreply->abort();
    downreply->deleteLater();
    downreply = NULL;

}

void sourceInterface::update()
{
    timenum++;
    if(alltime > 5000){
        qDebug()<<"all size: "<<allsize<<"  all time :"<<alltime;
        timer->stop();
        stopdownload();

        speed = allsize * 1000.0 / alltime;
        QString unit;
        if (speed < 1024) {
            unit = "bytes/sec";
        } else if (speed < 1024*1024) {
            speed /= 1024;
            unit = "kB/s";
        } else {
            speed /= 1024*1024;
            unit = "MB/s";
        }
        speedstr = QString(QString::number(speed, 10,1) +unit);
        qDebug()<<"this is speed :"<<speedstr;
        emit(downloadover(speedstr,Listwidget,Num));
    }
    if(alltime == 0 && timenum == 1000){
        qDebug()<<"not download!";
        timer->stop();
        stopdownload();
        speedstr = "0 kb/s";
        emit(downloadover(speedstr,Listwidget,Num));
    }
}

void sourceInterface::downloadFinish(QNetworkReply *reply)
{
    //    qDebug()<<"file size :"<<reply->readAll().size();
    timer->stop();
    qDebug()<<"all size: "<<allsize<<"  all time :"<<alltime;
    double speed = allsize * 1000.0 / alltime;

    QString unit;
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }
    speedstr = QString(QString::number(speed, 10,1) +unit);
    qDebug()<<SourceName<<" ====speedstr "<<speedstr;
    emit(downloadover(speedstr,Listwidget,Num));

    qDebug() << "Download Speed: " << speed << " " << unit;

}
void sourceInterface::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    timer->start();
    allsize = bytesReceived;
    alltime = timenum;

    qDebug()<<"all size :"<<allsize;
}
