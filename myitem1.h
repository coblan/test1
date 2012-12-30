#ifndef MYITEM1_H
#define MYITEM1_H

#include <QGraphicsObject>
#include <QCoreApplication>
#include <QPropertyAnimation>
class myItem1 : public QGraphicsObject
{

    Q_OBJECT
    Q_PROPERTY(double angle READ ang WRITE setAngle)
public:
    myItem1();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QByteArray caculate();



private:
    float bigR;
    float smalR;
    float angle;
    float ang()const{return angle;}
    void setAngle(const double& tmp){angle=tmp;}
    QPropertyAnimation* pro;
private slots:
    void forUpdate(){update();}
};
#endif // MYITEM1_H
