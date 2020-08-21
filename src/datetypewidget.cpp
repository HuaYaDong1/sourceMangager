#include "datetypewidget.h"
#include "ui_datetypewidget.h"

dateTypeWidget::dateTypeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dateTypeWidget)
{
    ui->setupUi(this);
}

dateTypeWidget::~dateTypeWidget()
{
    delete ui;
}
