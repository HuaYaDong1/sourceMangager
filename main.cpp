#include "sourcemanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sourceManager w;
    w.show();
    return a.exec();
}
