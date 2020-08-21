#include "sourceinterface.h"

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
        typeList<<line;
    }
    while(!line.isNull())//字符串有内容
    {
        line=in.readLine();//循环读取下行
        if((line.left(1).compare("#") !=0) && (line.compare("") !=0)){
            if(line.contains(" "))
            {
                QStringList list = line.split(" ");
                typeList << list.at(0);
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
QString sourceInterface::getDownloadSpeedFromSource()
{
    int beforeNum = getPacketsFromNetwork();
    sleep(1);
    int afterNum = getPacketsFromNetwork();

    qDebug()<<"-----------------------------------------";
    int networkSpeed = afterNum - beforeNum;

    QString str = QString("%1 kb/s").arg(networkSpeed);

    qDebug()<<str;
    qDebug()<<"----------ssss-------------------------------";
    return str;
}

int  sourceInterface::getPacketsFromNetwork()
{
    int packetnum;
    QList<QNetworkInterface>list=QNetworkInterface::allInterfaces();//获取所有网络接口信息

    foreach(QNetworkInterface interface,list)
    {
        if(interface.name() == "lo"){
            continue;
        }
        FILE *fp;
        char cmd[1024];
        char pRetMsg[1024];
        QByteArray tmp = interface.name().toLatin1();
        char *name = tmp.data();
        sprintf(cmd,"ifconfig %s | grep 'bytes' | awk '{print $3}'",name);

        if ((fp = popen(cmd, "r") ) == NULL)
        {
            printf("Popen Error!\n");
            return -2;
        }
        else
        {
            memset(pRetMsg, 0, 1024);
            //get lastest result
            fgets(pRetMsg, 1024, fp);
            fgets(pRetMsg, 1024, fp);
            packetnum = atoi(pRetMsg);
            if(packetnum == 0){
                continue;
            }
            if ( (pclose(fp)) == -1)
            {
                printf("close popenerror!\n");
                return -3;
            }

            qDebug()<<packetnum;
            return packetnum;
        }
    }
    return packetnum;
}


void sourceInterface::deleteSource(QString sourceName, QString sourceFileName)
{
    //root
    qDebug()<<sourceName<<"    "<<sourceFileName;
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


void sourceInterface::addSource(QString sourceName, QString sourceFileName)
{
    //root

    QFile file(sourceFileName);
    file.open(QIODevice::ReadWrite | QIODevice::Append);

    file.write(QString(sourceName + "\n").toLatin1());
    file.close();

}




