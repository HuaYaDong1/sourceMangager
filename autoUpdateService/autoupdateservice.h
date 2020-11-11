#ifndef AUTOUPDATESERVICE_H
#define AUTOUPDATESERVICE_H

#include <QObject>

#include <QApt/Backend>
#include <QApt/Transaction>
#include <QApt/Package>

#include <QTime>!-----------------------\
#include <QDebug>




class autoUpdateService : public QObject
{
    Q_OBJECT
public:
    explicit autoUpdateService(QObject *parent = nullptr);
    QApt::Backend *m_backend;
    QApt::Transaction *m_trans;


private:
    void updateSource();


signals:

};

#endif // AUTOUPDATESERVICE_H
