/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include<QDebug>
#include<QStaticText>
#include<diagram.h>
#include<QLabel>
#include<QTimer>
#include<dotsignal.h>
#include <QObject>
#include<QVBoxLayout>
#include<QComboBox>
#include<QDialogButtonBox>
#include<variable.h>
#include<QListWidget>
#include<QPushButton>
#include<variableprocessdialog.h>
#include<variableinputdialog.h>
#include<variableoutputdialog.h>
#include<variableconditiondialog.h>
#include<variableloopdialog.h>
#include <QTextDocument>
QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;

QT_END_NAMESPACE

class Arrow;

//! [0]
class DiagramItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

public:
    enum { Type = UserType + 15 };
   /// enum DiagramType { Step, Conditional, StartEnd, Io };
    QPointF previousPosition() const;
    void setPreviousPosition(const QPointF previousPosition);
    double scaleFactor;
 enum ActionStates {
    ResizeState = 0x01,
    RotationState = 0x02
};
 enum CornerFlags {
     Top = 0x01,
     Bottom = 0x02,
     Left = 0x04,
     Right = 0x08,
     TopLeft = Top|Left,
     TopRight = Top|Right,
     BottomLeft = Bottom|Left,
     BottomRight = Bottom|Right
 };
    enum CornerGrabbers {
        GrabberTop = 0,
        GrabberBottom,
        GrabberLeft,
        GrabberRight,
        GrabberTopLeft,
        GrabberTopRight,
        GrabberBottomLeft,
        GrabberBottomRight
    };
    DotSignal *cornerGrabber[8];

    DiagramItem(Diagram::DiagramType diagramType, QMenu *contextMenu, QGraphicsItem *parent = 0);

    void removeArrow(Arrow *arrow);
    void removeArrowPolar(Arrow *arrow,QString polar);
    void removeArrows();
    bool addArrowState(Arrow *arrow, QString polar, QString rota);

    QPolygonF polygon() const { return myPolygon; }

    void setText(QString text, QColor color);

    ///QPixmap image() const;
    Diagram::DiagramType diagramType() const { return myDiagramType; }
    int type() const override { return Type;}
    Diagram::DiagramType myDiagramType;
    QList<Arrow *> arrows;
    QColor myBackgroundColor;
    QColor myTextColor;
    QColor myBorderColor;

    QStaticText label;
    QStaticText labelAlgoritma;
    QString labelText;
    QString labelAlgoritmaText;
    Arrow *startArrow;
    Arrow *endArrow;
    Arrow *leftArrow;
    Arrow *rightArrow;
    QString startArrowRota="";
    QString endArrowRota="";
    QString leftArrowRota="";
    QString rightArrowRota="";
    bool loopItemRun=false;
    int myDiagramWidth;
    int myDiagramHeight;
    bool renkdrm;
    // Seçilenler listesi
    QList<VariableRecord> selectedVariables;
    //VariableExpressionDialog dlg;
    QPointF labelPosition;
    QGraphicsTextItem *tempLabel; // geçici label
    QFont f;
  public slots:
      void renk();
signals:
    void rectChanged(DiagramItem *rect);
    void previousPositionChanged();
    void clicked(DiagramItem *rect);
    void signalMove(QGraphicsItem *item, qreal dx, qreal dy);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

private:
    unsigned int m_cornerFlags;
    unsigned int m_actionFlags;

    bool drm;

    bool rotateState=false;
    bool m_leftMouseButtonPressed;
    QPointF m_previousPosition;

    QPolygonF myPolygon;
    QMenu *myContextMenu;

    void setPositionGrabbers();
    void setVisibilityGrabbers();
    void hideGrabbers();

    void resizeLeft( const QPointF &pt);
    void resizeRight( const QPointF &pt);
    void resizeBottom(const QPointF &pt);
    void resizeTop(const QPointF &pt);
    void rotateItem(const QPointF &pt);

};
//! [0]

#endif // DIAGRAMITEM_H
