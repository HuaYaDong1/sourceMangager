#include "selectform.h"
#include "ui_selectform.h"

selectForm::selectForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectForm)
{
    ui->setupUi(this);
}

selectForm::~selectForm()
{
    delete ui;
}


void selectForm::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QStyleOption opt;
    opt.init(this);
    painter.setBrush(opt.palette.color(QPalette::Base));

    if(QColor(255,255,255) == opt.palette.color(QPalette::Base) || QColor(248,248,248) == opt.palette.color(QPalette::Base))
    {
        painter.setBrush(QColor(207,207,207));
    }else{
        painter.setBrush(QColor(59,59,61));
    }

    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setWidth(rect.width() - 0);
    rect.setHeight(rect.height() - 0);
    painter.drawRoundedRect(rect, 0, 0);
    {
        QPainterPath painterPath;
        painterPath.addRoundedRect(rect, 0, 0);
        painter.drawPath(painterPath);
    }
    QWidget::paintEvent(event);
}
