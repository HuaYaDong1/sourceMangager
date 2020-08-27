#include "addsource.h"
#include "ui_addsource.h"
#include <QDebug>

addSource::addSource(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addSource)
{
    ui->setupUi(this);
    ui->add_lineEdit->setPlaceholderText("输入服务器地址");
    ui->version_lineEdit->setPlaceholderText("自定义版本");
    ui->suffix_lineEdit->setPlaceholderText("自定义分类目录");
    ui->preview_lineEdit->setPlaceholderText("预览");
    ui->preview_lineEdit->setFocusPolicy(Qt::NoFocus);
    ui->preview_lineEdit->setEnabled(false);
    ui->deb->setChecked(true);
    ui->main->setChecked(true);
    typeStr = "deb";
    branchStr = "main";
    isAddBtnClicked = false;
    versionStr = "v10";
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
    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(cancelBtnClicked()) );

    ui->version_lineEdit->setFocusPolicy(Qt::NoFocus);
}

addSource::~addSource()
{
    delete ui;
}

void addSource::on_add_lineEdit_textChanged(const QString &arg1)
{
    ui->add_lineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");

    webStr = arg1;
    setSource();
}

void addSource::on_version_lineEdit_textChanged(const QString &arg1)
{
    ui->version_lineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
    versionStr = arg1;
    setSource();
}

void addSource::on_suffix_lineEdit_textChanged(const QString &arg1)
{
    ui->suffix_lineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
    suffixStr = arg1;
    ui->comboBox->setCurrentIndex(1);
    setSource();
}

void addSource::on_preview_lineEdit_textChanged(const QString &arg1)
{
    qDebug()<<arg1;
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
    ui->suffix_lineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");

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
    ui->version_lineEdit->setStyleSheet("QLineEdit{border:1px solid gray border-radius:1px}");
    if(text.compare("手动编辑") == 0){
         ui->version_lineEdit->setFocusPolicy(Qt::StrongFocus);
         versionStr = "";
    }else{
         ui->version_lineEdit->setFocusPolicy(Qt::NoFocus);
         versionStr = text;
    }
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
    if(ui->add_lineEdit->text().isEmpty()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("warning"),  tr("地址不能为空"), QMessageBox::Yes);
        if(reply == QMessageBox::Yes)
        {
            ui->add_lineEdit->setStyleSheet("QLineEdit{border:3px solid red }");
            return ;
        }
    }

    QRegExp rx("(https?|ftp|file)://[-A-Za-z0-9+&@#/%?=~_|!:,.;]+[-A-Za-z0-9+&@#/%=~_|]");//正则表达式判断网址格式是否正确
    int pos=0;
    QRegExpValidator v(rx, 0);
    QString http = ui->add_lineEdit->text();
    if (2==v.validate(http,pos))
    {
    }else{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("warning"),  tr("输入格式不正确"), QMessageBox::Yes);
        if(reply == QMessageBox::Yes)
        {
            ui->add_lineEdit->setStyleSheet("QLineEdit{border:3px solid red }");
            return ;
        }
    }

    if(branchStr.compare("")==0 && suffixStr.compare("")==0){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("warning"),  tr("分类目录不能为空"), QMessageBox::Yes);
        if(reply == QMessageBox::Yes)
        {
            ui->suffix_lineEdit->setStyleSheet("QLineEdit{border:3px solid red }");
            return ;
        }
    }

    if(ui->version_lineEdit->text().isEmpty() && ui->comboBox->currentText().compare("手动编辑")==0){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("warning"),  tr("版本不能为空"), QMessageBox::Yes);
        if(reply == QMessageBox::Yes)
        {
            ui->version_lineEdit->setStyleSheet("QLineEdit{border:3px solid red }");
            return ;
        }
    }

    isAddBtnClicked = true;
    this->close();
}

void addSource::cancelBtnClicked()
{
    this->close();
}
