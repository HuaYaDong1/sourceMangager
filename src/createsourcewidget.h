#ifndef CREATESOURCEWIDGET_H
#define CREATESOURCEWIDGET_H

#include <QDialog>

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

private:

public slots:
    void trueBtnClicked();
    void cancelBtnClicked();
};

#endif // CREATESOURCEWIDGET_H
