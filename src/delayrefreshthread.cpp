#include "delayrefreshthread.h"
#include "sourceinterface.h"


delayRefreshThread::delayRefreshThread(QLabel *spend_Label, QLabel *delay_Label, QString address)
{
    Spend_Label = spend_Label;
    Delay_Label = delay_Label;
    Address = address;
}

void delayRefreshThread::run()
{
    Delay_Label->setText(sourceinterface->setPingToWidget(Address));
    //Spend_Label->setText(sourceinterface->getDownloadSpeedFromSource());
    return;
}
