#include "widget.h"
#include "ui_widget.h"
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <qdebug.h>
#include <QRegExp>
#include <QMessageBox>
#include <QScrollBar>
#include <math.h>
#include <QIntValidator>
#include <QTextDocument>

QGraphicsView* viewGlobal=0;
myItem* myTmp=0;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    scene=new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(0,0,500,500);

    ui->graphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewGlobal=ui->graphicsView;


    item=new myItem(ui->graphicsView);
    myTmp=item;
    scene->addItem(item);
    item2=new myItem1;
    QGraphicsScene* scene2=new QGraphicsScene;
    ui->graphicsView_2->setScene(scene2);
    ui->graphicsView_2->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->graphicsView_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene2->addItem(item2);

    ui->lineEdit->setValidator(new QIntValidator);
    ui->lineEdit->setText("1");

   // setWindowIcon(QIcon(":/txt.png"));
    setWindowTitle(tr("圆台计算"));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::itemChanged()
{
    item->respsonToChang();
}
void Widget::setFactor()
{
    float max=qMax(ui->graphicsView->geometry().width(),ui->graphicsView->geometry().height());

}
void Widget::on_pushButton_clicked()
{
    item->upLong->setLineLong();
    item->bottomLong->setLineLong();
    item->height->setLineLong();
    item->respsonToChang();
    dataNeedUpdate();

    float factor=qMin(ui->graphicsView_2->rect().width(),ui->graphicsView_2->rect().height())\
            /qMax(item2->boundingRect().width(),item2->boundingRect().height())*0.9;
    ui->graphicsView_2->setMatrix(QMatrix());
    ui->graphicsView_2->scale(factor,factor);



}
void Widget::dataNeedUpdate()
{
    item2->setData(0,item->upData);
    item2->setData(1,item->bottomData);
    item2->setData(2,item->heightData);
    item2->setData(3,ui->lineEdit->text().toInt());

    QByteArray byt= item2->caculate();
    QDataStream str(&byt,QIODevice::ReadOnly);
    ui->listWidget->clear();
    QStringList list;
    list<<tr("小半径:")<<tr("大半径:")<<tr("角度:")<<tr("显示角度");
    for(int i=0;i<4;i++){
        float tmp;
        str>>tmp;
        ui->listWidget->addItem(new QListWidgetItem(list[i]+QString::number(tmp)));
    }
}
myItem::myItem(QGraphicsView *v, QGraphicsItem *parent):QGraphicsItem(parent),view(v)
{
    upLong=new sizeIndicator(this);
    bottomLong=new sizeIndicator(this);
    height=new sizeIndicator(this);
    respsonToChang();
}
void myItem::respsonToChang()
{
    refreshData();
    refreshGeo();
}
void myItem::refreshData()
{
    upData=upLong->getLineLong();
    bottomData=bottomLong->getLineLong();
    heightData=height->getLineLong();
    qDebug()<<upData<<bottomData<<heightData;
}
void myItem::refreshGeo()
{
    vec.clear();
    vec<<QPointF(-bottomData/2,heightData/2)<<QPointF(bottomData/2,heightData/2)<<QPointF(upData/2,-heightData/2)\
      <<QPointF(-upData/2,-heightData/2);

    qDebug()<<vec;
    float max=qMax(qMax(upData,bottomData),heightData);
    float viewMax=qMax(view->geometry().width(),view->geometry().height());
    float factor=viewMax/max*0.6;

    QMatrix matri;
    view->setMatrix(matri);
    view->scale(factor,factor);

    setPos(view->mapToScene(view->rect().center()));

    qDebug()<<view->rect().center()<<pos();
    upLong->setPoint(vec[3],vec[2],sizeIndicator::upT);
    bottomLong->setPoint(vec[0],vec[1],sizeIndicator::bottomT);

    height->setPoint(vec[2],vec[1],sizeIndicator::vertical);
}
bool myItem::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    if(qgraphicsitem_cast<QGraphicsTextItem*>(watched)!=0&&event->type()==QEvent::FocusOut){
        respsonToChang();
    }
    qDebug()<<"into here-----------------";
    return false;
}
myItem& myItem::operator <<(QPointF p)
{
    vec<<p;
    return *this;
}

QRectF myItem::boundingRect()const
{
    float longTmp=qMax(upData,bottomData);

    QRectF rec= QRectF(-longTmp/2,-heightData/2,longTmp,heightData);
    rec.adjust(-5,-5,5,5);
    return rec;
}
void myItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QPainterPath path;

    path.addPolygon(vec);
    path.closeSubpath();

    painter->setBrush(Qt::red);

    painter->drawPath(path);

}
//------------------------------------------------------------------------------------------
sizeIndicator::sizeIndicator(QGraphicsItem *parent):QGraphicsItem(parent)
{

    text=new QGraphicsTextItem(this);

    text->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    text->setPlainText("10");
    text->setTextInteractionFlags(Qt::TextEditorInteraction);
    setLineLong();

    setEdit(false);
    installSceneEventFilter(myTmp);
    text-> installSceneEventFilter(myTmp);
}
void sizeIndicator::setLineLong()
{
    lineLong=text->toPlainText().toFloat();
}
void sizeIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath path;
    path.moveTo(0,-10);
    path.lineTo(0,10);
    path.moveTo(0,0);
    path.lineTo(point);
    path.moveTo(point.x(),-10);
    path.lineTo(point.x(),10);

    painter->drawPath(path);
}
QRectF sizeIndicator::boundingRect()const
{
    QRectF rect(0,-20,point.x(),40);
    rect.adjust(-5,-5,5,5);
    return rect;

}
void sizeIndicator::setPoint(const QPointF &p1, const QPointF &p2,direction dic)
{
    setMatrix(viewGlobal->matrix().inverted());
    float texWid=qApp->fontMetrics().boundingRect(text->toPlainText()).width();
    float texHeight=qApp->fontMetrics().height();
    float angleTmp=0.0f;
    switch(dic){
    case upT:
    {
        setPos(p1);
        point=mapFromParent(p2);
        text->setPos(point.x()/2-texWid/2,-texHeight-10);
        setPos(mapToParent(QPointF(0,-20)));
    }
        break;
    case bottomT:
    {
        setPos(p1);
        point=mapFromParent(p2);
        text->setPos(point.x()/2-texWid/2,texHeight-10);
        setPos(mapToParent(QPointF(0,20)));
    }
        break;
    case vertical:
    {
        QPointF temp;
        if(p1.x()>p2.x()){
            setPos(p1);
            temp=QPointF(p1.x(),p2.y());

        }else{
            setPos(QPointF(p2.x(),p1.y()));
            temp=p2;

        }
        point=mapFromParent(temp);
        QLineF line1(QPointF(0,0),point);
        angleTmp=line1.angle();
        rotate(-angleTmp);
        point=mapFromParent(temp);
        text->setPos(point.x()/2-texWid/2,texHeight-20);
        setPos(mapToParent(QPointF(0,-20)));
    }
        break;
    default:
        break;
    }
    update();
}
