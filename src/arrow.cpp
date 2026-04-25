
#include "../include/arrow.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <qmath.h>
#include <QPen>
#include <QPainter>

//! [0]
Arrow::Arrow(DiagramItem *startItem, DiagramItem *endItem , QString startPolar, QString endPolar, QMenu *contextMenu, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    myStartItem = startItem;
    myEndItem = endItem;
    myStartPolar=startPolar;
    myEndPolar=endPolar;
      myContextMenu = contextMenu;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    //setFlags(ItemIsSelectable | ItemIsMovable);
    //setAcceptHoverEvents(true);
    myBorderColor = Qt::black;
    setPen(QPen(myBorderColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
     f.setPointSize(15);
     f.setBold(true);
}
//! [0]
void Arrow::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
//! [1]
QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//! [1]

//! [2]
QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}
//! [2]

//! [3]
void Arrow::updatePosition()
{
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);
}
//! [3]

//! [4]
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
          QWidget *)
{
   // painter->setPen(QPen(myColor, 1, Qt::SolidLine));
///qDebug()<<option->state <<QStyle::State_MouseOver;
    if (myStartItem->collidesWithItem(myEndItem))
        return;

    QPen myPen = pen();
    myPen.setColor(Qt::black);     // Kalem rengi
    myPen.setWidth(2);             // Kalem kalınlığı

    myPen.setColor(myTextColor);
    qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setFont(f);
    answerLabel.setText(answer);
    painter->drawStaticText(QPoint(this->boundingRect().left()+this->boundingRect().width()/2+5,this->boundingRect().center().y()-25), answerLabel);
    // 3️⃣ Çiz
    painter->setBrush(myBorderColor);
    QPointF intersectPointEndPoint;


     QPointF intersectPointStartPoint;


     /*****************************************************/
    int wStart=0,wEnd=0;
     if(myStartItem->myDiagramType==Diagram::DiagramType::Input)
     {
         wStart=15;
     }
     if(myEndItem->myDiagramType==Diagram::DiagramType::Input)
     {
         wEnd=15;
     }
     /*****************************************************/

     if(this->myStartPolar=="left")
         intersectPointStartPoint=myStartItem->mapToScene(myStartItem->boundingRect().left()+wStart,myStartItem->boundingRect().center().y());
     if(this->myStartPolar=="right")
         intersectPointStartPoint=myStartItem->mapToScene(myStartItem->boundingRect().right()-wStart,myStartItem->boundingRect().center().y());
         if(this->myStartPolar=="start")
         intersectPointStartPoint=myStartItem->mapToScene(myStartItem->boundingRect().center().x(),myStartItem->boundingRect().top());
     if(this->myStartPolar=="end")
         intersectPointStartPoint=myStartItem->mapToScene(myStartItem->boundingRect().center().x(),myStartItem->boundingRect().bottom());

     if(this->myEndPolar=="left")
         intersectPointEndPoint=myEndItem->mapToScene(myEndItem->boundingRect().left()+wEnd,myEndItem->boundingRect().center().y());
     if(this->myEndPolar=="right")
         intersectPointEndPoint=myEndItem->mapToScene(myEndItem->boundingRect().right()-wEnd,myEndItem->boundingRect().center().y());
     if(this->myEndPolar=="start")
         intersectPointEndPoint=myEndItem->mapToScene(myEndItem->boundingRect().center().x(),myEndItem->boundingRect().top());
     if(this->myEndPolar=="end")
         intersectPointEndPoint=myEndItem->mapToScene(myEndItem->boundingRect().center().x(),myEndItem->boundingRect().bottom());
     //  auto start = diagramItem->mapToScene(diagramItem->boundingRect().center());

   //  qDebug()<<intersectPointStartPoint<<intersectPointEndPoint;

     setLine(QLineF(intersectPointEndPoint, intersectPointStartPoint));


  //! [5] //! [6]

    double angle = std::atan2(-line().dy(), line().dx());

    QPointF arrowP1 = line().p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                    cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                    cos(angle + M_PI - M_PI / 3) * arrowSize);

  arrowHead.clear();
    arrowHead << line().p1() << arrowP1 << arrowP2;

//! [6] //! [7]
    painter->drawLine(line());
    painter->drawPolygon(arrowHead);
    if (isSelected()) {
       int aci=qFabs(angle/(1.5708/90))/10;
       painter->setPen(QPen(myBorderColor, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 8.0);
        painter->drawLine(myLine);
        myLine.translate(0,-16.0);
        painter->drawLine(myLine);

       // qDebug()<<"angel"<<aci;
    }
}

void Arrow::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(answer=="Y"){answer="N";update();return;}
    if(answer=="N"){answer="Y";update();return;}

    /*bool ok;
        QString text = QInputDialog::getText(0, "Input dialog",
                                            "Date of Birth:", QLineEdit::Normal,
                                            "", &ok);
       if (ok && !text.isEmpty()) {
           QDate date = QDate::fromString(text);
           QString age = text;
           QMessageBox::information (0, "The Age",
                                     QString("The age is %1").arg(age));
       }*/
}
//! [7]

