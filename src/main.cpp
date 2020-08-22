#include "sourcemanager.h"
#include "addsource.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sourceManager w;
//    addSource b;
    w.show();
//    b.show();
    return a.exec();
}
