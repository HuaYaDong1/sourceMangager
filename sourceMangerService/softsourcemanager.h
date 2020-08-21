#ifndef SOFTSOURCEMANAGER_H
#define SOFTSOURCEMANAGER_H
#include <QObject>
#include <QDBusConnection>
#include <QFile>
#include <QTextStream>

class softSourceManager :public QObject
{

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.softSource.manager.interface")
public:
    softSourceManager();

public slots:
    void addSource(QVariantList sourceInfo);
    void deleteSource(QVariantList sourceInfo);


};

#endif // SOFTSOURCEMANAGER_H