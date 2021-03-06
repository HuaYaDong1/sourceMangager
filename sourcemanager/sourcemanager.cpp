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
    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(addBtnClicked()));
    connect(ui->pageMainSourceBtn, SIGNAL(clicked()), this, SLOT(pageMainSourceBtnClicked()));
    connect(ui->pageUnnecessaryBtn, SIGNAL(clicked()), this, SLOT(pageUnnecessaryBtnClicked()));
    connect(ui->updateBtn, SIGNAL(clicked()), this, SLOT(updateBtnClicked()));
    connect(ui->setSourceBtn, SIGNAL(clicked()), this, SLOT(setSourceBtnClicked()));
    connect(ui->changeBtn, SIGNAL(clicked()), this, SLOT(changeBtnClicked()));
    connect(ui->createSourceBtn, SIGNAL(clicked()), this, SLOT(createSourceBtnClicked()));
    connect(ui->stopBtn, SIGNAL(clicked()), this, SLOT(stopBtnClicked()));
    connect(ui->deleteSourcePageBtn, SIGNAL(clicked()), this, SLOT(deleteSourcePageBtnClicked()));
    connect(ui->testingUpdateBtn, SIGNAL(clicked()), this, SLOT(testingUpdateBtnClicked()));


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

    searchSourcesNumber();//搜查主源个数
    searchUnnecessarySourcesNumber();//搜查次源个数
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_3->setCurrentIndex(1);
    ui->sourceControlWidget->hide();
    ui->createSourceBtn->hide();
    ui->deleteSourcePageBtn->hide();
    ui->page_3->hide();

    connect(sourceinterface,SIGNAL(downloadover(QString ,QListWidget *, int )),this,SLOT(downloadOverSlot(QString ,QListWidget *, int )));
    connect(sourceinterface,SIGNAL(downloadspeed(QString ,QListWidget *, int )),this,SLOT(downloadspeed(QString ,QListWidget *, int )));
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

    Num++;
    while(Num != Listwidget->count())
    {
        sourceInformationWidget* pwig1 = static_cast<sourceInformationWidget*> (Listwidget->itemWidget(Listwidget->item(Num)));
        qDebug()<<pwig1->ui->address_Label->text()<<"~~~~~~~~~~~~~~~~~~~~~";
        if(!pwig1->ui->delay_Label->text().compare("无法连接") == 0)
        {
            sourceinterface->getDownloadSpeedFromSource(pwig1->ui->address_Label->text(), Listwidget, Num);
            break;
        }
        else
        {
            pwig1->ui->spend_Label->setText("无法连接");
        }
        qDebug()<<Num  << Listwidget->count()-1;
        Num++;
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
    qDebug()<<mainWidget->objectName();
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(mainSageCount+1);
    ui->sourceControlWidget->hide();
    selectWidget = mainWidget;
    ui->label->setText(mainWidget->objectName());
    ui->updateLabel->setText("");
    ui->setSourceBtn->show();
    ui->createSourceBtn->hide();
    ui->deleteSourcePageBtn->hide();
    ui->testingUpdateBtn->show();
}

//次源页切换
void sourceManager::pageUnnecessaryBtnClicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
    ui->stackedWidget_3->setCurrentIndex(UnnecessarySageCount+1);
    ui->sourceControlWidget->show();
    selectWidget = UnnecessaryWidget;
    ui->label->setText(UnnecessarySourceName);
    ui->updateLabel->setText("");
    ui->setSourceBtn->hide();
    ui->createSourceBtn->show();
    ui->deleteSourcePageBtn->show();
    ui->testingUpdateBtn->hide();
}

//搜查主源个数,添加按钮，页面，及listwidget
void sourceManager::searchSourcesNumber()
{
    mainSourceList = sourceinterface->getMainSourceName();
    pageMainNum = mainSourceList.count();
    mainSourceName = mainSourceList.at(0); //第一次副源页切换，当前页名字
    ui->label->setText(mainSourceList.at(0));

    QWidget * page[1024];
    QListWidget * WidgetList[1024];
    QPushButton *unnecessarySourceBtn[1024];

    for(int i=0; i<pageMainNum; i++)
    {
        page[i] = new QWidget();
        unnecessarySourceBtn[i] = new QPushButton();
        ui->horizontalLayout->addWidget(unnecessarySourceBtn[i]);
        ui->stackedWidget->addWidget(page[i]);
        QVBoxLayout *verticalLayout = new QVBoxLayout(page[i]);
        unnecessarySourceBtn[i]->setText(mainSourceList.at(i));

        WidgetList[i] = new QListWidget(page[i]);
        WidgetList[i]->setObjectName(mainSourceList.at(i));
        ui->verticalLayout_4->addWidget(WidgetList[i]);
        verticalLayout->addWidget(WidgetList[i]);
        connect(unnecessarySourceBtn[i], &QPushButton::clicked, this, [=]()
        {
            ui->stackedWidget->setCurrentIndex(i+1);
            ui->label->setText(mainSourceList.at(i));
            selectWidget = WidgetList[i];
            mainWidget = WidgetList[i];
            mainSourceName = SourceList.at(i);
            mainSageCount = i;
            ui->updateLabel->setText("");
        } );

        selectWidget = WidgetList[0];
        mainWidget = WidgetList[0];
        qDebug()<<WidgetList[0]->objectName();
        qDebug()<<mainSourceList.at(i);
        showMainSource(WidgetList[i], i);
    }
}

//搜查次源个数,添加按钮，页面，及listwidget
void sourceManager::searchUnnecessarySourcesNumber()
{
    SourceList = sourceinterface->getSourceName();
    pageNum = SourceList.count();
    UnnecessarySourceName = SourceList.at(0); //第一次副源页切换，当前页名字
    qDebug()<<SourceList;

    QWidget * page[1024];
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
        connect(unnecessarySourceBtn[i], &QPushButton::clicked, this, [=]()
        {
            ui->stackedWidget_3->setCurrentIndex(i+1);
            ui->label->setText(SourceList.at(i));
            selectWidget = WidgetList[i];
            UnnecessaryWidget = WidgetList[i];
            UnnecessarySourceName = SourceList.at(i);
            UnnecessarySageCount = i;
            ui->updateLabel->setText("");
            seletcPage = page[i];
            selectBtn = unnecessarySourceBtn[i];
            qDebug()<<i+1<<"~~~~~~~~~~~~~~~~~~~~~~~~~~";
        } );
        connect(WidgetList[i],SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(selectDeleteIteam(QListWidgetItem*)));
        UnnecessaryWidget = WidgetList[0];
        qDebug()<<SourceList.at(i);
        showUnnecessarySource(WidgetList[i], i);
    }
}

//页面切换回调
void sourceManager::SourceBtnClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->label->setText("sources.list");
    selectWidget = ui->listWidget;
}

//主源展示页回调
void sourceManager::showMainSource(QListWidget *listWidget, int num)
{
    QStringList adddresslist = sourceinterface->getSourceAddressList("/etc/apt/mainsources/"+mainSourceList.at(num));
    QStringList typeList= sourceinterface->getSourceTypeList("/etc/apt/mainsources/"+mainSourceList.at(num));

    initializationList(listWidget,adddresslist.count()+1);//初始化列表框
    fillInTheData(listWidget, adddresslist, typeList); //填充地址，类型，数据
    fillInTheDynamicData(listWidget, adddresslist.count());

}

//次源展示页回调
void sourceManager::showUnnecessarySource(QListWidget *listWidget, int num)
{
    QStringList adddresslist = sourceinterface->getSourceAddressList("/etc/apt/sources.list.d/"+SourceList.at(num));
    QStringList typeList= sourceinterface->getSourceTypeList("/etc/apt/sources.list.d/"+SourceList.at(num));

    initializationList(listWidget,adddresslist.count()+1);//初始化列表框
    fillInTheData(listWidget, adddresslist, typeList); //填充地址，类型，数据
    fillInTheDynamicData(listWidget, adddresslist.count());

}

//初始化列表框
void sourceManager::initializationList(QListWidget *ListWidget, int sourseNum)
{
    QScroller::grabGesture(ListWidget,QScroller::LeftMouseButtonGesture);
    ListWidget -> setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    for (int i = 0; i < sourseNum; i++)
    {
        QListWidgetItem *aItem =new QListWidgetItem;
        aItem->setSizeHint(QSize(50, 40));
        ListWidget->addItem(aItem);
        widget[i] = new sourceInformationWidget(ListWidget);
        ListWidget->setItemWidget(aItem,widget[i]);
    }
}

//填充静态数据
void sourceManager::fillInTheData(QListWidget *ListWidget,
                                  QStringList addressList,
                                  QStringList typeList)
{
    for(int i = 1; i < addressList.count()+1; i++){
        QListWidgetItem *item = ListWidget->item(i);
        sourceInformationWidget* pwig = static_cast<sourceInformationWidget*> (ListWidget->itemWidget(item));
        pwig->ui->address_Label->setText(addressList.at(i-1));
        pwig->ui->spend_Label->setText("等待检测");
        QString type = typeList.at(i-1);
        if(type.compare("deb") == 0)
        {
            pwig->ui->type_Label->setText("软件源");
        }
        else
        {
            pwig->ui->type_Label->setText("源码源");
        }
    }
}

//填充动态数据延时
void sourceManager::fillInTheDynamicData(QListWidget *ListWidget, int num)
{
    for(int i = 1; i < num+1; i++){
        QListWidgetItem *item = ListWidget->item(i);
        sourceInformationWidget* pwig = static_cast<sourceInformationWidget*> (ListWidget->itemWidget(item));
        delayRefreshThread *thread = new delayRefreshThread(pwig->ui->spend_Label, pwig->ui->delay_Label, pwig->ui->address_Label->text());
        thread->start();
    }
}

//刷新按钮回调
void sourceManager::refreshBtnClicked()
{
    Nettestflag = true;
    qDebug()<<Nettestflag;
    qDebug()<<"~~~~~~~~刷新~~~~~~~~~~~~~~~~~:"<<selectWidget->objectName();
    fillInTheDynamicData(selectWidget, selectWidget->count()-1);
    //激活循序下载测速
    for(int i = 1; i < selectWidget->count(); i++)
    {
        sourceInformationWidget* pwig = static_cast<sourceInformationWidget*> (selectWidget->itemWidget(selectWidget->item(i)));
        if(!pwig->ui->delay_Label->text().compare("无法连接") == 0)//跳过无法连接的源
        {
            sourceinterface->getDownloadSpeedFromSource(pwig->ui->address_Label->text(), selectWidget, i);
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
    if(pwig->ui->address_Label->text().compare("address") == 0)
    {
        deleteFlag = 0;
        return;
    }
    delete_item = item;
    deleteFlag = 1;
    changeFlag = 1;
    ui->label_2->setText(pwig->ui->address_Label->text());
    qDebug() << "address : " << pwig->ui->address_Label->text() ;
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

    QStringList Address = pwig->ui->address_Label->text().split(" ");
    sourceChange << QVariant::fromValue(pwig->ui->address_Label->text());//原数据
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

    sourceChange << QVariant::fromValue(addSourcewidget->ui->preview_lineEdit->text());//新数据
    sourceChange << QVariant::fromValue("/etc/apt/sources.list.d/"+selectWidget->objectName());//文件
    qDebug()<<"xxxxxxxxxxx"<<sourceChange;
    serviceInterface->asyncCall("changedSource", sourceChange);
    delete delete_item;

    addForListwidget(selectWidget, addSourcewidget->ui->preview_lineEdit->text());
    changeFlag = 0;
    ui->label_2->setText("");
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
                            << QVariant::fromValue(pwig->ui->address_Label->text());
            }
            else
            {
                sourceDelete<< QVariant::fromValue("/etc/apt/sources.list.d/"+selectWidget->objectName())
                            << QVariant::fromValue(pwig->ui->address_Label->text());
            }

            serviceInterface->asyncCall("deleteSource", sourceDelete);
            delete delete_item;
            deleteFlag = 0;
            ui->label_2->setText("");
        }else if (reply == QMessageBox::No){
        }else{}
    }
}

//添加源按钮回调
void sourceManager::addBtnClicked()
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
    QVariantList sourceDelete;
    addSource *addSourcewidget = new addSource();
    addSourcewidget->exec();
    if(addSourcewidget->ui->preview_lineEdit->text().isEmpty())
    {
        return ;
    }
    if(!addSourcewidget->isAddBtnClicked)
    {
        return;
    }
    QString sourceinfo = addSourcewidget->ui->preview_lineEdit->text();

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
    aItem->setSizeHint(QSize(50, 40));
    ListWidget->addItem(aItem);
    sourceInformationWidget *widget = new sourceInformationWidget(ListWidget);
    ListWidget->setItemWidget(aItem,widget);
    QStringList Address = address.split(" ");
    widget->ui->address_Label->setText(address);
    QString type = Address.at(0);
    if(type.compare("deb") == 0){
        widget->ui->type_Label->setText("软件源");
    }else{
        widget->ui->type_Label->setText("源码源");
    }
    widget->ui->delay_Label->setText("等待检测");
    widget->ui->spend_Label->setText("等待检测");
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

        QWidget *page = new QWidget();
        QPushButton *unnecessarySourceBtn = new QPushButton();
        ui->horizontalLayout_3->addWidget(unnecessarySourceBtn);
        ui->stackedWidget_3->addWidget(page);
        QVBoxLayout *verticalLayout = new QVBoxLayout(page);
        unnecessarySourceBtn->setText(name);

        QListWidget *WidgetList = new QListWidget(page);
        WidgetList->setObjectName(name);
        ui->verticalLayout_3->addWidget(WidgetList);
        verticalLayout->addWidget(WidgetList);
        int i = pageNum;
        connect(unnecessarySourceBtn, &QPushButton::clicked, this, [=]()
        {
            ui->stackedWidget_3->setCurrentIndex(i+1);
            ui->label->setText(name);
            selectWidget = WidgetList;
            UnnecessaryWidget = WidgetList;
            UnnecessarySourceName = name;
            UnnecessarySageCount = i;
            ui->updateLabel->setText("");
            seletcPage = page;
            selectBtn = unnecessarySourceBtn;
            qDebug()<<i+1<<"~~~~~~~~~~~~~~~~~~~~~~~~~~";
        } );
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
        initializationList(WidgetList,+1);//初始化列表框
        pageNum++;
    }
}

void sourceManager::stopBtnClicked()
{
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
}

//检测源是否为最新
void sourceManager::testingUpdateBtnClicked()
{
//    testingUpdateThread *testingupdatethread = new testingUpdateThread(selectWidget);
//    testingupdatethread->start();
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
