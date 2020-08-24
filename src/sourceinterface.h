#ifndef SOURCEINTERFACE_H
#define SOURCEINTERFACE_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QHostInfo>
#include <QProcess>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <stdio.h>
#include <unistd.h>
#include <QTextCodec>
#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidget>

class sourceInterface : public QObject
{
    Q_OBJECT
public:
    sourceInterface();
    ~sourceInterface();


    QStringList getSourceName();
    QStringList getMainSourceName();
    QStringList getSourceAddressList(QString fileName);
    QString  setPingToWidget(QString sourceName);
    QStringList getSourceTypeList(QString fileName);
    void getDownloadSpeedFromSource(QString sourceName, QListWidget *listwidget, int num);
    void getDownloadSpeedFromSource1(QString sourceName);


    QTimer *timer;
    QNetworkAccessManager *manager;
    QNetworkReply *downreply ;

    int allsize = 0;
    int alltime = 0;
    QTime downloadTime;
    bool timeout = false;
    bool isStart = false;
    double speed;
    QString speedstr;
    QListWidget *Listwidget;
    int Num;
    QString SourceName;


public slots:
    void update();
    void stopdownload();
    void downloadFinish(QNetworkReply *reply);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

signals:
    void downloadover(QString , QListWidget * , int );
    void downloadspeed(QString , QListWidget * , int);

};

#endif // SOURCEINTERFACE_H
