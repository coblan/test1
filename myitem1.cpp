#include "myitem1.h"
#include <QDebug>
#include <math.h>
#include <QMessageBox>
#include <QPainter>
#include <QPropertyAnimation>
myItem1::myItem1()
{
    setData(0,0.0f);
    setData(1,0.0f);
    setData(2,0.0f);
    bigR=0;
    smalR=0;
    angle=0;
    pro=new QPropertyAnimation(this,"angle");
    pro->setEasingCurve(QEasingCurve::InOutCirc);
    connect(pro,SIGNAL(valueChanged(QVariant)),this,SLOT(forUpdate()));
}
QRectF myItem1::boundingRect()const
{
    QRectF rec(-bigR,-bigR,2*bigR,2*bigR);
    rec.adjust(-10,-10,10,10);
    return rec;
}
void myItem1::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(bigR==0||angle==0)
        return;
    QRect rec(-bigR,-bigR,bigR*2,bigR*2);
    QPainterPath path1,path2,path3;
    path1.moveTo(smalR,0);
    path1.lineTo(bigR,0);
    path1.arcTo(rec,0,angle);

    rec.setRect(-smalR,-smalR,smalR*2,smalR*2);
    path2.moveTo(smalR,0);
    path2.arcTo(rec,0,angle);
    path3=path2.toReversed();
    path1.lineTo(path2.currentPosition());
    path1.connectPath(path3);

    painter->setBrush(Qt::green);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPath(path1);


}
QByteArray myItem1::caculate()
{
    float r1=data(0).toFloat()/2;
    float r2=data(1).toFloat()/2;
    float h=data(2).toFloat();
    int count=data(3).toInt();
    if(count<1){
        count=1;
        QMessageBox::warning(NULL,tr("警告"),tr("分数不正常，设置为1"));
    }

    float tmp=r2;
    r2=qMax(r2,r1);
    r1=qMin(tmp,r1);

    if(r1>=r2||r2<=0||h<=0){
        QMessageBox::warning(NULL,tr("警告"),tr("半径畸形，不能计算"));
        return QByteArray();
    }

    float x=r1*h/(r2-r1);
    bigR=sqrt(powf((x+h),2)+powf(r2,2));
    smalR=sqrt(powf(r1,2)+powf(x,2));

    float angleTotal=360*r2/bigR;

    angle=angleTotal/count;
    float ang_tmp=angle;
    pro->setDuration(1500);
    pro->setStartValue(0);
    pro->setEndValue(angle);
    pro->start();

   // update();

    QByteArray byt;
    QDataStream str(&byt,QIODevice::WriteOnly);
    str<<smalR<<bigR<<angleTotal<<ang_tmp;
    return byt;
}
