#include "seleteframe.h"
#include "ui_seleteframe.h"

seleteFrame::seleteFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::seleteFrame)
{
    ui->setupUi(this);
}

seleteFrame::~seleteFrame()
{
    delete ui;
}

void seleteFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QStyleOption opt;
    opt.init(this);
    painter.setBrush(opt.palette.color(QPalette::Base));

    if(QColor(255,255,255) == opt.palette.color(QPalette::Base) || QColor(248,248,248) == opt.palette.color(QPalette::Base))
    {
        painter.setBrush(QColor(255,255,255));
    }else{
        painter.setBrush(QColor(59,59,61));
    }

    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setWidth(rect.width() - 0);
    rect.setHeight(rect.height() - 0);
    painter.drawRoundedRect(rect, 4, 4);
    {
        QPainterPath painterPath;
        painterPath.addRoundedRect(rect, 4, 4);
        painter.drawPath(painterPath);
    }
    QWidget::paintEvent(event);
}
