#include "addsource.h"
#include "ui_addsource.h"
#include <QDebug>

addSource::addSource(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addSource)
{
    ui->setupUi(this);
    ui->add_lineEdit->setPlaceholderText("输入网址");
    ui->version_lineEdit->setPlaceholderText("自定义版本");
    ui->suffix_lineEdit->setPlaceholderText("自定义后缀");
    ui->preview_lineEdit->setPlaceholderText("预览");
    ui->preview_lineEdit->setFocusPolicy(Qt::NoFocus);
    ui->preview_lineEdit->setEnabled(false);
    ui->deb->setChecked(true);
    ui->main->setChecked(true);
    typeStr = "deb";
    branchStr = "main";
    connect(ui->add_lineEdit, SIGNAL(textChanged(const QString &)), this,SLOT(on_add_lineEdit_textChanged(const QString &)));
    connect(ui->version_lineEdit, SIGNAL(textChanged(const QString &)), this,SLOT(on_version_lineEdit_textChanged(const QString &)));
    connect(ui->suffix_lineEdit, SIGNAL(textChanged(const QString &)), this,SLOT(on_suffix_lineEdit_textChanged(const QString &)));
    connect(ui->preview_lineEdit, SIGNAL(textChanged(const QString &)), this,SLOT(on_preview_lineEdit_textChanged(const QString &)));

    connect(ui->deb, SIGNAL(stateChanged(int)), this, SLOT(debStateChanged(int)));
    connect(ui->debsrc, SIGNAL(stateChanged(int)), this, SLOT(debSrcStateChanged(int)));

    connect(ui->main, SIGNAL(stateChanged(int)), this, SLOT(branchStrupdate(int)));
    connect(ui->restricted, SIGNAL(stateChanged(int)), this, SLOT(branchStrupdate(int)));
    connect(ui->universe, SIGNAL(stateChanged(int)), this, SLOT(branchStrupdate(int)));
    connect(ui->multiverse, SIGNAL(stateChanged(int)), this, SLOT(branchStrupdate(int)));

    connect(ui->comboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(versionBoxSel(const QString &)));
    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(addBtnClicked()) );
}

addSource::~addSource()
{
    delete ui;
}

void addSource::on_add_lineEdit_textChanged(const QString &arg1)
{
    webStr = arg1;
    setSource();
}

void addSource::on_version_lineEdit_textChanged(const QString &arg1)
{
    versionStr = arg1;
    setSource();
}

void addSource::on_suffix_lineEdit_textChanged(const QString &arg1)
{
    suffixStr = arg1;
    ui->comboBox->setCurrentIndex(1);
    setSource();
}

void addSource::on_preview_lineEdit_textChanged(const QString &arg1)
{

}

void addSource::debStateChanged(int state)
{
    if(state == Qt::Checked){
        typeStr = "deb";
        ui->debsrc->setChecked(false);
        setSource();
    }
}

void addSource::debSrcStateChanged(int state)
{
    if(state == Qt::Checked){
        typeStr = "deb-src";
        ui->deb->setChecked(false);
        setSource();
    }
}

void addSource::branchStrupdate(int state)
{
    qDebug()<<state;
    branchStr = "";
    if(ui->main->isChecked())
    {
        branchStr = branchStr+" main";
    }
    if(ui->restricted->isChecked())
    {
        branchStr = branchStr+" restricted";
    }
    if(ui->universe->isChecked())
    {
        branchStr = branchStr+" universe";
    }
    if(ui->multiverse->isChecked())
    {
        branchStr = branchStr+" multiverse";
    }
    qDebug()<<"branchStr : "<<branchStr;
    setSource();
}

void addSource::versionBoxSel(const QString &text)
{
    versionStr = text;
    qDebug()<<versionStr;
    setSource();
}

QString addSource::setSource()
{
    QString source = typeStr+" "+webStr+" "+versionStr+" "+branchStr+" "+suffixStr;
    ui->preview_lineEdit->setText(source);
    return source;
}

void addSource::addBtnClicked()
{
    this->close();
}
