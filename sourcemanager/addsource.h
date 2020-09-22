#ifndef ADDSOURCE_H
#define ADDSOURCE_H

#include <QWidget>
#include <QDialog>
#include <QMessageBox>
#include <QDir>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>


namespace Ui {
class addSource;
}

class addSource : public QDialog
{
    Q_OBJECT

public:
    explicit addSource(QWidget *parent = nullptr);
    ~addSource();
    Ui::addSource *ui;
    bool isAddBtnClicked;

    void paintEvent(QPaintEvent *event);



private slots:
    void on_add_lineEdit_textChanged(const QString &arg1);
    void on_version_lineEdit_textChanged(const QString &arg1);
    void on_suffix_lineEdit_textChanged(const QString &arg1);
    void custom_CheckBoxStateChanged(int state);
    void class_CheckBoxStateChanged(int state);
    void debStateChanged(bool state);
    void debSrcStateChanged(bool state);
    void branchStrupdate(int state);
    void versionBoxSel(const QString &text);
    void addBtnClicked();
    void cancelBtnClicked();


private:
    QString typeStr;
    QString webStr;
    QString versionStr;
    QString suffixStr;
    QString branchStr;

    QString setSource();
    QStringList getVersionList(QString fileName);
};

#endif // ADDSOURCE_H
