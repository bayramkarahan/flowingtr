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

#include "DiagramScene.h"
#include "arrow.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>

//! [0]
DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = Diagram::Start;
    line = 0;
    textItem = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
}
//! [0]

//! [1]
void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if (isItemChange(Arrow::Type)) {
        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}
//! [1]

//! [2]
void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}
//! [2]

//! [3]
void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}
//! [3]

//! [4]
void DiagramScene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(DiagramTextItem::Type)) {
        QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}
//! [4]

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(Diagram::DiagramType type)
{
    myItemType = type;
}

//! [5]
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}
//! [5]
bool DiagramScene::haveStateItem(Diagram::DiagramType diagramItemType)
{
    bool have=false;
    foreach (QGraphicsItem *item, items()) {
        if (item->type() == DiagramItem::Type)
        {
            if(qgraphicsitem_cast<DiagramItem *>(item)->myDiagramType==diagramItemType)
                have=true;
        }
    }

    return have;
}
//! [6]
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    bool insertState=false;
    DiagramItem *item;
  //  qDebug()<<myMode<<DiagramScene::MoveItem;

    switch (myMode) {
        case InsertItem:
         if(haveStateItem(myItemType)&&myItemType==Diagram::DiagramType::Start) insertState=true;
        if(haveStateItem(myItemType)&&myItemType==Diagram::DiagramType::End) insertState=true;

        if(!insertState)
        {
            item = new DiagramItem(myItemType, myItemMenu);
            item->setBrush(myItemColor);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            if(myItemType==Diagram::DiagramType::Loop)
            {
                ///qDebug()<<"Döngü Ekledin";
                //alt link
                DiagramItem *endLinkItem = new DiagramItem(Diagram::DiagramType::Link, myItemMenu);
                endLinkItem->setBrush(myItemColor);
                addItem(endLinkItem);
                endLinkItem->setPos(mouseEvent->scenePos().x(),mouseEvent->scenePos().y()+150);
                //sol alt link
                DiagramItem *leftEndProcessItem = new DiagramItem(Diagram::DiagramType::Process, myItemMenu);
                leftEndProcessItem->setBrush(myItemColor);
                addItem(leftEndProcessItem);
                leftEndProcessItem->setPos(mouseEvent->scenePos().x()-250,mouseEvent->scenePos().y()+150);
                //sol link
                DiagramItem *leftLinkItem = new DiagramItem(Diagram::DiagramType::Link, myItemMenu);
                leftLinkItem->setBrush(myItemColor);
                addItem(leftLinkItem);
                leftLinkItem->setPos(mouseEvent->scenePos().x()-250,mouseEvent->scenePos().y());

                Arrow *arrow1 = new Arrow(item,endLinkItem,"end","start",myItemMenu);
                arrow1->setColor(myLineColor);
                item->addArrowState(arrow1,"end","O");
                endLinkItem->addArrowState(arrow1,"start","I");
                arrow1->setZValue(-1000.0);
                addItem(arrow1);
                arrow1->updatePosition();

                Arrow *arrow2 = new Arrow(endLinkItem,leftEndProcessItem,"left","right",myItemMenu);
                arrow2->setColor(myLineColor);
                endLinkItem->addArrowState(arrow2,"left","O");
                leftEndProcessItem->addArrowState(arrow2,"right","I");
                arrow2->setZValue(-1000.0);
                addItem(arrow2);
                arrow2->updatePosition();

                Arrow *arrow3 = new Arrow(leftEndProcessItem,leftLinkItem,"start","end",myItemMenu);
                arrow3->setColor(myLineColor);
                leftEndProcessItem->addArrowState(arrow3,"start","O");
                leftLinkItem->addArrowState(arrow3,"end","I");
                arrow3->setZValue(-1000.0);
                addItem(arrow3);
                arrow3->updatePosition();

                Arrow *arrow4 = new Arrow(leftLinkItem,item,"right","left",myItemMenu);
                arrow4->setColor(myLineColor);
                leftLinkItem->addArrowState(arrow4,"right","O");
                item->addArrowState(arrow4,"left","I");
                arrow4->setZValue(-1000.0);
                addItem(arrow4);
                arrow4->updatePosition();

             }

        }
        emit itemInserted(myItemType);
            break;
//! [6] //! [7]
        case InsertLine:

            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;
//! [7] //! [8]
    case InsertText:
        textItem = new DiagramTextItem(myItemMenu);
        textItem->setFont(myFont);
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        textItem->setZValue(1000.0);
        connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
                this, SLOT(editorLostFocus(DiagramTextItem*)));
        connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                this, SIGNAL(itemSelected(QGraphicsItem*)));
        addItem(textItem);
        textItem->setDefaultTextColor(myTextColor);
        textItem->setPos(mouseEvent->scenePos());
        emit textInserted(textItem);
        break;

    case MoveItem:
        //qDebug()<<"seçme modunda";
       /* int boyut=30;
        int xp=mouseEvent->scenePos().x();
        int yp=mouseEvent->scenePos().y();
        QRect rect(QPoint(xp-boyut,yp-boyut),QPoint(xp+boyut,yp+boyut));
        QPainterPath pp;
        pp.addRect(rect);
        setSelectionArea(pp);

        Arrow *ar1;
        foreach (QGraphicsItem *item,selectedItems()) {
            if (item->type() == Arrow::Type)
            {
                ar1=qgraphicsitem_cast<Arrow *>(item);
                ar1->setFlag(QGraphicsItem::ItemIsSelectable);
            }

        }
        QList<QGraphicsItem*> list = items();
           foreach(QGraphicsItem* item, list){
               item->setSelected(true);
              // qDebug()<<"selection: "<<item->isSelected();
           }
        //QTransform deviceTransform;
        //QGraphicsItem *selectItem = itemAt(startItems.first(), deviceTransform);
*/
        break;
        //! [8] //! [9]
   /* default:
        ;*/
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
         QList<QGraphicsItem *> endItems = items(line->line().p2());
         /**************************************************************************************/
         foreach (QGraphicsItem *item, items()) {
             DiagramItem *eleman = qgraphicsitem_cast<DiagramItem *>(item);
             if(eleman!=0)
             {
             eleman->renkdrm=false;
             }
         }
     /****************************************************************************************/
         DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.last());
        if(endItem!=0)
        {
            //qDebug()<<"tür";
            if (endItem->type() == DiagramItem::Type){

                endItem->renkdrm=true;
             }
        }
/****************************************************************************************************/
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
    update();
}
//! [10]

//! [11]

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // Tüm nesnelerin renk bayrağını sıfırla
    for (QGraphicsItem *item : items()) {
        DiagramItem *eleman = qgraphicsitem_cast<DiagramItem *>(item);
        if (eleman)
            eleman->renkdrm = false;
    }

    if (line != nullptr && myMode == InsertLine) {

        // Başlangıç ve bitiş noktaları
        QPointF startPoint = line->line().p1();
        QPointF endPoint   = line->line().p2();

        // Eski sistem yerine, toleranslı arama kullanılıyor
        qreal xTol = 25;
        qreal yTol = 20;
        DiagramItem *startItem = findDiagramItemNear(startPoint, xTol, yTol);
        DiagramItem *endItem   = findDiagramItemNear(endPoint,   xTol, yTol);

        removeItem(line);
        delete line;

        if (startItem && endItem && startItem != endItem) {
            // Yön tespiti
            QString startPolar = polarDiagramItem(startItem, startPoint);
            QString endPolar   = polarDiagramItem(endItem,   endPoint);

            if (!startPolar.isEmpty() && !endPolar.isEmpty()) {
                Arrow *arrow = new Arrow(startItem, endItem, startPolar, endPolar, myItemMenu);
                arrow->setColor(myLineColor);

                bool startAddStatus = startItem->addArrowState(arrow, startPolar, "O");
                bool endAddStatus   = endItem->addArrowState(arrow, endPolar, "I");

                if (startItem->myDiagramType == Diagram::DiagramType::Loop)
                    arrow->answer = "N";

                if (startAddStatus && endAddStatus) {
                    arrow->setZValue(-1000.0);
                    addItem(arrow);
                    arrow->updatePosition();
                } else {
                    // Başarısızsa temizlik yap
                    if (startAddStatus) {
                        if (startPolar == "left")  { startItem->leftArrow = nullptr;  startItem->leftArrowRota = ""; }
                        if (startPolar == "right") { startItem->rightArrow = nullptr; startItem->rightArrowRota = ""; }
                        if (startPolar == "start") { startItem->startArrow = nullptr; startItem->startArrowRota = ""; }
                        if (startPolar == "end")   { startItem->endArrow = nullptr;   startItem->endArrowRota = ""; }
                    }

                    if (endAddStatus) {
                        if (endPolar == "left")  { endItem->leftArrow = nullptr;  endItem->leftArrowRota = ""; }
                        if (endPolar == "right") { endItem->rightArrow = nullptr; endItem->rightArrowRota = ""; }
                        if (endPolar == "start") { endItem->startArrow = nullptr; endItem->startArrowRota = ""; }
                        if (endPolar == "end")   { endItem->endArrow = nullptr;   endItem->endArrowRota = ""; }
                    }
                }
            }
        }
    }

    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
    update();
}

//! [13]
//!

DiagramItem* DiagramScene::findDiagramItemNear(QPointF point, qreal xTol, qreal yTol)
{
    QList<QGraphicsItem*> nearbyItems = items(QRectF(point.x() - xTol,
                                                      point.y() - yTol,
                                                      xTol * 2,
                                                      yTol * 2));

    for (QGraphicsItem* item : nearbyItems) {
        DiagramItem* diagramItem = qgraphicsitem_cast<DiagramItem*>(item);
        if (diagramItem)
            return diagramItem;
    }

    return nullptr;
}

QString DiagramScene::polarDiagramItem(DiagramItem *diagramItem, QPointF point)
{
    // Nesnenin local bounding rect ve boyut bilgisi
    QRectF rect = diagramItem->boundingRect();
    QSizeF size = rect.size();

    // Genişlik 100px → xTolerance = 25px (örn. %25)
    // Yükseklik 80px → yTolerance = 20px (örn. %25)
    qreal xTolerance = size.width() * 0.25;
    qreal yTolerance = size.height() * 0.25;

    // Kenar merkez noktalarının sahne koordinatları
    QPointF leftPoint   = diagramItem->mapToScene(QPointF(rect.left(),  rect.center().y()));
    QPointF rightPoint  = diagramItem->mapToScene(QPointF(rect.right(), rect.center().y()));
    QPointF topPoint    = diagramItem->mapToScene(QPointF(rect.center().x(), rect.top()));
    QPointF bottomPoint = diagramItem->mapToScene(QPointF(rect.center().x(), rect.bottom()));

    // Tıklanan nokta zaten sahne noktasında veriliyor: point

    // Her yön için tolerans dikdörtgeni oluştur (point bu dikdörtgenin içindeyse eşleşir)
    QRectF leftArea(leftPoint.x() - xTolerance, leftPoint.y() - yTolerance, xTolerance * 2, yTolerance * 2);
    QRectF rightArea(rightPoint.x() - xTolerance, rightPoint.y() - yTolerance, xTolerance * 2, yTolerance * 2);
    QRectF topArea(topPoint.x() - xTolerance, topPoint.y() - yTolerance, xTolerance * 2, yTolerance * 2);
    QRectF bottomArea(bottomPoint.x() - xTolerance, bottomPoint.y() - yTolerance, xTolerance * 2, yTolerance * 2);

    // Nokta hangi bölgedeyse onu döndür
    if (leftArea.contains(point))   return "left";
    if (rightArea.contains(point))  return "right";
    if (topArea.contains(point))    return "start";
    if (bottomArea.contains(point)) return "end";

    return "";
}

//! [14]
bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}


//! [14]


void DiagramScene::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        QList<QGraphicsItem *> selectedItemsList = selectedItems();
        for (QGraphicsItem *item : selectedItemsList) {
            // Eğer özel bir DiagramItem ise, onun üzerinden özel temizleme gerekiyorsa burada yap
            removeItem(item);
            delete item;
        }
    } else {
        QGraphicsScene::keyPressEvent(event);  // Diğer tuşlar için varsayılan davranış
    }
}

void DiagramScene::scaleSelectedItems(qreal factor)
{
    for (QGraphicsItem *item : selectedItems()) {
        item->setScale(item->scale() * factor);
    }
}

void DiagramScene::saveScene(const QString &filePath)
{
    QJsonArray itemsArray;
    QMap<DiagramItem*, int> itemIdMap;
    int nextId = 0;

    for (QGraphicsItem *item : items()) {
        QJsonObject obj;

        if (auto dItem = qgraphicsitem_cast<DiagramItem*>(item)) {
            int id = nextId++;
            itemIdMap[dItem] = id;

            obj["type"] = "DiagramItem";
            obj["id"] = id;
            obj["diagramType"] = static_cast<int>(dItem->myDiagramType);
            obj["x"] = dItem->pos().x();
            obj["y"] = dItem->pos().y();

            QJsonArray pointsArray;
            for (const QPointF &pt : dItem->polygon()) {
                QJsonObject ptObj;
                ptObj["x"] = pt.x();
                ptObj["y"] = pt.y();
                pointsArray.append(ptObj);
            }
            obj["polygon"] = pointsArray;
            obj["labelText"] = dItem->labelText;
            obj["labelAlgoritmaText"] = dItem->labelAlgoritmaText;
            obj["backgroundColor"] = dItem->myBackground.name();

            // selectedVariables
            QJsonArray variableArray;
            for (const VariableRecord &v : dItem->selectedVariables) {
                QJsonObject vObj;
                vObj["label"] = v.label;
                vObj["value"] = v.value;
                vObj["inputMessage"] = v.inputMessage;
                vObj["outputMessage"] = v.outputMessage;
                vObj["valueType"] = v.valueType;
                vObj["expression"] = v.expression;
                vObj["operationType"] = v.operationType;
                vObj["startValue"] = v.startValue;
                vObj["endValue"] = v.endValue;
                vObj["stepValue"] = v.stepValue;
                vObj["isInput"] = v.isInput;
                variableArray.append(vObj);
            }
            obj["variables"] = variableArray;

            itemsArray.append(obj);
        }
        else if (auto tItem = qgraphicsitem_cast<DiagramTextItem*>(item)) {
            obj["type"] = "DiagramTextItem";
            obj["text"] = tItem->toPlainText();
            obj["x"] = tItem->pos().x();
            obj["y"] = tItem->pos().y();
            itemsArray.append(obj);
        }
    }

    // Arrow'ları ekle
    for (QGraphicsItem *item : items()) {
        if (auto arrow = qgraphicsitem_cast<Arrow*>(item)) {
            QJsonObject obj;
            obj["type"] = "Arrow";
            obj["startId"] = itemIdMap.value(arrow->myStartItem, -1);
            obj["endId"] = itemIdMap.value(arrow->myEndItem, -1);
            obj["startPolar"] = arrow->myStartPolar;
            obj["endPolar"] = arrow->myEndPolar;
            obj["answer"] = arrow->answer;
            itemsArray.append(obj);
        }
    }

    // Global değişken listesi
    QJsonArray globalVars;
    for (const VariableRecord &v : Variable::onlineVariableList) {
        QJsonObject vObj;
        vObj["label"] = v.label;
        vObj["value"] = v.value;
        vObj["inputMessage"] = v.inputMessage;
        vObj["outputMessage"] = v.outputMessage;
        vObj["valueType"] = v.valueType;
        vObj["expression"] = v.expression;
        vObj["operationType"] = v.operationType;
        vObj["startValue"] = v.startValue;
        vObj["endValue"] = v.endValue;
        vObj["stepValue"] = v.stepValue;
        vObj["isInput"] = v.isInput;
        globalVars.append(vObj);
    }

    QJsonObject root;
    root["items"] = itemsArray;
    root["globalVariables"] = globalVars;

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
        file.write(QJsonDocument(root).toJson());
}
void DiagramScene::loadScene(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray itemsArray = doc["items"].toArray();

    clear();

    QMap<int, DiagramItem*> idItemMap;

    for (const QJsonValue &val : itemsArray) {
        QJsonObject obj = val.toObject();
        QString type = obj["type"].toString();

        if (type == "DiagramItem") {
            int id = obj["id"].toInt();
            Diagram::DiagramType dt = static_cast<Diagram::DiagramType>(obj["diagramType"].toInt());

            auto item = new DiagramItem(dt, myItemMenu);
            item->setPos(obj["x"].toDouble(), obj["y"].toDouble());

            QPolygonF polygon;
            QJsonArray pointsArray = obj["polygon"].toArray();
            for (const QJsonValue &ptVal : pointsArray) {
                QJsonObject ptObj = ptVal.toObject();
                polygon << QPointF(ptObj["x"].toDouble(), ptObj["y"].toDouble());
            }
           /// item->setPolygon(polygon);

            QString labelText = obj["labelText"].toString();
            QString labelAlgoritmaText = obj["labelAlgoritmaText"].toString();

            QColor bgColor = QColor(obj["backgroundColor"].toString());
            item->labelText=labelText;
            item->label.setText(labelText);

            item->labelAlgoritma.setText(labelAlgoritmaText);
            item->labelAlgoritmaText=labelAlgoritmaText;
            // selectedVariables
            QJsonArray variableArray = obj["variables"].toArray();
            for (const QJsonValue &vVal : variableArray) {
                QJsonObject vObj = vVal.toObject();
                VariableRecord v;
                v.inputMessage = vObj["inputMessage"].toString();
                v.outputMessage = vObj["outputMessage"].toString();
                v.label = vObj["label"].toString();
                v.value = vObj["value"].toString();
                v.valueType = vObj["valueType"].toString();
                v.expression = vObj["expression"].toString();
                v.operationType = vObj["operationType"].toInt();
                v.startValue = vObj["startValue"].toInt();
                v.endValue = vObj["endValue"].toInt();
                v.stepValue = vObj["stepValue"].toInt();
                v.isInput = vObj["isInput"].toBool();
                item->selectedVariables.append(v);
            }

            addItem(item);
            idItemMap[id] = item;
        }
        else if (type == "DiagramTextItem") {
            auto textItem = new DiagramTextItem(myItemMenu);
            textItem->setFont(myFont);
            textItem->setDefaultTextColor(myTextColor);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setPlainText(obj["text"].toString());
            textItem->setPos(obj["x"].toDouble(), obj["y"].toDouble());
            addItem(textItem);
        }
    }

    // Arrow'ları yükle
    for (const QJsonValue &val : itemsArray) {
        QJsonObject obj = val.toObject();
        if (obj["type"].toString() != "Arrow") continue;

        int startId = obj["startId"].toInt();
        int endId = obj["endId"].toInt();
        QString startPolar = obj["startPolar"].toString();
        QString endPolar = obj["endPolar"].toString();

        DiagramItem *startItem = idItemMap.value(startId, nullptr);
        DiagramItem *endItem = idItemMap.value(endId, nullptr);

        if (startItem && endItem && startItem != endItem) {
            Arrow *arrow = new Arrow(startItem, endItem, startPolar, endPolar, myItemMenu);
            arrow->setColor(myLineColor);
            arrow->answer = obj["answer"].toString();

            if (startItem->addArrowState(arrow, startPolar, "O") &&
                endItem->addArrowState(arrow, endPolar, "I")) {
                arrow->setZValue(-1000.0);
                addItem(arrow);
                arrow->updatePosition();
            }
        }
    }

    // Global değişken listesi yükle
    Variable::onlineVariableList.clear();
    QJsonArray globalVars = doc["globalVariables"].toArray();
    for (const QJsonValue &val : globalVars) {
        QJsonObject vObj = val.toObject();
        VariableRecord v;
        v.inputMessage = vObj["inputMessage"].toString();
        v.outputMessage = vObj["outputMessage"].toString();
        v.label = vObj["label"].toString();
        v.value = vObj["value"].toString();
        v.valueType = vObj["valueType"].toString();
        v.expression = vObj["expression"].toString();
        v.operationType = vObj["operationType"].toInt();
        v.startValue = vObj["startValue"].toInt();
        v.endValue = vObj["endValue"].toInt();
        v.stepValue = vObj["stepValue"].toInt();
        v.isInput = vObj["isInput"].toBool();
        Variable::onlineVariableList.append(v);
    }
}

void DiagramScene::saveAsPng(const QString &filePath)
{
    // Sadece sahnedeki nesnelerin kapladığı alan kadar kaydetmek için
    QRectF bounds = this->itemsBoundingRect();

    // Kenarlarda biraz boşluk bırakmak istersen:
    qreal margin = 10;
    bounds.adjust(-margin, -margin, margin, margin);

    QImage image(bounds.size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white);  // Arkaplan rengi, istersen Qt::transparent yapabilirsin

    QPainter painter(&image);
    this->render(&painter, QRectF(), bounds);
    image.save(filePath, "PNG");
}

void DiagramScene::alignCenterHorizontal()
{
    /*yatay hizala*/
    // Bütün sahnedeki itemleri seç
    ///for (QGraphicsItem *item : items())
    ///    item->setSelected(true);

    if (selectedItems().isEmpty()) return;

    QList<QList<QGraphicsItem*>> groups;

    // 1- Önce grupları belirle
    for (QGraphicsItem *item : selectedItems()) {
        DiagramItem *dItem = qgraphicsitem_cast<DiagramItem*>(item);
        if (!dItem) continue;

        // Eğer item daha önce bir gruba eklenmiş mi, kontrol et
        bool inGroup = false;
        for (const QList<QGraphicsItem*> &g : groups) {
            if (g.contains(dItem)) {
                inGroup = true;
                break;
            }
        }
        if (inGroup) continue;

        // Eğer Loop ise, loop + bağlı elemanlar bir grup
        if (dItem->myDiagramType == Diagram::Loop) {
            QList<QGraphicsItem*> group;
            group << dItem;

            // Bottom -> Left bağlantılı olanları da ekle
            for (Arrow *arrow : dItem->arrows) {
                if (arrow->myStartItem == dItem && arrow->myStartPolar == "bottom") {
                    DiagramItem *target = arrow->myEndItem;
                    if (target) group << target;
                }
                if (arrow->myStartItem == dItem && arrow->myStartPolar == "left") {
                    DiagramItem *target = arrow->myEndItem;
                    if (target) group << target;
                }
            }
            groups << group;
        } else {
            // Diğer bağımsız itemler
            groups << (QList<QGraphicsItem*>() << dItem);
        }
    }

    // 2- Şimdi her grubun merkez x'ini al
    qreal totalX = 0;
    for (const QList<QGraphicsItem*> &group : groups) {
        qreal groupSumX = 0;
        for (QGraphicsItem *item : group)
            groupSumX += item->pos().x();
        qreal groupCenterX = groupSumX / group.size();
        totalX += groupCenterX;
    }
    qreal averageX = totalX / groups.size();

    // 3- Şimdi her grubu ortalama x'e göre kaydır
    for (const QList<QGraphicsItem*> &group : groups) {
        qreal groupSumX = 0;
        for (QGraphicsItem *item : group)
            groupSumX += item->pos().x();
        qreal groupCenterX = groupSumX / group.size();
        qreal offsetX = averageX - groupCenterX;

        for (QGraphicsItem *item : group) {
            QPointF oldPos = item->pos();
            item->setPos(oldPos.x() + offsetX, oldPos.y());
        }
    }

    // 4- Son olarak tüm Arrow'ları güncelle
    for (QGraphicsItem *item : items()) {
        Arrow* arrow = qgraphicsitem_cast<Arrow*>(item);
        if (arrow)
            arrow->updatePosition();
    }

    update();
}

void DiagramScene::alignCenterVertical()
{
    /*dikey hizalama*/
    if (selectedItems().isEmpty()) return;

    QList<QList<QGraphicsItem*>> groups;

    // 1- Grupları belirle
    for (QGraphicsItem *item : selectedItems()) {
        DiagramItem *dItem = qgraphicsitem_cast<DiagramItem*>(item);
        if (!dItem) continue;

        // Daha önce gruba eklenmiş mi kontrol
        bool inGroup = false;
        for (const QList<QGraphicsItem*> &g : groups) {
            if (g.contains(dItem)) {
                inGroup = true;
                break;
            }
        }
        if (inGroup) continue;

        // Loop ise: loop + bağlı bottom ve left nesneleri grup
        if (dItem->myDiagramType == Diagram::Loop) {
            QList<QGraphicsItem*> group;
            group << dItem;

            for (Arrow *arrow : dItem->arrows) {
                if (arrow->myStartItem == dItem && (arrow->myStartPolar == "bottom" || arrow->myStartPolar == "left")) {
                    DiagramItem *target = arrow->myEndItem;
                    if (target) group << target;
                }
            }
            groups << group;
        } else {
            // Bağımsız item
            groups << (QList<QGraphicsItem*>() << dItem);
        }
    }

    // 2- Grupların ortalama Y merkezini bul
    qreal totalY = 0;
    for (const QList<QGraphicsItem*> &group : groups) {
        qreal groupSumY = 0;
        for (QGraphicsItem *item : group)
            groupSumY += item->pos().y();
        qreal groupCenterY = groupSumY / group.size();
        totalY += groupCenterY;
    }
    qreal averageY = totalY / groups.size();

    // 3- Grupları kaydır
    for (const QList<QGraphicsItem*> &group : groups) {
        qreal groupSumY = 0;
        for (QGraphicsItem *item : group)
            groupSumY += item->pos().y();
        qreal groupCenterY = groupSumY / group.size();
        qreal offsetY = averageY - groupCenterY;

        for (QGraphicsItem *item : group) {
            QPointF oldPos = item->pos();
            item->setPos(oldPos.x(), oldPos.y() + offsetY);
        }
    }

    // 4- Tüm Arrow'ları güncelle
    for (QGraphicsItem *item : items()) {
        Arrow* arrow = qgraphicsitem_cast<Arrow*>(item);
        if (arrow)
            arrow->updatePosition();
    }

    update();
}
