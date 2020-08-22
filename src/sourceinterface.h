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

class sourceInterface : public QObject
{
    Q_OBJECT
public:
    sourceInterface();
    ~sourceInterface();


    QStringList getSourceName();
    QStringList getSourceAddressList(QString fileName);
    QString  setPingToWidget(QString sourceName);
    QStringList getSourceTypeList(QString fileName);
    QString getDownloadSpeedFromSource();


    QTimer *timer;
    QTime time;
    QTime qtime;
    QNetworkAccessManager *manager;
    QNetworkReply *downreply ;

    int allsize = 0;
    int alltime = 0;
    int timenum=0;
    bool timeout = false;
    double speed;
    QString speedstr;


public slots:
    void update();
    void stopdownload();
    void downloadFinish(QNetworkReply *reply);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

};

#endif // SOURCEINTERFACE_H
