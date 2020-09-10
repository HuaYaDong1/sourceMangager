#ifndef APT_PKG_H
#define APT_PKG_H

#include <QObject>
#include <QDebug>

#include <apt-pkg/cachefile.h>
#include <apt-pkg/cachefile.h>
#include <apt-pkg/depcache.h>
#include <apt-pkg/sourcelist.h>
#include <apt-pkg/pkgsystem.h>
#include <apt-pkg/policy.h>

class  apt_pkg : public QObject
{
public:
    apt_pkg();
    pkgCacheFile cache;
    std::vector<metaIndex*> VolatileFiles;
};

#endif // APT_PKG_H
