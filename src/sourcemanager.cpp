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
#include "downloadspendrefreshthread.h"


sourceManager::sourceManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::sourceManager)
{
    ui->setupUi(this);
    sourceinterface = new sourceInterface;
    deleteFlag = 0;
    //connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(selectDeleteIteam(QListWidgetItem*)));
    connect(ui->mainSourseBtn, SIGNAL(clicked()), this, SLOT(SourceBtnClicked()) );
    connect(ui->deleteBtn, SIGNAL(clicked()), this, SLOT(deleteBtnClicked()) );
    connect(ui->refreshBtn, SIGNAL(clicked()), this, SLOT(refreshBtnClicked()) );
    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(addBtnClicked()) );

    searchSourcesNumber();//搜查源个数
    showMainSource(ui->listWidget);//主源展示页回调 主源与非固定的次源分开展示refreshBtn
    ui->label->setText("sources.list");
    ui->listWidget->setObjectName(QString::fromUtf8("sources.list"));
    selectWidget = ui->listWidget;
    ui->stackedWidget->setCurrentIndex(0);
    ui->sourceControlWidget->hide();

    connect(sourceinterface,SIGNAL(downloadover(QString ,QListWidget *, int )),this,SLOT(downloadOverSlot(QString ,QListWidget *, int )));
}
sourceManager::~sourceManager()
{
    delete ui;
}

void sourceManager::downloadOverSlot(QString speed,QListWidget *Listwidget, int Num)
{
    sourceInformationWidget* pwig = static_cast<sourceInformationWidget*> (Listwidget->itemWidget(Listwidget->item(Num)));
    if(!pwig->ui->delay_Label->text().compare("N/A") == 0)
    {
        pwig->ui->spend_Label->setText(speed);
    }else{
        pwig->ui->spend_Label->setText("无法连接");
    }

    Num++;
    if(Num != Listwidget->count())
    {
        sourceInformationWidget* pwig1 = static_cast<sourceInformationWidget*> (Listwidget->itemWidget(Listwidget->item(Num)));
        qDebug()<<pwig1->ui->address_Label->text();
        if(!pwig1->ui->delay_Label->text().compare("N/A") == 0)
        {
            sourceinterface->getDownloadSpeedFromSource(pwig1->ui->address_Label->text(), Listwidget, Num);
        }


        qDebug()<<Num  << Listwidget->count()-1;
    }else{
        return ;
    }
}

//搜查源个数,添加按钮，页面，及listwidget
void sourceManager::searchSourcesNumber()
{
    SourceList = sourceinterface->getSourceName();
    pageNum = SourceList.count();
    qDebug()<<SourceList;

    for(int i=0; i<pageNum; i++)
    {
        page[i] = new QWidget();
        unnecessarySourceBtn[i] = new QPushButton();
        ui->horizontalLayout->addWidget(unnecessarySourceBtn[i]);
        ui->stackedWidget->addWidget(page[i]);
        QVBoxLayout *verticalLayout = new QVBoxLayout(page[i]);
        unnecessarySourceBtn[i]->setText(SourceList.at(i));

        WidgetList[i] = new QListWidget(page[i]);
        WidgetList[i]->setObjectName(SourceList.at(i));
        ui->verticalLayout_2->addWidget(WidgetList[i]);
        verticalLayout->addWidget(WidgetList[i]);
        connect(unnecessarySourceBtn[i], &QPushButton::clicked, this, [=]()
        {
            ui->stackedWidget->setCurrentIndex(i+2);
            ui->label->setText(SourceList.at(i));
            selectWidget = WidgetList[i];
            ui->sourceControlWidget->show();
        } );
        connect(WidgetList[i],SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(selectDeleteIteam(QListWidgetItem*)));

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
    ui->sourceControlWidget->hide();
}

//主源展示页回调
void sourceManager::showMainSource(QListWidget *listWidget)
{
    QStringList adddresslist = sourceinterface->getSourceAddressList("/etc/apt/sources.list");
    QStringList typeList= sourceinterface->getSourceTypeList("/etc/apt/sources.list");

    //sourceinterface->getDownloadSpeedFromSource(adddresslist.at(0));

    initializationList(listWidget,adddresslist.count()+1);//初始化列表框
    fillInTheData(listWidget, adddresslist, typeList); //填充地址，类型，数据
    fillInTheDynamicData(listWidget, adddresslist.count());
    //    downloadSpendRefreshThread *thread = new downloadSpendRefreshThread(listWidget, adddresslist.count());
    //    thread->start();
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
        pwig->ui->type_Label->setText(typeList.at(i-1));
    }
}

//填充动态数据延时 //下载速度
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
    qDebug()<<"~~~~~~~~刷新~~~~~~~~~~~~~~~~~:"<<spend;
    fillInTheDynamicData(ui->listWidget, ui->listWidget->count()-1);
    for(int i = 0; i < pageNum; i++)
    {
        fillInTheDynamicData(WidgetList[i], WidgetList[i]->count()-1);
    }
    sourceInformationWidget* pwig = static_cast<sourceInformationWidget*> (selectWidget->itemWidget(selectWidget->item(1)));
    sourceinterface->getDownloadSpeedFromSource(pwig->ui->address_Label->text(), selectWidget, 1);
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
    ui->label_2->setText(pwig->ui->address_Label->text());
    qDebug() << "address : " << pwig->ui->address_Label->text() ;
}

//删除按钮点击回调
void sourceManager::deleteBtnClicked()
{
    questionMessage();//删除按钮前询问
}

//删除按钮前询问
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

//想源文件内添加
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

void sourceManager::addForListwidget(QListWidget *ListWidget, QString address)
{
    QListWidgetItem *aItem =new QListWidgetItem;
    aItem->setSizeHint(QSize(50, 40));
    ListWidget->addItem(aItem);
    sourceInformationWidget *widget = new sourceInformationWidget(ListWidget);
    ListWidget->setItemWidget(aItem,widget);
    QStringList Address = address.split(" ");
    widget->ui->address_Label->setText(address);
    widget->ui->type_Label->setText(Address.at(0));
}
