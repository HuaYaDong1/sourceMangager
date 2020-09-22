#ifndef SELETEFRAME_H
#define SELETEFRAME_H

#include <QFrame>
#include <QPainter>
#include <QStyleOption>

namespace Ui {
class seleteFrame;
}

class seleteFrame : public QFrame
{
    Q_OBJECT

public:
    explicit seleteFrame(QWidget *parent = nullptr);
    ~seleteFrame();

    void paintEvent(QPaintEvent *event);

private:
    Ui::seleteFrame *ui;
};

#endif // SELETEFRAME_H
