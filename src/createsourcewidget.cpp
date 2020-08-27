#include "createsourcewidget.h"
#include "ui_createsourcewidget.h"
#include <QDebug>

createSourceWidget::createSourceWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createSourceWidget)
{
    ui->setupUi(this);
    connect(ui->trueBtn, SIGNAL(clicked()), this, SLOT(trueBtnClicked()));
    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(cancelBtnClicked()));
    connect(ui->nameLineEdit, SIGNAL(textChanged(const QString &)), this,SLOT(nameLineEdit_textChanged(const QString &)));
    isTrueBtnClicked = false;
}

createSourceWidget::~createSourceWidget()
{
    delete ui;
}

void createSourceWidget::nameLineEdit_textChanged(const QString &arg1)
{
    qDebug()<<arg1;
    ui->nameLineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
}

void createSourceWidget::trueBtnClicked()
{
    if(ui->nameLineEdit->text().isEmpty())
    {
         ui->nameLineEdit->setStyleSheet("QLineEdit{border:3px solid red }");
          ui->nameLineEdit->setPlaceholderText("输入不能为空");
         return;
    }
    if(!ui->nameLineEdit->text().contains(".",Qt::CaseSensitive))
    {
        ui->nameLineEdit->setStyleSheet("QLineEdit{border:3px solid red }");
        ui->nameLineEdit->setPlaceholderText("输入格式为xxxxx.list");
        return;
    }
    QStringList name = ui->nameLineEdit->text().split(".");
    QString list = name.at(1);
    if(!list.compare("list") == 0)
    {
        ui->nameLineEdit->setStyleSheet("QLineEdit{border:3px solid red }");
        ui->nameLineEdit->setPlaceholderText("后缀为.list");
        return;
    }

    isTrueBtnClicked = true;
    this->close();
}

void createSourceWidget::cancelBtnClicked()
{
    this->close();
}
