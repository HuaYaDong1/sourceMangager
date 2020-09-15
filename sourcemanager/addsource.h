#ifndef ADDSOURCE_H
#define ADDSOURCE_H

#include <QWidget>
#include <QDialog>
#include <QMessageBox>
#include <QDir>
#include <QPainter>


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

    void paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
                                                        //Antialiasing
        painter.setBrush(QBrush(QColor(255, 255, 255)));
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


private slots:
    void on_add_lineEdit_textChanged(const QString &arg1);
    void on_version_lineEdit_textChanged(const QString &arg1);
    void on_suffix_lineEdit_textChanged(const QString &arg1);
    void on_preview_lineEdit_textChanged(const QString &arg1);
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
