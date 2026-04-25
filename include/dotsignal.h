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

#ifndef DOTSIGNAL_H
#define DOTSIGNAL_H

#include <QObject>
#include <QGraphicsRectItem>
#include<QPen>
#include<QPainter>
#include<QPainter>
class QGraphicsSceneHoverEventPrivate;
class QGraphicsSceneMouseEvent;

class DotSignal : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

public:
    explicit DotSignal(QGraphicsItem *parentItem = 0, QObject *parent = 0);
    explicit DotSignal(QPointF pos, QGraphicsItem *parentItem = 0, QObject *parent = 0);
    ~DotSignal();

    enum Flags {
        Movable = 0x01
    };

    QPointF previousPosition() const;
    void setPreviousPosition(const QPointF previousPosition);

    void setDotFlags(unsigned int flags);
    unsigned int m_flags;
    bool renkdrm;
signals:
    void previousPositionChanged();
    void signalMouseRelease();
    void signalMove(QGraphicsItem *signalOwner, qreal dx, qreal dy);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

public slots:

private:

    QPointF m_previousPosition;
    QColor _outterborderColor; ///< the hover event handlers will toggle this between red and black
    QPen _outterborderPen; ///< the pen is used to paint the red/black border

};

#endif // DOTSIGNAL_H
