#ifndef SYSTEMDTIMERMANAGER_H
#define SYSTEMDTIMERMANAGER_H

#include <QObject>

#include <glib.h>
#include <QFile>
#include <QFileSystemWatcher>
#include <QTextStream>
#include <unistd.h>
#include <QTime>
#include <QDebug>
#include <QProcess>


#define CONFIG_FILE_PATH "/var/lib/sourcemanager/updateTime.conf"
#define SYSTEMD_PATH "/usr/lib/systemd/system/"

class systemdtimermanager : public QObject
{
    Q_OBJECT
public:
    explicit systemdtimermanager(QObject *parent = nullptr);

signals:

private:
    bool parseConfig(QString filename);
    void setTimerFile(QString fileName,QString timerStr);
    void updateTimerFile();

private slots:
    void fileUpdated(QString path);

};

#endif // SYSTEMDTIMERMANAGER_H
