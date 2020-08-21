#ifndef SOURCEINFORMATIONWIDGET_H
#define SOURCEINFORMATIONWIDGET_H

#include <QWidget>

namespace Ui {
class sourceInformationWidget;
}

class sourceInformationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit sourceInformationWidget(QWidget *parent = nullptr);
    ~sourceInformationWidget();
    Ui::sourceInformationWidget *ui;

private:

};

#endif // SOURCEINFORMATIONWIDGET_H
