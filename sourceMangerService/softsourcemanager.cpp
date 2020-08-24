#include "softsourcemanager.h"

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
    QProcess process;
    process.start("apt-get update");
    process.waitForFinished(-1);
}
void softSourceManager::setMainSource(QVariantList sourceFileName)
{
    QString cmd = QString("cp %1 /etc/apt/sources.list").arg(sourceFileName.at(0).toString());
    QProcess process;
    process.start(cmd);
    process.waitForFinished(-1);
}
