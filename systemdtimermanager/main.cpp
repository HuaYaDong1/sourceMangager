#include <QCoreApplication>
#include "systemdtimermanager.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    systemdtimermanager manager;

    return a.exec();
}
