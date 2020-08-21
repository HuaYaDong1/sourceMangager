#include "sourcemanager.h"
#include "ui_sourcemanager.h"
#include <QScroller>
#include "sourceinformationwidget.h"
#include "ui_sourceinformationwidget.h"
#include <QDebug>
#include "sourceinterface.h"
#include "delayrefreshthread.h"


sourceManager::sourceManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::sourceManager)
{
    ui->setupUi(this);
    sourceinterface = new sourceInterface;
    spend = sourceinterface->getDownloadSpeedFromSource();
    deleteFlag = 0;
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(selectDeleteIteam(QListWidgetItem*)));
    connect(ui->mainSourseBtn, SIGNAL(clicked()), this, SLOT(SourceBtnClicked()) );
    connect(ui->deleteBtn, SIGNAL(clicked()), this, SLOT(deleteBtnClicked()) );
    connect(ui->refreshBtn, SIGNAL(clicked()), this, SLOT(refreshBtnClicked()) );

    searchSourcesNumber();//搜查源个数
    showMainSource(ui->listWidget);//主源展示页回调 主源与非固定的次源分开展示refreshBtn
    ui->label->setText("source.list");
    selectWidget = ui->listWidget;
    ui->stackedWidget->setCurrentIndex(0);
}

sourceManager::~sourceManager()
{
    delete ui;
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
        ui->verticalLayout_2->addWidget(WidgetList[i]);
        verticalLayout->addWidget(WidgetList[i]);
        connect(unnecessarySourceBtn[i], &QPushButton::clicked, this, [=]()
        {
            ui->stackedWidget->setCurrentIndex(i+2);
            ui->label->setText(SourceList.at(i));
            selectWidget = WidgetList[i];
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
    ui->label->setText("source.list");
    selectWidget = ui->listWidget;
}

//主源展示页回调
void sourceManager::showMainSource(QListWidget *listWidget)
{
    QStringList adddresslist = sourceinterface->getSourceAddressList("/etc/apt/sources.list");
    QStringList typeList= sourceinterface->getSourceTypeList("/etc/apt/sources.list");

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
        pwig->ui->type_Label->setText(typeList.at(i-1));
    }
}

//填充动态数据延时 //下载速度
void sourceManager::fillInTheDynamicData(QListWidget *ListWidget, int num)
{
    //spend = "111/kb";
    for(int i = 1; i < num+1; i++){
        QListWidgetItem *item = ListWidget->item(i);
        sourceInformationWidget* pwig = static_cast<sourceInformationWidget*> (ListWidget->itemWidget(item));
        delayRefreshThread *thread = new delayRefreshThread(pwig->ui->spend_Label, pwig->ui->delay_Label, pwig->ui->address_Label->text());
        thread->start();
        pwig->ui->spend_Label->setText(spend);
    }
}

//刷新按钮回调
void sourceManager::refreshBtnClicked()
{
    spend = sourceinterface->getDownloadSpeedFromSource();
    fillInTheDynamicData(ui->listWidget, ui->listWidget->count()-1);
    for(int i = 0; i < pageNum; i++)
    {
        fillInTheDynamicData(WidgetList[i], WidgetList[i]->count()-1);
    }
}

//单击选中item回调
void sourceManager::selectDeleteIteam(QListWidgetItem *item)
{
    spend = sourceinterface->getDownloadSpeedFromSource();
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
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("question"),
                                      tr("delete ?"),
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(reply == QMessageBox::Yes){
            delete delete_item;
            deleteFlag = 0;
        }else if (reply == QMessageBox::No){
        }else{}
    }
}
