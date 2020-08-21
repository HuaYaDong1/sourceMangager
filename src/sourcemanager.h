#ifndef SOURCEMANAGER_H
#define SOURCEMANAGER_H

#include <QWidget>
#include <QStringList>
#include <QListWidgetItem>
#include <QMessageBox>

class sourceInterface;
class sourceInformationWidget;
QT_BEGIN_NAMESPACE
namespace Ui { class sourceManager; }
QT_END_NAMESPACE

class sourceManager : public QWidget
{
    Q_OBJECT

public:
    sourceManager(QWidget *parent = nullptr);
    ~sourceManager();

private slots:
    void showMainSource(QListWidget *listWidget);
    void showUnnecessarySource(QListWidget *listWidget, int num);
    void selectDeleteIteam(QListWidgetItem *item);
    void deleteBtnClicked();
    void SourceBtnClicked();
    void refreshBtnClicked();
    void addBtnClicked();
    void addForListwidget(QListWidget *ListWidget, QString address);

private:
    Ui::sourceManager *ui;
    void initializationList(QListWidget *ListWidget, int sourseNum);
    void fillInTheData(QListWidget *ListWidget,
                       QStringList addressList,
                       QStringList typeList);
    void fillInTheDynamicData(QListWidget *ListWidget, int num);
    void questionMessage();
    void searchSourcesNumber();

    sourceInformationWidget *widget[1024];

    QWidget * page[1024];
    QListWidget * WidgetList[1024];
    QPushButton *unnecessarySourceBtn[1024];

    QListWidgetItem *delete_item;
    QListWidget  *selectWidget;
    sourceInterface  *sourceinterface;
    QStringList SourceList ;
    int deleteFlag;
    int pageNum;
    QString spend;

signals:

};
#endif // SOURCEMANAGER_H
