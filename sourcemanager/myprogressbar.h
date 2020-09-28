#ifndef MYPROGRESSBAR_H
#define MYPROGRESSBAR_H

#include <QWidget>
#include <QPainter>
#include <QStyleOptionComplex>

class myProgressbar : public QWidget
{
    Q_OBJECT
public:
    explicit myProgressbar(QWidget *parent = nullptr);


    void paintEvent(QPaintEvent *event);


    int progressValue = 0;

signals:

};

#endif // MYPROGRESSBAR_H
