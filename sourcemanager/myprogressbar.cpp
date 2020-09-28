#include "myprogressbar.h"

myProgressbar::myProgressbar(QWidget *parent) : QWidget(parent)
{

}

void myProgressbar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QStyleOptionComplex option;

    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter.save();

    painter.setPen(Qt::NoPen);
    painter.setBrush(option.palette.button().color());
    painter.drawRoundedRect(0, 0, 100, 30, 4, 4);

    double value =  progressValue * 1;

    painter.setPen(Qt::NoPen);
    painter.setBrush(option.palette.highlight().color());
    painter.drawRoundedRect(0, 0, value, 30, 4, 4);
    painter.restore();
}
