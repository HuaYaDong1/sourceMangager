#include "autoupdateservice.h"
#include <syslog.h>

autoUpdateService::autoUpdateService(QObject *parent) : QObject(parent)
{
    updateSource();
    syslog(LOG_ERR,"This is syslog err!-----------------------\n");

    exit(0);
}

void autoUpdateService::updateSource()
{
    m_backend = new QApt::Backend(this);
    m_backend->init();

    m_trans = m_backend->updateCache();
    m_trans->run();
}
