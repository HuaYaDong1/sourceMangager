#include "sourcemanager.h"
#include "ui_sourcemanager.h"
#include <QScroller>
#include "sourceinformationwidget.h"
#include "ui_sourceinformationwidget.h"
#include <QDebug>
#include "sourceinterface.h"
#include "delayrefreshthread.h"
#include <QDBusInterface>
#include <QVariantList>
#include <QDBusPendingCall>
#include "addsource.h"
#include "ui_addsource.h"
#include "testingupdatethread.h"
#include "createsourcewidget.h"
#include "ui_createsourcewidget.h"


sourceManager::sourceManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::sourceManager)
{
    ui->setupUi(this);
    sourceinterface = new sourceInterface;
    deleteFlag = 0;
    changeFlag = 0;
    UnnecessarySageCount = 0;

    connect(ui->deleteBtn, SIGNAL(clicked()), this, SLOT(deleteBtnClicked()) );
    connect(ui->refreshBtn, SIGNAL(clicked()), this, SLOT(refreshBtnClicked()) );
    connect(ui->refreshBtn_2, SIGNAL(clicked()), this, SLOT(refreshBtnClicked()) );
    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(addBtnClicked()));
    connect(ui->pageMainSourceBtn, SIGNAL(clicked()), this, SLOT(pageMainSourceBtnClicked()));
    connect(ui->pageUnnecessaryBtn, SIGNAL(clicked()), this, SLOT(pageUnnecessaryBtnClicked()));
    connect(ui->updateBtn, SIGNAL(clicked()), this, SLOT(updateBtnClicked()));
    connect(ui->updateBtn_2, SIGNAL(clicked()), this, SLOT(updateBtnClicked()));
    connect(ui->setSourceBtn, SIGNAL(clicked()), this, SLOT(setSourceBtnClicked()));
    connect(ui->changeBtn, SIGNAL(clicked()), this, SLOT(changeBtnClicked()));
    connect(ui->createSourceBtn, SIGNAL(clicked()), this, SLOT(createSourceBtnClicked()));
    connect(ui->stopBtn, SIGNAL(clicked()), this, SLOT(stopBtnClicked()));
    connect(ui->stopBtn_1, SIGNAL(clicked()), this, SLOT(stopBtnClicked()));
    connect(ui->label_10, SIGNAL(clicked()), this, SLOT(UnnecessarySourcesShow()));
    connect(ui->label_11, SIGNAL(clicked()), this, SLOT(UnnecessarySourcesShow()));


    QDBusConnection::systemBus().registerObject("/citos/client/path",
                                                this,
                                                QDBusConnection :: ExportAllSlots |
                                                QDBusConnection :: ExportAllSignals
                                                );
    QDBusConnection::systemBus().connect(QString(),
                                         QString("/citos/client/path"),
                                         "com.client.test",
                                         "updateOver",
                                         this,
                                         SLOT(updateOverSlot(QString))
                                         );

    showMainSource(ui->listWidget);//主源页展示
    searchUnnecessarySourcesNumber();//搜查次源个数
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_3->setCurrentIndex(1);
    ui->createSourceBtn->hide();
    ui->page_3->hide();
    ui->stopBtn->hide();
    ui->stopBtn_1->hide();
    ui->setSourceBtn->hide();
    ui->changeBtn->hide();

    selectWidget = ui->listWidget;
    UnnecessarySourcesSelect();

    connect(sourceinterface,SIGNAL(downloadover(QString ,QListWidget *, int )),this,SLOT(downloadOverSlot(QString ,QListWidget *, int )));
    connect(sourceinterface,SIGNAL(downloadspeed(QString ,QListWidget *, int )),this,SLOT(downloadspeed(QString ,QListWidget *, int )));

    QPalette palette = ui->frame->palette();
    QColor ColorPlaceholderText(25,25,25,0);
    QBrush brush;
    brush.setColor(ColorPlaceholderText);
    palette.setBrush(QPalette::Window, brush);
    ui->frame->setPalette(palette);
}

sourceManager::~sourceManager()
{
    delete ui;
}

void sourceManager::downloadOverSlot(QString speed,QListWidget *Listwidget, int Num)
{
    if(!Nettestflag)
    {
        return ;
    }
    sourceInformationWidget* pwig = static_cast<sourceInformationWidget*> (Listwidget->itemWidget(Listwidget->item(Num)));
    if(!pwig->ui->delay_Label->text().compare("无法连接") == 0)
    {
        pwig->ui->spend_Label->setText(speed);
    }else{
        pwig->ui->spend_Label->setText("无法连接");
    }

    Num=Num+2;
    while(Num != Listwidget->count())
    {
        sourceInformationWidget* pwig1 = static_cast<sourceInformationWidget*> (Listwidget->itemWidget(Listwidget->item(Num)));
        qDebug()<<pwig1->ui->address_Check->text();
        if(!pwig1->ui->delay_Label->text().compare("无法连接") == 0)
        {
            QString address;
            if(selectWidget == ui->listWidget){
                address = sourceinterface->getMainSourceAddressList("/etc/apt/mainsources/"+pwig->ui->address_Check->text()+".list").at(0);
            }else{
                address = pwig->ui->address_Check->text();
            }
            sourceinterface->getDownloadSpeedFromSource(address, Listwidget, Num);
            break;
        }
        else
        {
            pwig1->ui->spend_Label->setText("无法连接");
        }
        qDebug()<<Num  << Listwidget->count()-1;
        Num=Num+2;
    }
    if(Num == Listwidget->count()){
        ui->stopBtn->hide();
        ui->stopBtn_1->hide();
        ui->refreshBtn->show();
        ui->refreshBtn_2->show();
    }
}

void sourceManager::downloadspeed(QString speed,QListWidget *Listwidget, int Num)
{
    sourceInformationWidget* pwig = static_cast<sourceInformationWidget*> (Listwidget->itemWidget(Listwidget->item(Num)));
    pwig->ui->spend_Label->setText(speed);
}

//主源页切换
void sourceManager::pageMainSourceBtnClicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
    selectWidget = ui->listWidget;
    ui->updateLabel->setText("");
    //ui->setSourceBtn->show();
    ui->createSourceBtn->hide();
    ui->changeBtn->hide();
}

//次源页切换
void sourceManager::pageUnnecessaryBtnClicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
    ui->stackedWidget_3->setCurrentIndex(UnnecessarySageCount+1);
    selectWidget = UnnecessaryWidget;
    ui->updateLabel->setText("");
    //ui->setSourceBtn->hide();
    ui->createSourceBtn->show();
    ui->changeBtn->show();
}

//搜查次源个数,添加按钮，页面，及listwidget
void sourceManager::searchUnnecessarySourcesNumber()
{
    SourceList = sourceinterface->getSourceName();
    pageNum = SourceList.count();
    UnnecessarySources = new  set_alarm_repeat_Dialog(ui->sourceNameWidget_2, pageNum);UnnecessarySources->hide();
    connect(UnnecessarySources->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(UnnecessarySourcesSelect_listClickslot()));
    ui->label_11->setText(SourceList.at(0));

    UnnecessarySourceName = SourceList.at(0); //第一次副源页切换，当前页名字
    qDebug()<<SourceList;

    //QWidget * page[1024];
    QListWidget * WidgetList[1024];
    QPushButton *unnecessarySourceBtn[1024];

    for(int i=0; i<pageNum; i++)
    {
        page[i] = new QWidget();
        unnecessarySourceBtn[i] = new QPushButton();
        ui->horizontalLayout_3->addWidget(unnecessarySourceBtn[i]);
        ui->stackedWidget_3->addWidget(page[i]);
        QVBoxLayout *verticalLayout = new QVBoxLayout(page[i]);
        unnecessarySourceBtn[i]->setText(SourceList.at(i));

        WidgetList[i] = new QListWidget(page[i]);
        WidgetList[i]->setObjectName(SourceList.at(i));
        ui->verticalLayout_3->addWidget(WidgetList[i]);
        verticalLayout->addWidget(WidgetList[i]);
        WidgetSelect[i] = WidgetList[i];
        connect(unnecessarySourceBtn[i], &QPushButton::clicked, this, [=]()
        {
            ui->stackedWidget_3->setCurrentIndex(i+1);
            selectWidget = WidgetList[i];
            UnnecessaryWidget = WidgetList[i];
            UnnecessarySourceName = SourceList.at(i);
            UnnecessarySageCount = i;
            ui->updateLabel->setText("");
            seletcPage = page[i];
            selectBtn = unnecessarySourceBtn[i];
        } );
        connect(WidgetList[i],SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(selectDeleteIteam(QListWidgetItem*)));
        UnnecessaryWidget = WidgetList[0];
        qDebug()<<SourceList.at(i);
        showUnnecessarySource(WidgetList[i], i);
        unnecessarySourceBtn[i]->hide();
    }
}

//页面切换回调
void sourceManager::SourceBtnClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    selectWidget = ui->listWidget;
}

//主源展示页回调
void sourceManager::showMainSource(QListWidget *listWidget)
{
    QStringList adddresslist = sourceinterface->getMainSourceName();

    initializationList(listWidget,adddresslist.count());//初始化列表框
    fillInMainTheData(listWidget, adddresslist); //填充地址，数据
    fillInMainDynamicData(listWidget, adddresslist.count());
}

//次源展示页回调
void sourceManager::showUnnecessarySource(QListWidget *listWidget, int num)
{
    QStringList adddresslist = sourceinterface->getSourceAddressList("/etc/apt/sources.list.d/"+SourceList.at(num));
    QStringList typeList= sourceinterface->getSourceTypeList("/etc/apt/sources.list.d/"+SourceList.at(num));

    initializationList(listWidget,adddresslist.count());//初始化列表框
    fillInTheData(listWidget, adddresslist, typeList); //填充地址，类型，数据
    fillInTheDynamicData(listWidget, adddresslist.count());
}

//初始化列表框
void sourceManager::initializationList(QListWidget *ListWidget, int sourseNum)
{
    QScroller::grabGesture(ListWidget,QScroller::LeftMouseButtonGesture);
    ListWidget -> setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ListWidget -> setFrameShape(QListWidget::NoFrame);

    QListWidgetItem *aItem =new QListWidgetItem;
    aItem->setSizeHint(QSize(50, 1));
    ListWidget->addItem(aItem);
    QWidget *line = new QWidget(ListWidget);
    line->setStyleSheet("background-color: rgba(139, 140, 139, 1);");
    ListWidget->setItemWidget(aItem,line);

    for (int i = 0; i < sourseNum; i++)
    {
        QListWidgetItem *aItem =new QListWidgetItem;
        aItem->setSizeHint(QSize(50, 55));
        ListWidget->addItem(aItem);
        sourceInformationWidget *widget = new sourceInformationWidget(ListWidget);
        ListWidget->setItemWidget(aItem,widget);
        if(ListWidget == ui->listWidget){//checkbox点击回调
            connect(widget->ui->address_Check, &QCheckBox::stateChanged, this, [=](int state)
            {
                for(int j = 0; j < SourceLabelList.count(); j++)
                {
                    delete source[j];
                }
                if(state == Qt::Checked){
                    SourceLabelList << widget->ui->address_Check->text();
                    mainSourceSet();
                    addMainSource(widget->ui->address_Check->text());
                }else{
                    for(int j = 0; j < SourceLabelList.count(); j++)
                    {
                        if(widget->ui->address_Check->text().compare(SourceLabelList.at(j)) == 0){
                            SourceLabelList.removeAt(j);
                        }
                    }
                    mainSourceSet();
                    delMainSource(widget->ui->address_Check->text());
                }
            });
        }else{
            connect(widget->ui->address_Check, &QCheckBox::stateChanged, this, [=](int state)
            {
                if(state == Qt::Checked){

                }
            });
        }
        QListWidgetItem *aItem1 =new QListWidgetItem;
        aItem1->setSizeHint(QSize(50, 1));
        ListWidget->addItem(aItem1);
        QWidget *line = new QWidget(ListWidget);
        line->setStyleSheet("background-color: rgba(139, 140, 139, 1);");
        ListWidget->setItemWidget(aItem1,line);

        widget->ui->type_Label->hide();
    }
}

void sourceManager::addMainSource(QString sourceName)
{
    QVariantList sourceFileName;
    sourceFileName << QVariant::fromValue(sourceName);
    QDBusInterface *serviceInterface = new QDBusInterface("com.softSource.manager",
                                                          "/com/softSource/Manager",
                                                          "com.softSource.manager.interface",
                                                          QDBusConnection::systemBus());
    if(!serviceInterface->isValid())
    {
        qDebug() << "Service Interface: " << qPrintable(QDBusConnection::systemBus().lastError().message());
        return;
    }

    serviceInterface->asyncCall("addMainSource",sourceFileName);
}
void sourceManager::delMainSource(QString sourceName)
{
    QVariantList sourceFileName;
    sourceFileName << QVariant::fromValue(sourceName);
    QDBusInterface *serviceInterface = new QDBusInterface("com.softSource.manager",
                                                          "/com/softSource/Manager",
                                                          "com.softSource.manager.interface",
                                                          QDBusConnection::systemBus());
    if(!serviceInterface->isValid())
    {
        qDebug() << "Service Interface: " << qPrintable(QDBusConnection::systemBus().lastError().message());
        return;
    }
    serviceInterface->asyncCall("delMainSource",sourceFileName);
}
//在设置主源 添加 或 删除源名字label
void sourceManager::mainSourceSet()
{
    for(int i = 0; i< SourceLabelList.count(); i++)
    {
        source[i] = new QLabel;
        source[i]->setText(SourceLabelList.at(i));
        source[i]->setMaximumSize(QSize(16777215, 21));
        source[i]->setStyleSheet("QLabel{border:1px groove #DDDDDD;}");
        ui->gridLayout_2->addWidget(source[i], i/5, i%5+1, 1, 1);
    }
}

//主源页数据填充静态数据
void sourceManager::fillInMainTheData(QListWidget *ListWidget,
                                      QStringList addressList)
{
    int j = 1;
    for(int i = 0; i < addressList.count(); i++){
        QListWidgetItem *item = ListWidget->item(j);
        sourceInformationWidget* pwig = static_cast<sourceInformationWidget*> (ListWidget->itemWidget(item));
        QString address = addressList.at(i);QString name ;
        QStringList list = address.split(".");
        for(int n = 0; n < list.count()-1; n++)
        {
            if(n == 0){
                name = name+list.at(n);
            }else{
                name = name+"."+list.at(n);
            }
        }
        pwig->ui->address_Check->setText(name);
        pwig->ui->spend_Label->setText("等待检测");
        j=j+2;
    }
}

//次源页填充静态数据
void sourceManager::fillInTheData(QListWidget *ListWidget,
                                  QStringList addressList,
                                  QStringList typeList)
{
    int j = 1;
    for(int i = 0; i < addressList.count(); i++){
        QListWidgetItem *item = ListWidget->item(j);
        sourceInformationWidget* pwig = static_cast<sourceInformationWidget*> (ListWidget->itemWidget(item));
        pwig->ui->address_Check->setText(addressList.at(i));
        pwig->ui->spend_Label->setText("等待检测");
        QString type = typeList.at(i);
        if(type.compare("deb") == 0)
        {
            pwig->ui->type_Label->setText("软件源");
        }
        else
        {
            pwig->ui->type_Label->setText("源码源");
        }
        j=j+2;
    }
}

//填充主源动态数据延时
void sourceManager::fillInMainDynamicData(QListWidget *ListWidget, int num)
{
    int j = 1;
    for(int i = 0; i < num; i++){
        QListWidgetItem *item = ListWidget->item(j);
        sourceInformationWidget* pwig = static_cast<sourceInformationWidget*> (ListWidget->itemWidget(item));
        QStringList address = sourceinterface->getMainSourceAddressList("/etc/apt/mainsources/"+pwig->ui->address_Check->text()+".list") ;
        delayRefreshThread *thread = new delayRefreshThread(pwig->ui->spend_Label, pwig->ui->delay_Label, address.at(0));
        thread->start();
        j=j+2;
    }
}

//填充动态数据延时
void sourceManager::fillInTheDynamicData(QListWidget *ListWidget, int num)
{
    int j = 1;
    for(int i = 0; i < num; i++){
        QListWidgetItem *item = ListWidget->item(j);
        sourceInformationWidget* pwig = static_cast<sourceInformationWidget*> (ListWidget->itemWidget(item));
        delayRefreshThread *thread = new delayRefreshThread(pwig->ui->spend_Label, pwig->ui->delay_Label, pwig->ui->address_Check->text());
        thread->start();
        j=j+2;
    }
}

//刷新按钮回调
void sourceManager::refreshBtnClicked()
{
    ui->stopBtn->show();
    ui->stopBtn_1->show();
    ui->refreshBtn->hide();
    ui->refreshBtn_2->hide();
    Nettestflag = true;
    qDebug()<<Nettestflag;
    qDebug()<<"~~~~~~~~刷新~~~~~~~~~~~~~~~~~:"<<selectWidget->objectName();
    if(selectWidget == ui->listWidget){
        fillInMainDynamicData(selectWidget, selectWidget->count()/2-1);
    }else{
        fillInTheDynamicData(selectWidget, selectWidget->count()/2-1);
    }
    //激活循序下载测速
    for(int i = 1; i < selectWidget->count(); i=i+2)
    {
        sourceInformationWidget* pwig = static_cast<sourceInformationWidget*> (selectWidget->itemWidget(selectWidget->item(i)));
        if(!pwig->ui->delay_Label->text().compare("无法连接") == 0)//跳过无法连接的源
        {
            QString address;
            if(selectWidget == ui->listWidget){ //区分是主源还是次源
                address = sourceinterface->getMainSourceAddressList("/etc/apt/mainsources/"+pwig->ui->address_Check->text()+".list").at(0);
            }else{
                address = pwig->ui->address_Check->text();
            }
            sourceinterface->getDownloadSpeedFromSource(address, selectWidget, i);
            break;
        }else{
            pwig->ui->spend_Label->setText("无法连接");
        }
    }
}

//单击选中item回调
void sourceManager::selectDeleteIteam(QListWidgetItem *item)
{
    sourceInformationWidget* pwig =static_cast<sourceInformationWidget*> (selectWidget->itemWidget(item));
    if(pwig->ui->address_Check->text().compare("address") == 0)
    {
        deleteFlag = 0;
        return;
    }
    delete_item = item;
    deleteFlag = 1;
    changeFlag = 1;
    qDebug() << "address : " << pwig->ui->address_Check->text() ;
}

//次源选择页展示
void sourceManager::UnnecessarySourcesSelect()
{
    UnnecessarySources->listWidget->setFixedSize(300,162);
    UnnecessarySources->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    UnnecessarySources->setAttribute(Qt::WA_TranslucentBackground);
    UnnecessarySources->resize(300,162);
    UnnecessarySources->listWidget->move(0,0);
    for(int i = 0; i < SourceList.count(); i++)
    {
        UnnecessarySources->widget[i]->alarmLabel0->setText(SourceList.at(i));
        //删除次源文件
        connect(UnnecessarySources->widget[i]->alarmLabel1, &QPushButton::clicked, this,[=]()
        {
            QVariantList sourceFileName;
            QDBusInterface *serviceInterface = new QDBusInterface("com.softSource.manager",
                                                                  "/com/softSource/Manager",
                                                                  "com.softSource.manager.interface",
                                                                  QDBusConnection::systemBus());
            if(!serviceInterface->isValid())
            {
                qDebug() << "Service Interface: " << qPrintable(QDBusConnection::systemBus().lastError().message());
                return;
            }
            sourceFileName<< QVariant::fromValue(UnnecessarySources->widget[i]->alarmLabel0->text());
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("question"),
                                          tr("delete 当前页面源 ?"),
                                          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if(reply == QMessageBox::Yes){
                serviceInterface->asyncCall("deleteSourceFile", sourceFileName);
                ui->updateLabel->setText("删除源");
                //删除 页与 按钮
                delete selectBtn;
                delete seletcPage;
            }else if (reply == QMessageBox::No){
            }else{}
            SourceList.removeAll(selectWidget->objectName());
            qDebug() << selectWidget->objectName() <<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~````";
            qDebug() << SourceList.count() << "!~!~~!~!~!~!~!~!~!~"<< UnnecessarySageCount;
            delete UnnecessarySources->listWidget->takeItem(i);
            pageNum--;
            ui->stackedWidget_3->removeWidget(page[i]);
        });
    }
}

void sourceManager::UnnecessarySourcesShow()
{
    QPointF position = this->pos();
    UnnecessarySources->move(position.x()+29,position.y()+150);
    UnnecessarySources->show();
}

//次源选择页回调
void sourceManager::UnnecessarySourcesSelect_listClickslot()
{
    int num=UnnecessarySources->listWidget->currentRow();

    set_alarm_repeat_widget *pwig =static_cast<set_alarm_repeat_widget*> (UnnecessarySources->listWidget->
                                                                          itemWidget(UnnecessarySources->listWidget->item(num)));

    selectWidget = WidgetSelect[num];
    ui->label_11->setText(pwig->alarmLabel0->text());
    UnnecessarySources->close();
    ui->stackedWidget_3->setCurrentIndex(num+1);
}

//change按钮点击回调
void sourceManager::changeBtnClicked()
{
    int number;
    if(!changeFlag)
    {
        return;
    }
    sourceInformationWidget* pwig =static_cast<sourceInformationWidget*> (selectWidget->itemWidget(delete_item));
    QDBusInterface *serviceInterface = new QDBusInterface("com.softSource.manager",
                                                          "/com/softSource/Manager",
                                                          "com.softSource.manager.interface",
                                                          QDBusConnection::systemBus());
    if(!serviceInterface->isValid())
    {
        qDebug() << "Service Interface: " << qPrintable(QDBusConnection::systemBus().lastError().message());
        return;
    }
    QVariantList sourceChange;

    addSource *addSourcewidget = new addSource();

    QStringList Address = pwig->ui->address_Check->text().split(" ");
    sourceChange << QVariant::fromValue(pwig->ui->address_Check->text());//原数据
    addSourcewidget->ui->addBtn->setText("更改");
    QString type = Address.at(0);//类型
    if(type.compare("deb") == 0){
        addSourcewidget->ui->deb->setChecked(true);
        addSourcewidget->ui->debsrc->setChecked(false);
    }else{
        addSourcewidget->ui->deb->setChecked(false);
        addSourcewidget->ui->debsrc->setChecked(true);
    }

    for(int i=1; i<Address.count(); i++)//网址
    {
        QRegExp rx("(https?|ftp|file)://[-A-Za-z0-9+&@#/%?=~_|!:,.;]+[-A-Za-z0-9+&@#/%=~_|]");//正则表达式判断网址格式是否正确
        int pos=0;
        QRegExpValidator v(rx, 0);
        QString http =Address.at(i);
        if (2==v.validate(http,pos))
        {
            addSourcewidget->ui->add_lineEdit->setText(Address.at(i));//网址
            number = i+1;
        }
    }
    addSourcewidget->ui->version_lineEdit->setText(Address.at(number));//版本
    for(int i=number+1; i<Address.count(); i++)//分类目录
    {
        QString branch = Address.at(i);
        if(branch.compare("main") == 0){
            addSourcewidget->ui->main->setChecked(true);
        }else if(branch.compare("restricted") == 0){
            addSourcewidget->ui->restricted->setChecked(true);
        }else if(branch.compare("universe") == 0){
            addSourcewidget->ui->universe->setChecked(true);
        }else if(branch.compare("multiverse") == 0){
            addSourcewidget->ui->multiverse->setChecked(true);
        }else{
            addSourcewidget->ui->suffix_lineEdit->setText(branch+" ");
        }
    }

    addSourcewidget->exec();
    if(!addSourcewidget->isAddBtnClicked)
    {
        return;
    }

    sourceChange << QVariant::fromValue(addSourcewidget->ui->textEdit->toPlainText());//新数据
    sourceChange << QVariant::fromValue("/etc/apt/sources.list.d/"+selectWidget->objectName());//文件
    serviceInterface->asyncCall("changedSource", sourceChange);
    delete delete_item;

    addForListwidget(selectWidget, addSourcewidget->ui->textEdit->toPlainText());
    changeFlag = 0;
}

//删除按钮点击回调
void sourceManager::deleteBtnClicked()
{
    questionMessage();//删除按钮前询问
}

//删除前询问
void sourceManager::questionMessage()
{
    if(deleteFlag)
    {
        sourceInformationWidget* pwig =static_cast<sourceInformationWidget*> (selectWidget->itemWidget(delete_item));
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("question"),
                                      tr("delete ?"),
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        QDBusInterface *serviceInterface = new QDBusInterface("com.softSource.manager",
                                                              "/com/softSource/Manager",
                                                              "com.softSource.manager.interface",
                                                              QDBusConnection::systemBus());
        if(!serviceInterface->isValid())
        {
            qDebug() << "Service Interface: " << qPrintable(QDBusConnection::systemBus().lastError().message());
            return;
        }
        QVariantList sourceDelete;
        if(reply == QMessageBox::Yes){
            qDebug()<<"~~~~~~"<<selectWidget->objectName();
            if(!selectWidget->objectName().compare("sources.list"))
            {
                sourceDelete<< QVariant::fromValue("/etc/apt/"+selectWidget->objectName())
                            << QVariant::fromValue(pwig->ui->address_Check->text());
            }
            else
            {
                sourceDelete<< QVariant::fromValue("/etc/apt/sources.list.d/"+selectWidget->objectName())
                            << QVariant::fromValue(pwig->ui->address_Check->text());
            }
            serviceInterface->asyncCall("deleteSource", sourceDelete);
            delete delete_item;
            deleteFlag = 0;
        }else if (reply == QMessageBox::No){
        }else{}
    }
}

//添加源按钮回调
void sourceManager::addBtnClicked()
{
    qDebug() << "Service Interface:~~~~~~~~~~~~~~~~ " << selectWidget->objectName();
    QDBusInterface *serviceInterface = new QDBusInterface("com.softSource.manager",
                                                          "/com/softSource/Manager",
                                                          "com.softSource.manager.interface",
                                                          QDBusConnection::systemBus());
    if(!serviceInterface->isValid())
    {
        qDebug() << "Service Interface: " << qPrintable(QDBusConnection::systemBus().lastError().message());
        return;
    }
    QVariantList sourceDelete;
    addSource *addSourcewidget = new addSource();
    addSourcewidget->exec();
    if(addSourcewidget->ui->textEdit->toPlainText().isEmpty())
    {
        return ;
    }
    if(!addSourcewidget->isAddBtnClicked)
    {
        return;
    }
    QString sourceinfo = addSourcewidget->ui->textEdit->toPlainText();

    if(!selectWidget->objectName().compare("sources.list"))
    {
        sourceDelete<< QVariant::fromValue("/etc/apt/"+selectWidget->objectName())
                    << QVariant::fromValue(sourceinfo);
    }
    else
    {
        sourceDelete<< QVariant::fromValue("/etc/apt/sources.list.d/"+selectWidget->objectName())
                    << QVariant::fromValue(sourceinfo);
    }
    qDebug() <<sourceinfo;
    serviceInterface->asyncCall("addSource", sourceDelete);
    addForListwidget(selectWidget, sourceinfo);
}

//添加新列后刷新
void sourceManager::addForListwidget(QListWidget *ListWidget, QString address)
{
    QListWidgetItem *aItem =new QListWidgetItem;
    aItem->setSizeHint(QSize(50, 55));
    ListWidget->addItem(aItem);
    sourceInformationWidget *widget = new sourceInformationWidget(ListWidget);
    ListWidget->setItemWidget(aItem,widget);
    QStringList Address = address.split(" ");
    widget->ui->address_Check->setText(address);
    QString type = Address.at(0);
    if(type.compare("deb") == 0){
        widget->ui->type_Label->setText("软件源");
    }else{
        widget->ui->type_Label->setText("源码源");
    }

    widget->ui->type_Label->hide();
    widget->ui->spend_Label->setText("等待检测");

    QListWidgetItem *aItem1 =new QListWidgetItem;
    aItem1->setSizeHint(QSize(50, 1));
    ListWidget->addItem(aItem1);
    QWidget *line = new QWidget(ListWidget);
    line->setStyleSheet("background-color: rgba(139, 140, 139, 1);");
    ListWidget->setItemWidget(aItem1,line);
}

//apt-get update
void sourceManager::updateBtnClicked()
{
    QDBusInterface *serviceInterface = new QDBusInterface("com.softSource.manager",
                                                          "/com/softSource/Manager",
                                                          "com.softSource.manager.interface",
                                                          QDBusConnection::systemBus());
    if(!serviceInterface->isValid())
    {
        qDebug() << "Service Interface: " << qPrintable(QDBusConnection::systemBus().lastError().message());
        return;
    }
    serviceInterface->asyncCall("updateSource");
    ui->updateLabel->setText("正在更新");
}

//接收后台执行成功返回信号
void sourceManager::updateOverSlot(QString str)
{
    qDebug() << "更新结束"<<str;
    ui->updateLabel->setText(str);
}

//设置主源
void sourceManager::setSourceBtnClicked()
{
    QVariantList sourceFileName;
    QDBusInterface *serviceInterface = new QDBusInterface("com.softSource.manager",
                                                          "/com/softSource/Manager",
                                                          "com.softSource.manager.interface",
                                                          QDBusConnection::systemBus());
    if(!serviceInterface->isValid())
    {
        qDebug() << "Service Interface: " << qPrintable(QDBusConnection::systemBus().lastError().message());
        return;
    }
    sourceFileName<< QVariant::fromValue("/etc/apt/mainsources/"+selectWidget->objectName());
    serviceInterface->asyncCall("setMainSource", sourceFileName);
}

//创建一个次源
void sourceManager::createSourceBtnClicked()
{
    createSourceWidget *createSource = new createSourceWidget();
    createSource->exec();

    if(createSource->isTrueBtnClicked){
        QString name = createSource->ui->nameLineEdit->text();
        UnnecessarySourceName = name; //第一次副源页切换，当前页名字

        QWidget *page1 = new QWidget();
        ui->stackedWidget_3->addWidget(page1);
        QVBoxLayout *verticalLayout = new QVBoxLayout(page1);

        QListWidget *WidgetList = new QListWidget(page1);
        WidgetList->setObjectName(name);
        ui->verticalLayout_3->addWidget(WidgetList);
        verticalLayout->addWidget(WidgetList);
        int i = pageNum;

        connect(WidgetList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(selectDeleteIteam(QListWidgetItem*)));

        UnnecessaryWidget = WidgetList;
        qDebug()<<name;

        QVariantList sourceFileName;
        QDBusInterface *serviceInterface = new QDBusInterface("com.softSource.manager",
                                                              "/com/softSource/Manager",
                                                              "com.softSource.manager.interface",
                                                              QDBusConnection::systemBus());
        if(!serviceInterface->isValid())
        {
            qDebug() << "Service Interface: " << qPrintable(QDBusConnection::systemBus().lastError().message());
            return;
        }
        sourceFileName<< QVariant::fromValue("/etc/apt/sources.list.d/"+name);
        serviceInterface->asyncCall("addExtensionSource", sourceFileName);
        initializationList(WidgetList,0);//初始化列表框
        pageNum++;

        ui->stackedWidget_3->setCurrentIndex(i+1);
        SourceList << name;
        selectWidget = WidgetList;
        ui->label_11->setText(name);

        QListWidgetItem *aItem =new QListWidgetItem;
        aItem->setSizeHint(QSize(276, 32));
        aItem->setTextColor(QColor(255, 0, 0, 255));
        UnnecessarySources->listWidget->addItem(aItem);
        set_alarm_repeat_widget *widget = new set_alarm_repeat_widget(UnnecessarySources->listWidget);
        UnnecessarySources->listWidget->setItemWidget(aItem,widget);
        widget->alarmLabel0->setText(name);

        connect(widget->alarmLabel1, &QPushButton::clicked, this,[=]()
        {
            QVariantList sourceFileName;
            QDBusInterface *serviceInterface = new QDBusInterface("com.softSource.manager",
                                                                  "/com/softSource/Manager",
                                                                  "com.softSource.manager.interface",
                                                                  QDBusConnection::systemBus());
            if(!serviceInterface->isValid())
            {
                qDebug() << "Service Interface: " << qPrintable(QDBusConnection::systemBus().lastError().message());
                return;
            }
            qDebug() << pageNum - 1 <<"~~~~~~~~~~~~~~~~~~~";
            int num = pageNum - 1;
            sourceFileName<< QVariant::fromValue(widget->alarmLabel0->text());
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("question"),
                                          tr("delete 当前页面源 ?"),
                                          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if(reply == QMessageBox::Yes){
                serviceInterface->asyncCall("deleteSourceFile", sourceFileName);
                ui->updateLabel->setText("删除源");
                //删除 页与 按钮
                delete selectBtn;
                //seletcPage->hide();
                delete seletcPage;
            }else if (reply == QMessageBox::No){
            }else{}
            SourceList.removeAll(selectWidget->objectName());
            qDebug() << selectWidget->objectName() <<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~````";
            qDebug() << SourceList.count() << "~!~!~!~!~!~!~!~!~!~"<< UnnecessarySageCount;
            delete UnnecessarySources->listWidget->takeItem(num);
            pageNum--;
            ui->stackedWidget_3->removeWidget(page[num]);
        });
    }
}

void sourceManager::stopBtnClicked()
{
    ui->stopBtn->hide();
    ui->stopBtn_1->hide();
    ui->refreshBtn->show();
    ui->refreshBtn_2->show();
    Nettestflag = false;
    qDebug()<<Nettestflag;
}

//删除当前源回调
void sourceManager::deleteSourcePageBtnClicked()
{
    QVariantList sourceFileName;
    QDBusInterface *serviceInterface = new QDBusInterface("com.softSource.manager",
                                                          "/com/softSource/Manager",
                                                          "com.softSource.manager.interface",
                                                          QDBusConnection::systemBus());
    if(!serviceInterface->isValid())
    {
        qDebug() << "Service Interface: " << qPrintable(QDBusConnection::systemBus().lastError().message());
        return;
    }
    sourceFileName<< QVariant::fromValue(selectWidget->objectName());

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("question"),
                                  tr("delete 当前页面源 ?"),
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if(reply == QMessageBox::Yes){
        serviceInterface->asyncCall("deleteSourceFile", sourceFileName);
        ui->updateLabel->setText("删除源");
        //删除 页与 按钮
        delete selectBtn;
        //seletcPage->hide();
        delete seletcPage;
    }else if (reply == QMessageBox::No){
    }else{}

    SourceList.removeAll(selectWidget->objectName());
    delete UnnecessarySources->listWidget->takeItem(0);
    pageNum--;
}

//检测源是否为最新
void sourceManager::testingUpdateBtnClicked()
{
    QString name = selectWidget->objectName();
    QStringList sourcelist1 = sourceinterface->getSourceAddressList("/etc/apt/sources.list");
    QStringList sourcelist2 = sourceinterface->getSourceAddressList("/etc/apt/mainsources/"+name);
    if(sourcelist1.count() != sourcelist2.count())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::critical(this, tr("critical"),  tr("数量不同需要更新"), QMessageBox::Yes);
        if(reply == QMessageBox::Yes)
        {
            return ;
        }
    }
    else
    {
        for(int i = 0; i<sourcelist1.count(); i++)
        {
            bool flag = false;
            QString source1 = sourcelist1.at(i);
            for(int j = 0; j<sourcelist2.count(); j++)
            {
                QString source2 = sourcelist2.at(j);
                if(source1.compare(source2) == 0)
                {
                    flag = true;
                    break;
                }
            }
            if(!flag){
                QMessageBox::StandardButton reply;
                reply = QMessageBox::critical(this, tr("critical"),  tr("内容不同需要更新"), QMessageBox::Yes);
                if(reply == QMessageBox::Yes)
                {
                    return ;
                }
            }
        }
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::information(this, tr("information"),  tr("完全相同不需要更新"), QMessageBox::Yes);
    if(reply == QMessageBox::Yes)
    {
        return ;
    }
}

void sourceManager::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    QStyleOption opt;
    opt.init(this);
    painter.setBrush(opt.palette.color(QPalette::Base));

    if(QColor(255,255,255) == opt.palette.color(QPalette::Base) || QColor(248,248,248) == opt.palette.color(QPalette::Base))
    {
        painter.setBrush(QColor(255,255,255));
    }else{
        painter.setBrush(QColor(34,34,34));
    }

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
