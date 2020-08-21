#include "sourceinformationwidget.h"
#include "ui_sourceinformationwidget.h"

sourceInformationWidget::sourceInformationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sourceInformationWidget)
{
    ui->setupUi(this);
}

sourceInformationWidget::~sourceInformationWidget()
{
    delete ui;
}
