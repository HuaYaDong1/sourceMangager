#ifndef SELECTFORM_H
#define SELECTFORM_H

#include <QWidget>
#include <QPainter>
#include <QStyleOption>

namespace Ui {
class selectForm;
}

class selectForm : public QWidget
{
    Q_OBJECT

public:
    explicit selectForm(QWidget *parent = nullptr);
    ~selectForm();

    void paintEvent(QPaintEvent *event);

private:
    Ui::selectForm *ui;
};

#endif // SELECTFORM_H
