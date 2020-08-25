#include <QCoreApplication>
#include "softsourcemanager.h"
#include "softSourceManagerAdaptor.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //建立到session bus的连接
    QDBusConnection connection = QDBusConnection::systemBus();
    //在session bus上注册服务
    if(!connection.registerService("com.softSource.manager"))
    {
        qDebug() << "error:" << connection.lastError().message();
        exit(-1);
    }
    softSourceManager *sourceManager = new softSourceManager();
    new InterfaceAdaptor(sourceManager);

    //注册名为/test/objects的对象，把类Object所有槽函数导出为object的method
    connection.registerObject("/com/softSource/Manager", sourceManager, QDBusConnection :: ExportAllSlots |QDBusConnection :: ExportAllSignals);

    return a.exec();
}
