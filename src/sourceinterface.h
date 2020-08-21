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
    int getPacketsFromNetwork();
    void deleteSource(QString sourceName,QString sourceType,QString sourceFileName);
    void addSource(QString sourceName,QString sourceType,QString sourceFileName);





};

#endif // SOURCEINTERFACE_H
