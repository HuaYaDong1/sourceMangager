#include "addsource.h"
#include "ui_addsource.h"
#include <QDebug>

addSource::addSource(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addSource)
{
    ui->setupUi(this);
    ui->add_lineEdit->setPlaceholderText("输入服务器地址");
    ui->version_lineEdit->setPlaceholderText("自定义版本名称");
    ui->suffix_lineEdit->setPlaceholderText("自定义分类目录");
    ui->textEdit->setPlaceholderText("预览");
    ui->textEdit->setFocusPolicy(Qt::NoFocus);
    ui->deb->setChecked(true);
    ui->main->setChecked(true);
    typeStr = "deb";
    branchStr = "main";
    isAddBtnClicked = false;
    versionStr = "v10";

    connect(ui->add_lineEdit, SIGNAL(textChanged(const QString &)), this,SLOT(on_add_lineEdit_textChanged(const QString &)));
    connect(ui->version_lineEdit, SIGNAL(textChanged(const QString &)), this,SLOT(on_version_lineEdit_textChanged(const QString &)));
    connect(ui->suffix_lineEdit, SIGNAL(textChanged(const QString &)), this,SLOT(on_suffix_lineEdit_textChanged(const QString &)));
    connect(ui->deb, SIGNAL(toggled(bool)), this, SLOT(debStateChanged(bool)));
    connect(ui->debsrc, SIGNAL(toggled(bool)), this, SLOT(debSrcStateChanged(bool)));
    connect(ui->custom_CheckBox, SIGNAL(stateChanged(int)), this, SLOT(custom_CheckBoxStateChanged(int)));
    connect(ui->class_CheckBox, SIGNAL(stateChanged(int)), this, SLOT(class_CheckBoxStateChanged(int)));
    connect(ui->main, SIGNAL(stateChanged(int)), this, SLOT(branchStrupdate(int)));
    connect(ui->restricted, SIGNAL(stateChanged(int)), this, SLOT(branchStrupdate(int)));
    connect(ui->universe, SIGNAL(stateChanged(int)), this, SLOT(branchStrupdate(int)));
    connect(ui->multiverse, SIGNAL(stateChanged(int)), this, SLOT(branchStrupdate(int)));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(versionBoxSel(const QString &)));
    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(addBtnClicked()) );
    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(cancelBtnClicked()) );

    ui->version_lineEdit->setFocusPolicy(Qt::NoFocus);

    QString matedataFile = "/tmp/sourcemanager/version";
    QFile file(matedataFile);
    if( file.exists()){
        QStringList versionList = getVersionList("/tmp/sourcemanager/version");
        ui->comboBox->addItems(versionList);
        qDebug()<<versionList;
    }else {
        QStringList versionList1;
        versionList1 <<"v10"<<"v10sp1"<<"2004"<<"2010";
        ui->comboBox->addItems(versionList1);
    }
    ui->horizontalLayout_5->setSpacing(0);
    ui->version_lineEdit->hide();
    ui->suffix_lineEdit->hide();
    ui->textEdit->setStyleSheet("border: none;");
    ui->add_lineEdit->setText("http://");


//    QPalette palette = ui->comboBox->palette();
//    QColor ColorPlaceholderText(255,255,255,0);
//    QBrush brush;
//    brush.setColor(ColorPlaceholderText);
//    palette.setBrush(QPalette::Button, brush);
//    ui->comboBox->setPalette(palette);
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
    ui->version_lineEdit->setStyleSheet("QLineEdit{border:1px solid #CFCFCF border-radius:1px}");
    versionStr = arg1;
    setSource();
}

void addSource::on_suffix_lineEdit_textChanged(const QString &arg1)
{
    ui->suffix_lineEdit->setStyleSheet("QLineEdit{border:1px solid #CFCFCF border-radius:1px}");
    suffixStr = arg1;
    ui->comboBox->setCurrentIndex(1);
    setSource();
}

void addSource::debStateChanged(bool state)
{
    if(state){
        typeStr = "deb";
        ui->debsrc->setChecked(false);
        setSource();
    }
}

void addSource::debSrcStateChanged(bool state)
{
    if(state){
        typeStr = "deb-src";
        ui->deb->setChecked(false);
        setSource();
    }
}

void addSource::custom_CheckBoxStateChanged(int state)
{
    if(state == Qt::Checked){
        ui->comboBox->setEnabled(false);
        ui->version_lineEdit->setFocusPolicy(Qt::StrongFocus);
        versionStr = "";
        ui->version_lineEdit->show();
        //        ui->comboBox->setStyleSheet("background-color: rgba(233, 233, 233, 1);");
        //        QPalette palette = ui->comboBox->palette();
        //        QColor ColorPlaceholderText(213, 213, 213,1);
        //        QBrush brush;
        //        brush.setColor(ColorPlaceholderText);
        //        palette.setBrush(QPalette::Button, brush);
        //        ui->comboBox->setPalette(palette);
    }else{
        ui->comboBox->setEnabled(true);
        versionStr = ui->comboBox->currentText();
        ui->version_lineEdit->hide();
        //        ui->comboBox->setStyleSheet("background-color: rgba(255, 255, 255, 1);");
        //        QPalette palette = ui->comboBox->palette();
        //        QColor ColorPlaceholderText(255,255,255,1);
        //        QBrush brush;
        //        brush.setColor(ColorPlaceholderText);
        //        palette.setBrush(QPalette::Button, brush);
        //        ui->comboBox->setPalette(palette);
    }
    setSource();
}

void addSource::class_CheckBoxStateChanged(int state)
{
    if(state == Qt::Checked){
        ui->suffix_lineEdit->show();
        suffixStr = ui->suffix_lineEdit->text();
    }else {
        ui->suffix_lineEdit->hide();
        suffixStr = "";
    }
    setSource();
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
    if(ui->custom_CheckBox->isChecked() == true){
        ui->version_lineEdit->setFocusPolicy(Qt::StrongFocus);
        versionStr = ui->suffix_lineEdit->text();
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
    ui->textEdit->setText(source);
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

    if(ui->version_lineEdit->text().isEmpty() && ui->custom_CheckBox->isChecked() == true){
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

QStringList addSource::getVersionList(QString fileName)
{
    QStringList sourceList;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<fileName<<"----file open failed!";
        return sourceList;
    }
    QTextStream in(&file);
    QString line = in.readLine();
    if((line.left(1).compare("#") !=0) && (line.compare("") !=0)){
        sourceList<<line;
    }
    while(!line.isNull())//字符串有内容
    {
        line=in.readLine();//循环读取下行
        if((line.left(1).compare("#") !=0) && (line.compare("") !=0)){
            sourceList << line;
        }
    }
    file.close();
    return sourceList;
}

void addSource::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //painter.setBrush(QBrush(QColor(255, 255, 255)));
    QStyleOption opt;
    opt.init(this);
    painter.setBrush(opt.palette.color(QPalette::Base));

    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setWidth(rect.width() - 0);
    rect.setHeight(rect.height() - 0);
    painter.drawRoundedRect(rect, 7, 7);
    {
        QPainterPath painterPath;
        painterPath.addRoundedRect(rect, 7, 7);
        painter.drawPath(painterPath);
    }
    QWidget::paintEvent(event);
}



