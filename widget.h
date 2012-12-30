#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <myitem1.h>

namespace Ui {
class Widget;
}
class myItem;
class sizeIndicator;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QGraphicsScene* scene;

    myItem* item;
    myItem1* item2;
public slots:
    void itemChanged();
    void setFactor();

    void dataNeedUpdate();
private slots:

    void on_pushButton_clicked();
};

class myItem:public QGraphicsItem
{

public:
    enum type{up ,down,latral};
    explicit myItem(QGraphicsView* v,QGraphicsItem * parent = 0);

    void refreshData();

    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void refreshGeo();

    QGraphicsView* view;
    myItem& operator <<(QPointF);
    float upData;
    float bottomData;
    float heightData;

    sizeIndicator* upLong;
    sizeIndicator* bottomLong;
    sizeIndicator* height;
    QVector<QPointF> vec;
    type tt;

    void respsonToChang();
    void caculat();
protected:
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);
};

class sizeIndicator: public QGraphicsItem
{
public:
    enum direction{upT,bottomT,vertical};
    explicit sizeIndicator(QGraphicsItem* parent=0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect()const;

    void setPoint(const QPointF& p1, const QPointF& p2, direction dic);

    void setEdit(const bool tmp)
    {isEdit=tmp;}
    float getLineLong()const{return lineLong;}

    QGraphicsView* view;

    void setLineLong();
 private:
    QPointF point;

    float lineLong;

    bool isEdit;
    QGraphicsTextItem* text;
};
#endif // WIDGET_H
