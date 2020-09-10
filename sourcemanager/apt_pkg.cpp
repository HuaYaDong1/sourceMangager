#include "apt_pkg.h"

apt_pkg::apt_pkg()
{
    VolatileFiles = cache.GetSourceList()->SrcList;

    vector<pkgIndexFile*>::iterator i;
    for(i=VolatileFiles.begin();i<VolatileFiles.end();i++)
    {
        qDebug()<<*i;
    }
}
