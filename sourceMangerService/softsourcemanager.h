#ifndef SOFTSOURCEMANAGER_H
#define SOFTSOURCEMANAGER_H
#include <QObject>
#include <QDBusConnection>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QDBusMessage>

#include <QApt/Globals>
#include <QApt/Backend>
#include <QApt/Transaction>
#include <QApt/DownloadProgress>
#include <QApt/SourcesList>
#include <QApt/Package>
#include <QApt/DependencyInfo>

class softSourceManager :public QObject
{

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.softSource.manager.interface")
public:
    softSourceManager();

    QApt::Backend *m_backend;
    QApt::Transaction *m_trans;

public slots:
    void addSource(QVariantList sourceInfo);
    void deleteSource(QVariantList sourceInfo);
    void setMainSource(QVariantList sourceFileName);
    void updateSource();
    void changedSource(QVariantList sourceInfo);
    void addExtensionSource(QVariantList sourceInfo);
    void deleteSourceFile(QVariantList sourceInfo);

    void addMainSource(QVariantList sourceInfo);
    void delMainSource(QVariantList sourceInfo);

    void progressChanged(int progress);

signals:
    void updateOver(QString arg);
};

#endif // SOFTSOURCEMANAGER_H
