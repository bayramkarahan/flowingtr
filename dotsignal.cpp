/*****************************************************************************
 *   Copyright (C) 2020 by Bayram KARAHAN                                    *
 *   <bayramk@gmail.com>                                                     *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 3 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .          *
 *****************************************************************************/

#include "dotsignal.h"
#include <QBrush>
#include <QColor>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
DotSignal::DotSignal(QGraphicsItem *parentItem, QObject *parent) :
    QObject(parent)
{
    setParentItem(parentItem);
    setAcceptHoverEvents(true);
    setBrush(QBrush(QColor(255,0,0,255)));
   setRect(-4,-4,4,4);
    // setRect(0,0,20,20);
    setDotFlags(0);
    QGraphicsItem *item = qgraphicsitem_cast<DotSignal *>(this);

   // qDebug()<<"Nokta oluşturuldu"<<item;
}

DotSignal::DotSignal(QPointF pos, QGraphicsItem *parentItem, QObject *parent) :
    QObject(parent)
{
    setParentItem(parentItem);
    setAcceptHoverEvents(true);
    setBrush(QBrush(QColor(255,0,0,255)));
    //setRect(0,0,20,20);
    setRect(-4,-4,4,4);
    setPos(pos);
    setPreviousPosition(pos);
    setDotFlags(0);
}

DotSignal::~DotSignal()
{

}

QPointF DotSignal::previousPosition() const
{
    return m_previousPosition;
}

void DotSignal::setPreviousPosition(const QPointF previousPosition)
{
    if (m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged();
}

void DotSignal::setDotFlags(unsigned int flags)
{
    m_flags = flags;
}

void DotSignal::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//qDebug() <<"köşeye tıklandı ";
    if(m_flags & Movable){
        auto dx = event->scenePos().x() - m_previousPosition.x();
        auto dy = event->scenePos().y() - m_previousPosition.y();
        moveBy(dx,dy);
        setPreviousPosition(event->scenePos());
        emit signalMove(this, dx, dy);
    } else {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void DotSignal::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() <<"köşeye tıklandı ";
    if(m_flags & Movable){
        setPreviousPosition(event->scenePos());
    } else {
        QGraphicsItem::mousePressEvent(event);

    }
}

void DotSignal::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   // qDebug() <<"köşeye tıklandı ";
    emit signalMouseRelease();
    QGraphicsItem::mouseReleaseEvent(event);
}

void DotSignal::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
 //qDebug() <<"köşe-enter "<<m_flags;
    setBrush(QBrush(Qt::red));

}

void DotSignal::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
  //  qDebug() <<"köşe "<<m_flags;
     setBrush(QBrush(Qt::black));

   // QGraphicsItem::hoverEnterEvent(event);

}

void DotSignal::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    // fill the box with solid color, use sharp corners
     _outterborderPen.setCapStyle(Qt::SquareCap);
    _outterborderPen.setStyle(Qt::SolidLine);
    //_outterborderPen.setColor(QColor(0,0,0,50));
    _outterborderPen.setWidth(1);

    //painter->setPen(_outterborderColor);

    QPointF topLeft (0, 0);
    QPointF bottomRight ( 10, 10);//dikkat bura ayarlanacak

    QRectF rect (topLeft, bottomRight);
    if (renkdrm)
         _outterborderPen.setColor(QColor(255,0,0,150));
        else
         _outterborderPen.setColor(QColor(0,0,0,20));

 painter->setPen(_outterborderPen);
    painter->drawRoundRect(rect,90,90);

   // QPixmap p1(":/icons/erase.png");
    //QPixmap p2(":/icons/arrow-up-down.png");
    //QPixmap p3(":/icons/rotate-right.png");
/*
if(_outterborderColor!=Qt::red&&m_flags==5) {
    _outterborderColor=QColor(0,255,0);
 //   painter->drawPixmap(-5,-5,p3.scaled(20,20,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
}

if(_outterborderColor!=Qt::red&&m_flags==2)
{
   // _outterborderColor=QColor(255,255,0);
   // painter->drawPixmap(-5,-5,ptt.scaled(20,20,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
}

if(_outterborderColor!=Qt::red&&m_flags==6)
{
    _outterborderColor=QColor(0,0,255);
  //  painter->drawPixmap(-5,-5,p1.scaled(20,20,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
}
if(_outterborderColor!=Qt::red&&m_flags==4)
{
    _outterborderColor=QColor(0,0,0);
   // painter->drawPixmap(-5,-5,p2.scaled(20,20,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
}
if(_outterborderColor!=Qt::red&&m_flags==7) {
    _outterborderColor=QColor(0,0,0);
   // painter->drawPixmap(-5,-5,p2.scaled(20,20,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
}
*/
    //QBrush brush (Qt::SolidPattern);
    //brush.setPen(_outterborderColor);
    //painter->fillRect(rect,brush);


}

