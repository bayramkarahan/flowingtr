
#include "diagramtextitem.h"
#include "DiagramScene.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>

//! [0]
DiagramTextItem::DiagramTextItem(QMenu *contextMenu, QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    myContextMenu = contextMenu;
    myBackgroundColor=QColor(0,0,0,0);
    myBorderColor=QColor(0,0,0,0);
}
//! [0]
void DiagramTextItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
//! [1]
QVariant DiagramTextItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        emit selectedChange(this);
    return value;
}
//! [1]

//! [2]
void DiagramTextItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}
//! [2]

//! [5]
void DiagramTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
//! [5]

void DiagramTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Zemin rengi

    painter->setBrush(myBackgroundColor);

    // Kenar çizgisi rengi ve kalınlığı
    QPen pen(myBorderColor, 2); // 2 px kalınlık
    painter->setPen(pen);

    // Dikdörtgeni çiz
    painter->drawRect(boundingRect());
   // QPen pen1(myPenColor, 2); // 2 px kalınlık
    //painter->setPen(pen1);

    // Varsayılan QGraphicsTextItem metin çizimini yap
    QGraphicsTextItem::paint(painter, option, widget);
}
