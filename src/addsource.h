#ifndef ADDSOURCE_H
#define ADDSOURCE_H

#include <QWidget>
#include <QDialog>

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

private slots:
    void on_add_lineEdit_textChanged(const QString &arg1);
    void on_version_lineEdit_textChanged(const QString &arg1);
    void on_suffix_lineEdit_textChanged(const QString &arg1);
    void on_preview_lineEdit_textChanged(const QString &arg1);
    void debStateChanged(int state);
    void debSrcStateChanged(int state);
    void branchStrupdate(int state);
    void versionBoxSel(const QString &text);
    void addBtnClicked();


private:
    QString typeStr;
    QString webStr;
    QString versionStr;
    QString suffixStr;
    QString branchStr;

    QString setSource();


};

#endif // ADDSOURCE_H