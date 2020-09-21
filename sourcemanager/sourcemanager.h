#ifndef SOURCEMANAGER_H
#define SOURCEMANAGER_H

#include <QWidget>
#include <QStringList>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPainter>
#include <QMenu>
#include <QDebug>
#include "setAlarmRepeatDialog.h"

class sourceInterface;
class sourceInformationWidget;
class addSource;
QT_BEGIN_NAMESPACE
namespace Ui { class sourceManager; }
QT_END_NAMESPACE

class sourceManager : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.client.test")

public:
    sourceManager(QWidget *parent = nullptr);
    ~sourceManager();

    void paintEvent(QPaintEvent *event);


private slots:
    void showMainSource(QListWidget *listWidget);
    void showUnnecessarySource(QListWidget *listWidget, int num);
    void selectDeleteIteam(QListWidgetItem *item);
    void changeBtnClicked();
    void deleteBtnClicked();
    void SourceBtnClicked();
    void refreshBtnClicked();
    void addBtnClicked();
    void addForListwidget(QListWidget *ListWidget, QString address);
    void downloadOverSlot(QString  speed, QListWidget *Listwidget, int Num);
    void downloadspeed(QString speed,QListWidget *Listwidget, int Num);
    void pageMainSourceBtnClicked();
    void pageUnnecessaryBtnClicked();
    void updateBtnClicked();
    void updateOverSlot(QString );
    void setSourceBtnClicked();
    void createSourceBtnClicked();
    void stopBtnClicked();
    void deleteSourcePageBtnClicked();
    void testingUpdateBtnClicked();
    void UnnecessarySourcesSelect();
    void UnnecessarySourcesSelect_listClickslot();
    void UnnecessarySourcesShow();

private:
    Ui::sourceManager *ui;
    void initializationList(QListWidget *ListWidget, int sourseNum);
    void fillInTheData(QListWidget *ListWidget,
                       QStringList addressList,
                       QStringList typeList);
    void fillInMainTheData(QListWidget *ListWidget,
                           QStringList addressList);
    void fillInTheDynamicData(QListWidget *ListWidget, int num);
    void fillInMainDynamicData(QListWidget *ListWidget, int num);
    void questionMessage();
    void searchUnnecessarySourcesNumber();
    void mainSourceSet();
    void addMainSource(QString sourceName);
    void delMainSource(QString sourceName);

    sourceInformationWidget *widget[1024];

    QWidget * page[1024];
    //    QListWidget * WidgetList[1024];
    //    QPushButton *unnecessarySourceBtn[1024];

    QListWidgetItem *delete_item;
    QListWidget  *selectWidget;
    sourceInterface  *sourceinterface;
    QStringList SourceList;
    QStringList mainSourceList;
    int deleteFlag;
    int changeFlag;
    int pageNum;
    int pageMainNum;
    QString spend;
    int flag = 1;
    int flag1 = 0;
    int flag2 = 1;
    QString UnnecessarySourceName;
    QString mainSourceName;
    int UnnecessarySageCount;
    int mainSageCount;

    QListWidget * mainWidget;
    QListWidget * UnnecessaryWidget;

    bool Nettestflag;

    QWidget *seletcPage;
    QPushButton *selectBtn;

    int mains_flag = 0;

    QLabel *source[1024];
    QStringList SourceLabelList;

    set_alarm_repeat_Dialog *UnnecessarySources = nullptr;

    QListWidget * WidgetSelect[1024];

signals:

};
#endif // SOURCEMANAGER_H
