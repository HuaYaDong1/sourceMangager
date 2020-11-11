#include "systemdtimermanager.h"
#include <syslog.h>
#include <QDebug>
#include <QDir>
systemdtimermanager::systemdtimermanager(QObject *parent) : QObject(parent)
{
    QFileSystemWatcher *fileWatch = new QFileSystemWatcher();
    fileWatch->addPath(CONFIG_FILE_PATH);
    connect(fileWatch, SIGNAL(fileChanged(QString)),this,SLOT(fileUpdated(QString)));
    if(!timerIsUpdate()){
        updateTimerFile();
    }
    qDebug()<<"1111";
}
bool systemdtimermanager::timerIsUpdate()
{
    QFileInfo fileinfo("/usr/lib/systemd/system/updateTime.timer");
    if(fileinfo.isFile())
    {
        return true;
    }
    else{
        return false;
    }
}
void systemdtimermanager::fileUpdated(QString path)
{
    Q_UNUSED(path);
    syslog(LOG_ERR,"------file changed ---------\n");
    updateTimerFile();
}
void systemdtimermanager::updateTimerFile()
{
    syslog(LOG_ERR,"-----updateTimerFile\n");
    QString filename;
    QStringList fileNameList;
    for(int i =0;i<10000;i++){
        filename.clear();
        filename = "updateTime";
        if(i != 0){
            filename.append(QString::number(i));
        }
        bool ret = parseConfig(filename);
        if(!ret)
            break;
        fileNameList<<filename;
    }
    qDebug()<<fileNameList;
    QProcess process;
    process.start("systemctl daemon-reload");
    process.waitForFinished(-1);

    QString shell;
    shell.append("systemctl  start");
    foreach (QString fileName, fileNameList) {
        shell.append(" ");
        shell.append(fileName);
        shell.append(".timer");
    }
    qDebug()<<shell;
    process.start(shell);
    process.waitForFinished(-1);

    QString restartShell;
    restartShell.append("systemctl restart autoUpdate.service");
    process.start(restartShell);
    process.waitForFinished(-1);


}
bool systemdtimermanager::parseConfig(QString filename)
{
    QString fileName = filename+".timer";
    int intConfHour;
    int intConfMin;
    int intConfSec;
    //计算之前的总秒数
    int offsetBeforeSec = 0 ;
    //随机出来的最后的时间的总秒数
    int allSec;
    QString allStr;

    GError **error = NULL;
    GKeyFileFlags flags = G_KEY_FILE_NONE;
    GKeyFile *keyfile = g_key_file_new();
    if (access(CONFIG_FILE_PATH, F_OK) == -1)
    {
        fprintf(stderr, "%s is not exist!\n", CONFIG_FILE_PATH);
    }
    g_key_file_load_from_file(keyfile, CONFIG_FILE_PATH, flags, error);
    char * fileNameStr;
    QByteArray tmp = filename.toLatin1();
    fileNameStr = tmp.data();
    gboolean has_key = g_key_file_has_key(keyfile, fileNameStr, "Time", error);
    if (!has_key)
    {
        return false;
    }
    char *ret = g_key_file_get_locale_string(keyfile, fileNameStr, "Time", NULL, error);
    char *offsettime = g_key_file_get_locale_string(keyfile, fileNameStr, "OffsetTime", NULL, error);
    g_key_file_free(keyfile);

    QString weeks;
    QString strTime = QString(QLatin1String(ret));
    if(strTime.contains(" ")){
        QStringList list =  strTime.split(" ");
        QString str1;
        QString str2;
        if(list.size() == 3){
            weeks = list.at(0);
            allStr.append(weeks);
            allStr.append(" ");
            str1 =list.at(1);
            str2 =list.at(2);
        }
        else if(list.size() == 2)
        {
            str1 =list.at(0);
            str2 =list.at(1);
        }
        if(str1.contains("-")){
            allStr.append(str1);
        }
        if(str2.contains(":")){
            QStringList list3 = str2.split(":");
            QString hourTime = list3.at(0);
            QString minTime = list3.at(1);
            QString secTime = list3.at(2);
            intConfHour = hourTime.toInt();
            intConfMin = minTime.toInt();
            intConfSec = secTime.toInt();
            qDebug()<<intConfHour<<intConfMin<<intConfSec<<weeks<<"==";
            offsetBeforeSec = intConfHour*3600 + intConfMin *60 + intConfSec;
        }
    }
    QString strOffsetTime = QString(QLatin1String(offsettime));
    if(strOffsetTime.contains(":")){
        QStringList listOffsetTime = strOffsetTime.split(":");
        QString hourOffset = listOffsetTime.at(0);
        QString minOffset = listOffsetTime.at(1);

        int secOffset =  (hourOffset.toInt()*3600 + minOffset.toInt()*60) ;

        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
        int randSec = qrand()%(secOffset*2);
        qDebug()<<"randSec:"<<randSec<<"secOffset:"<<secOffset;
        //设定时间后offset
        if(randSec > secOffset){
            allSec = offsetBeforeSec + (randSec-secOffset);
        }
        //设定时间前offset
        else{
            allSec = offsetBeforeSec - randSec;
        }
        qDebug()<<"allSec:"<<allSec;
        if(allSec >=3600){
            int Hour = allSec / 3600;
            int Min = (allSec - (3600*Hour)) /60;
            int Sec = (allSec - (3600*Hour)) %60;
            if(Hour>=24){
                Hour = Hour-24;
            }
            qDebug()<< Hour<<Min<<Sec;
            allStr.append(" ");
            if(Hour < 10){
                allStr.append("0");
                allStr.append(QString::number(Hour));
            }
            else{
                allStr.append(QString::number(Hour));
            }
            allStr.append(":");
            if(Min < 10){
                allStr.append("0");
                allStr.append(QString::number(Min));
            }
            else{
                allStr.append(QString::number(Min));
            }
            allStr.append(":");
            if(Sec < 10){
                allStr.append("0");
                allStr.append(QString::number(Sec));
            }
            else{
                allStr.append(QString::number(Sec));
            }
        }
        else{
            int Min = allSec  /60;
            int Sec = allSec  %60;
            qDebug()<<Min<<Sec;
            allStr.append(" ");
            allStr.append("00");
            allStr.append(":");
            allStr.append(QString::number(Min));
        }
        qDebug()<<"ALLstr: "<<allStr;
        setTimerFile(fileName,allStr);
    }
    return true;
}
void systemdtimermanager::setTimerFile(QString fileName, QString timerStr)
{
    QString fileNamePath;
    fileNamePath.append(SYSTEMD_PATH);
    fileNamePath.append(fileName);
    qDebug()<<fileNamePath;
    QFile file(fileNamePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<<"open failed!";
    }
    QTextStream stream(&file);
    stream<<"[Unit]\n";
    stream<<"Description=System Service: auto update soft source\n";
    stream<<"[Timer]\n";
    stream<<"OnCalendar="<<timerStr<<"\n";
    stream<<"Unit=autoUpdate.service\n";
    stream<<"[Install]\n";
    stream<<"WantedBy=timers.target\n";
    file.close();
}
