#ifndef CREATESOURCEWIDGET_H
#define CREATESOURCEWIDGET_H

#include <QDialog>
#include <QPainter>

namespace Ui {
class createSourceWidget;
}

class createSourceWidget : public QDialog
{
    Q_OBJECT

public:
    explicit createSourceWidget(QWidget *parent = nullptr);
    ~createSourceWidget();
    Ui::createSourceWidget *ui;
    bool isTrueBtnClicked;

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

private:

public slots:
    void trueBtnClicked();
    void cancelBtnClicked();
    void nameLineEdit_textChanged(const QString &arg1);
};

#endif // CREATESOURCEWIDGET_H
