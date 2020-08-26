#include "createsourcewidget.h"
#include "ui_createsourcewidget.h"

createSourceWidget::createSourceWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createSourceWidget)
{
    ui->setupUi(this);
    connect(ui->trueBtn, SIGNAL(clicked()), this, SLOT(trueBtnClicked()));
    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(cancelBtnClicked()));
    isTrueBtnClicked = false;
}

createSourceWidget::~createSourceWidget()
{
    delete ui;
}

void createSourceWidget::trueBtnClicked()
{
    isTrueBtnClicked = true;
    this->close();
}

void createSourceWidget::cancelBtnClicked()
{
    this->close();
}
