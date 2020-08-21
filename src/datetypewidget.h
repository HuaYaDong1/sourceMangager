#ifndef DATETYPEWIDGET_H
#define DATETYPEWIDGET_H

#include <QWidget>

namespace Ui {
class dateTypeWidget;
}

class dateTypeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit dateTypeWidget(QWidget *parent = nullptr);
    ~dateTypeWidget();

private:
    Ui::dateTypeWidget *ui;
};

#endif // DATETYPEWIDGET_H
