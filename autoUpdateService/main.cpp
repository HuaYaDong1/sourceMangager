#include <QCoreApplication>
#include "autoupdateservice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    autoUpdateService Service;

    return a.exec();
}
