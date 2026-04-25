
#include "../include/diagramitem.h"
#include "../include/arrow.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include<QThread>
#include<QCheckBox>
#include"../include/variable.h"
//extern QString varMain0;
#include<QTableWidget>
#include<QHeaderView>

//! [0]
DiagramItem::DiagramItem(Diagram::DiagramType diagramType, QMenu *contextMenu,
             QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
{
    startArrow=0;
    endArrow=0;
    leftArrow=0;
    rightArrow=0;
    drm=0;
    myDiagramType = diagramType;
    myContextMenu = contextMenu;
    //label=new QStaticText();
    label.setText("");

    labelAlgoritma.setText("");
    //label->setStyleSheet("background: red");
    //label->show();
    myBackgroundColor=QColor(0,0,0,0);
   // qDebug()<<"ayaralnana renk"<<myBackgroundColor.name(QColor::HexArgb);
    myTextColor=QColor(0,0,0,255);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setAcceptHoverEvents(true);
    setFlags(ItemIsSelectable|ItemSendsGeometryChanges);
    for(int i = 0; i < 8; i++){
        cornerGrabber[i] = new DotSignal(this);
    }

   // setPositionGrabbers();
//hideGrabbers();

setVisibilityGrabbers();
//drm=false;

if(myDiagramType==Diagram::DiagramType::Input)
{this->myDiagramWidth=200;this->myDiagramHeight=90;}

if(myDiagramType==Diagram::DiagramType::Start)
{this->myDiagramWidth=100;this->myDiagramHeight=60;}

if(myDiagramType==Diagram::DiagramType::End)
{this->myDiagramWidth=100;this->myDiagramHeight=60;}

if(myDiagramType==Diagram::DiagramType::Process)
{this->myDiagramWidth=200;this->myDiagramHeight=75;}

if(myDiagramType==Diagram::DiagramType::Conditional)
{this->myDiagramWidth=200;this->myDiagramHeight=90;}

if(myDiagramType==Diagram::DiagramType::Loop)
{this->myDiagramWidth=200;this->myDiagramHeight=75;}

if(myDiagramType==Diagram::DiagramType::Output)
{this->myDiagramWidth=200;this->myDiagramHeight=90;}

if(myDiagramType==Diagram::DiagramType::Link)
{this->myDiagramWidth=50;this->myDiagramHeight=50;}


f.setBold(true);
f = QFont("Arial", 12);
}
void DiagramItem::setVisibilityGrabbers()
{
///8 noktanın hangilerinin gözüküp gözükmayaceği buradan belirleniyor
    cornerGrabber[GrabberTop]->setVisible(true);
    cornerGrabber[GrabberBottom]->setVisible(true);
    cornerGrabber[GrabberLeft]->setVisible(true);
    cornerGrabber[GrabberRight]->setVisible(true);
    cornerGrabber[GrabberTopLeft]->setVisible(false);
    cornerGrabber[GrabberTopRight]->setVisible(false);
    cornerGrabber[GrabberBottomLeft]->setVisible(false);
    cornerGrabber[GrabberBottomRight]->setVisible(false);

}

void DiagramItem::setPositionGrabbers()
{

    ///burada noktaların pozisyonlarının ayarlandığı bölüm
    QRectF tmpRect = this->boundingRect();
    int h= cornerGrabber[GrabberRight]->boundingRect().height();
    int w= cornerGrabber[GrabberRight]->boundingRect().width();
    int wx=0;
    if(myDiagramType==Diagram::DiagramType::Input)
    {
        wx=w*2+w;
    }

    cornerGrabber[GrabberTop]->setPos(tmpRect.left() + tmpRect.width()/2-w, tmpRect.top());
    cornerGrabber[GrabberTop]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberTop]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberTop]->setZValue(-100);
    cornerGrabber[GrabberTop]->setDotFlags(GrabberTop);


    cornerGrabber[GrabberBottom]->setPos(tmpRect.left() + tmpRect.width()/2-w, tmpRect.bottom()-h-h);
    cornerGrabber[GrabberBottom]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberBottom]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberBottom]->setDotFlags(GrabberBottom);

    cornerGrabber[GrabberLeft]->setPos(tmpRect.left()+wx, tmpRect.top() + tmpRect.height()/2-h);
    cornerGrabber[GrabberLeft]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberLeft]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberLeft]->setDotFlags(GrabberLeft);

      cornerGrabber[GrabberRight]->setPos(tmpRect.right()-w-w-wx, tmpRect.top() + tmpRect.height()/2-h);
    cornerGrabber[GrabberRight]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberRight]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberRight]->setDotFlags(GrabberRight);

/**********************************************************************************************************************/

    cornerGrabber[GrabberTopLeft]->setPos(tmpRect.topLeft().x(), tmpRect.topLeft().y());
    cornerGrabber[GrabberTopLeft]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberTopLeft]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberTopLeft]->setZValue(-100);
    cornerGrabber[GrabberTopLeft]->setDotFlags(GrabberTopLeft);


    cornerGrabber[GrabberTopRight]->setPos(tmpRect.topRight().x()-8, tmpRect.topRight().y());
    cornerGrabber[GrabberTopRight]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberTopRight]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberTopRight]->setZValue(-100);
    cornerGrabber[GrabberTopRight]->setDotFlags(GrabberTopRight);

    cornerGrabber[GrabberBottomLeft]->setPos(tmpRect.bottomLeft().x(), tmpRect.bottomLeft().y()-8);
    cornerGrabber[GrabberBottomLeft]->setFlag(QGraphicsItem::ItemIsSelectable,true);
    cornerGrabber[GrabberBottomLeft]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberBottomLeft]->setZValue(-100);
    cornerGrabber[GrabberBottomLeft]->setDotFlags(GrabberBottomLeft);

    cornerGrabber[GrabberBottomRight]->setPos(tmpRect.bottomRight().x()-8, tmpRect.bottomRight().y()-8);
    cornerGrabber[GrabberBottomRight]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    cornerGrabber[GrabberBottomRight]->setFlag(QGraphicsItem::ItemIsMovable,false);
    cornerGrabber[GrabberBottomRight]->setZValue(-100);
    cornerGrabber[GrabberBottomRight]->setDotFlags(GrabberBottomRight);

update();

}

QPointF DiagramItem::previousPosition() const
{
    return m_previousPosition;
}

void DiagramItem::setPreviousPosition(const QPointF previousPosition)
{
    if (m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged();
}
//! [0]
void DiagramItem::hideGrabbers()
{
    for(int i = 0; i < 8; i++){
        cornerGrabber[i]->setVisible(false);
    }
}

//! [1]
void DiagramItem::removeArrow(Arrow *arrow)
{
  qDebug()<<"ok silinecek";
    int index = arrows.indexOf(arrow);

    if (index != -1)
    {
         qDebug()<<"ok silindi";
        arrows.removeAt(index);
    }
}
//! [1]
void DiagramItem::removeArrowPolar(Arrow *arrow,QString polar){
    if(polar=="left")leftArrow=0;
    if(polar=="right")rightArrow=0;
    if(polar=="start")startArrow=0;
    if(polar=="end")endArrow=0;
}
//! [2]
void DiagramItem::removeArrows()
{
    foreach (Arrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}
//! [2]

//! [3]
bool DiagramItem::addArrowState(Arrow *arrow,QString polar,QString rota)
{
   // qDebug()<<"nesne"<<this->myDiagramType;
    int polarCount=0;
    if(startArrow!=0) polarCount++;
    if(endArrow!=0) polarCount++;
    if(leftArrow!=0) polarCount++;
    if(rightArrow!=0) polarCount++;
    if(this->myDiagramType==Diagram::DiagramType::Start
            ||this->myDiagramType==Diagram::DiagramType::End)
    {

       if(polarCount>0)return false;
    }
    if(this->myDiagramType==Diagram::DiagramType::Input
            ||this->myDiagramType==Diagram::DiagramType::Process
        ||this->myDiagramType==Diagram::DiagramType::Output)
    {
       if(polarCount>1)return false;
    }
    if(this->myDiagramType==Diagram::DiagramType::Conditional)
    {
       if(polarCount>2)return false;
    }
    if(this->myDiagramType==Diagram::DiagramType::Link)
    {
        if(polarCount>3)return false;
    }
    //arrows.append(arrow);
    if(polar=="start"&&startArrow==0) { startArrow=arrow;startArrowRota=rota;return true;}
    if(polar=="start"&&startArrow!=0) { return false;}

    if(polar=="end"&&endArrow==0){endArrow=arrow;endArrowRota=rota;return true;}
    if(polar=="end"&&endArrow!=0){return false;}

    if(polar=="left"&&leftArrow==0) { leftArrow=arrow;leftArrowRota=rota;return true;}
    if(polar=="left"&&leftArrow!=0) { return false;}

    if(polar=="right"&&rightArrow==0){rightArrow=arrow;rightArrowRota=rota; return true;}
    if(polar=="right"&&rightArrow!=0)return false;
}

void DiagramItem::setText(QString text,QColor color)
{
    if(text!="step")
    {
        labelText = text; // sakla
        label = QStaticText(text);
        labelText = text; // sakla
        label = QStaticText(text);

    }
    ///  myBackground = color;
    setBrush(myBackgroundColor);         // ⬅️ Görsel rengi uygula!
    /// labelDiagram.setText(text);
    //  labelDiagram.setText(QString::number(myDiagramType));
    myBackgroundColor=color;
    //painter->setBrush(myBackground);
    update();
}


//! [3]

//! [5]
void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
//! [5]
//! [6]
QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, arrows) {
            arrow->updatePosition();
        }
        //return value;
    }

   /* if (change == QGraphicsItem::ItemTransformChange ||
        change == QGraphicsItem::ItemPositionChange ||
        change == QGraphicsItem::ItemScaleChange)
    {
        QRectF rect = this->boundingRect();
        QSizeF textSize = labelItem->boundingRect().size()*0.5;
        QPointF textPos(rect.center().x() - textSize.width() / 2,
                        rect.center().y() - textSize.height() / 2);
        labelItem->setPos(textPos);
    }*/
    //return QGraphicsItem::itemChange(change, value);

    return value;
}
//! [6]
void DiagramItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    rotateState=  !rotateState;
    if(this->myDiagramType==Diagram::DiagramType::Input)
    {
        VariableInputDialog dlg;
        if(selectedVariables.size()==0)
        {
            // İlk işlem satırı veya boş başlat
            dlg.addVariableRow();
        }
        for (int j = 0; j < selectedVariables.size(); ++j) {
            VariableRecord varselect = selectedVariables[j];
            //qDebug() << "labelText: " << labelText;
            dlg.addVariableRow(varselect);
        }
        if (dlg.exec() == QDialog::Accepted) {
            selectedVariables.clear();
            label.setTextFormat(Qt::RichText);
            label.setText("");
            labelAlgoritma.setTextFormat(Qt::RichText);
            labelAlgoritma.setText("");

            for (VariableRecord &selected : dlg.getSelectedVariables()) {
                selectedVariables.append(selected);
                qDebug() << "seçilen:" << selected.label << selected.value << selected.valueType<<selected.isInput;
                QString inputMesaj="";
                if(selected.inputMessage!="")
                    inputMesaj="\""+selected.inputMessage+"\", ";

                if(label.text()=="")
                {
                    if (selected.isInput&&selected.valueType == "int") {
                        label.setText(inputMesaj+  selected.label + " = ?");
                        labelAlgoritma.setText(inputMesaj+  selected.label + " (oku)");
                    }
                    if (selected.isInput&&selected.valueType == "string") {
                        label.setText(inputMesaj+  selected.label + " = ?");
                        labelAlgoritma.setText(inputMesaj+  selected.label + " (oku)");
                    }

                    if (selected.isInput&&selected.valueType == "float") {
                        label.setText(inputMesaj+  selected.label + " = ?");
                        labelAlgoritma.setText(inputMesaj+  selected.label + " (oku)");
                    }

                    if (!selected.isInput&&selected.valueType == "int") {
                        label.setText( selected.label + " = " + selected.value);
                        labelAlgoritma.setText(selected.label + " = " + selected.value);
                    }
                    if (!selected.isInput&&selected.valueType == "string") {
                        label.setText(selected.label + " = \"" + selected.value + "\"");
                        labelAlgoritma.setText(selected.label + " = \"" + selected.value + "\"");
                    }

                    if (!selected.isInput&&selected.valueType == "float") {
                        label.setText( selected.label + " = " + selected.value);
                        labelAlgoritma.setText(selected.label + " = " + selected.value);
                    }

                }
                else
                {
                    if (selected.isInput&&selected.valueType == "int") {
                        label.setText(label.text() + "<br>" +inputMesaj+  selected.label + " = ?");
                        labelAlgoritma.setText(labelAlgoritma.text() + "<br>" +inputMesaj+  selected.label + " (oku)");
                    }
                    if (selected.isInput&&selected.valueType == "string") {
                        label.setText(label.text() + "<br>" +inputMesaj+  selected.label + " = ?");
                        labelAlgoritma.setText(labelAlgoritma.text() + "<br>" +inputMesaj+  selected.label + " (oku)");
                    }

                    if (selected.isInput&&selected.valueType == "float") {
                        label.setText(label.text() + "<br>" +inputMesaj+  selected.label + " = ?");
                        labelAlgoritma.setText(labelAlgoritma.text() + "<br>" +inputMesaj+  selected.label + " (oku)");
                    }
                    if (!selected.isInput&&selected.valueType == "int") {
                        label.setText(label.text() + "<br>" + selected.label + " = " + selected.value);
                        labelAlgoritma.setText(labelAlgoritma.text() + "<br>" +selected.label + " = " + selected.value);
                    }
                    if (!selected.isInput&&selected.valueType == "string") {
                        label.setText(label.text() + "<br>" + selected.label + " = \"" + selected.value + "\"");
                        labelAlgoritma.setText(labelAlgoritma.text() + "<br>" +selected.label + " = \"" + selected.value + "\"");
                    }
                    if (!selected.isInput&&selected.valueType == "float") {
                        label.setText(label.text() + "<br>" + selected.label + " = " + selected.value);
                        labelAlgoritma.setText(labelAlgoritma.text() + "<br>" +selected.label + " = " + selected.value);
                    }
                }
                labelText=label.text();
                labelAlgoritmaText=labelAlgoritma.text();
            }
        }
     }
    if(this->myDiagramType==Diagram::DiagramType::Output)
    {
         VariableOutputDialog dlg;
        if(selectedVariables.size()==0)
        {
            // İlk işlem satırı veya boş başlat
            dlg.addExpressionRow();
        }
         for (int j = 0; j < selectedVariables.size(); ++j) {
             const VariableRecord &varselect = selectedVariables[j];
             qDebug() << "tanımlı işlemler: " << varselect.operationType << varselect.expression;
             dlg.addExpressionRowparametre(varselect);
         }
         if (dlg.exec() == QDialog::Accepted) {
             selectedVariables.clear();
             label.setText("");
             label.setTextFormat(Qt::RichText);  // Bunu mutlaka ekleyin
             labelAlgoritma.setText("");
             labelAlgoritma.setTextFormat(Qt::RichText);  // Bunu mutlaka ekleyin

             for (const auto &rec :  dlg.getExpressionsWithType()) {
                 if(rec.expression!=""){
                     ///qDebug() << "İşlem türü:" << type << "İfade:" << expr;
                     VariableRecord selected;
                     selected.label = rec.label;
                     selected.expression=rec.expression;
                     selected.operationType=rec.outputType;
                     selected.outputMessage=rec.outputMessage;
                     selectedVariables.append(selected);
                     QString outputMesaj="";
                     if(selected.outputMessage!="")
                         outputMesaj="\""+selected.outputMessage+"\", ";

                     /**********************************************/
                     if(label.text()=="")
                     {
                         if(rec.outputType!=1)
                         {
                             label.setText(outputMesaj+selected.expression);
                             labelAlgoritma.setText(outputMesaj+selected.expression+" (yaz)");
                         }else{
                             label.setText(outputMesaj+selected.expression+"="+selected.expression);
                             labelAlgoritma.setText(outputMesaj+selected.expression+"="+selected.expression+" (yaz)");
                         }
                     }
                     else
                     {
                         if(rec.outputType!=1)
                         {
                             label.setText(label.text()+"<br>"+outputMesaj+selected.expression);
                             labelAlgoritma.setText(labelAlgoritma.text()+"<br>"+outputMesaj+selected.expression+" (yaz)");
                         }else{
                             label.setText(label.text()+"<br>"+outputMesaj+selected.expression+"="+selected.expression);
                             labelAlgoritma.setText(labelAlgoritma.text()+"<br>"+outputMesaj+selected.expression+"="+selected.expression+" (yaz)");
                         }
                     }
                      /*********************************************/
                     labelText=label.text();
                     labelAlgoritmaText=labelAlgoritma.text();
                 }
             }
         }
    }
    if(this->myDiagramType==Diagram::DiagramType::Process)
    {
        VariableProcessDialog dlg;
        if(selectedVariables.size()==0)
        {
            // İlk işlem satırı veya boş başlat
            dlg.addExpressionRow();
        }
        for (int j = 0; j < selectedVariables.size(); ++j) {
            const VariableRecord &varselect = selectedVariables[j];
            qDebug() << "tanımlı işlemler: " << varselect.operationType << varselect.expression;
            dlg.addExpressionRowparametre(varselect.operationType, varselect.expression);
        }
        if (dlg.exec() == QDialog::Accepted) {

            auto processList = dlg.getExpressionsWithType();
            selectedVariables.clear();
            label.setText("");
            label.setTextFormat(Qt::RichText);  // Bunu mutlaka ekleyin
            for (const auto &rec : processList) {
                if(rec.expression!=""){
                     qDebug() << "İşlem türü:"<<rec.targetLabel << rec.processType << "İfade:" << rec.expression;
                    VariableRecord selected;
                    selected.label = rec.targetLabel;
                    selected.expression=rec.expression;
                    selected.operationType=rec.processType;
                    selected.valueType="int";
                    for (const auto& v : Variable::onlineVariableList) {
                        if (v.label == selected.label) {
                            selected.valueType=v.valueType;
                        }
                    }
                    selectedVariables.append(selected);
                    /**********************************************/
                    if(label.text()=="")
                    {
                        label.setText(selected.expression);
                    }
                    else
                    {
                        label.setText(label.text()+"<br>"+selected.expression);
                    }
                    /*********************************************/
                    labelText=label.text();
                    labelAlgoritmaText=label.text();
                    labelAlgoritma.setText(label.text());
                }
            }
            //setTempText(label.text(),QColor(255,0,0,255));
        }
    }
    if(this->myDiagramType==Diagram::DiagramType::Loop)
    {
        VariableLoopDialog dlg;
        if(selectedVariables.size()==0)
        {
            // İlk işlem satırı veya boş başlat
            dlg.addLoopRowparametre(0, "");
        }
        for (int j = 0; j < selectedVariables.size(); ++j) {
            const VariableRecord &varselect = selectedVariables[j];
            qDebug() << "tanımlı işlemler: " << varselect.operationType << varselect.expression;
            //dlg.addExpressionRowparametre(varselect.operationType, varselect.expression);
            dlg.addLoopRowparametre(varselect.operationType, varselect.expression);
        }
        if (dlg.exec() == QDialog::Accepted) {
            selectedVariables.clear();
            auto loops = dlg.getLoopsWithType();
            label.setText("");
            //labelDiagram.setTextFormat(Qt::RichText);  // Bunu mutlaka ekleyin
            label.setTextFormat(Qt::PlainText);
            for (const LoopRecord &loop : loops) {
                    qDebug() << "İşlem türü:" << loop.loopType<< "İfade:" << loop.expression<<loop.endValue;
                    VariableRecord selected;
                    selected.label = loop.label;
                    selected.expression=loop.expression;
                    selected.operationType=loop.loopType;
                    selected.startValue=loop.startValue;
                    selected.endValue=loop.endValue;
                    selected.stepValue=loop.stepValue;
                    selected.counter=0;
                    selectedVariables.append(selected);

                    if(loop.loopType==0){
                        QString ifade = loop.label + "=" + QString::number(loop.startValue) + "; " +
                                        loop.label + "<" + QString::number(loop.endValue) + "; " +
                                        loop.label + "=" + loop.label + "+" + QString::number(loop.stepValue) + ";";
                        qDebug()<<"for pozitive "<<ifade;
                    label.setText(ifade);
                    }else if(loop.loopType==1){
                        int tempStepValue=0;
                        if(loop.stepValue<0)tempStepValue=loop.stepValue*-1;
                        else tempStepValue=loop.stepValue;
                        QString ifade = loop.label + "=" + QString::number(loop.startValue) + "; " +
                                        loop.label + ">" + QString::number(loop.endValue) + "; " +
                                        loop.label + "=" + loop.label + "-" + QString::number(tempStepValue) + ";";
                        qDebug()<<"for negative "<<ifade;
                        label.setText(ifade);

                    }else if(loop.loopType==2){
                         qDebug()<<"while "<<selected.label<<loop.expression;
                        label.setText(loop.expression);
                    }
           }
        }
        labelText=label.text();
        labelAlgoritma.setText(label.text());
        labelAlgoritmaText=label.text();
    }
    if(this->myDiagramType==Diagram::DiagramType::Conditional)
    {
        VariableConditionDialog dlg;
        if(selectedVariables.size()==0)
        {
            // İlk işlem satırı veya boş başlat
            dlg.addExpressionRow();
        }
        for (int j = 0; j < selectedVariables.size(); ++j) {
            const VariableRecord &varselect = selectedVariables[j];
            qDebug() << "tanımlı işlemler: " << varselect.operationType << varselect.expression;
            dlg.addExpressionRowparametre(varselect.operationType, varselect.expression);
        }
        if (dlg.exec() == QDialog::Accepted) {
            selectedVariables.clear();
            auto exprList = dlg.getExpressionsWithType();
            label.setText("");
            //labelDiagram.setTextFormat(Qt::RichText);  // Bunu mutlaka ekleyin
            label.setTextFormat(Qt::PlainText);
            for (const auto &pair : exprList) {
                int type = pair.first;
                QString expr = pair.second;
                if(expr.split(' ')[2].trimmed()!=""){
                    qDebug() << "İşlem türü:" << type << "İfade:" << expr;
                    VariableRecord selected;
                    selected.label = expr.split(" ")[0].trimmed();
                    selected.expression=expr;
                    selected.operationType=type;
                    selectedVariables.append(selected);
                    label.setText(expr);

              }
            }
            labelText=label.text();
            labelAlgoritma.setText("eğer "+label.text());
            labelAlgoritmaText="eğer "+label.text();
        }

    }

    QGraphicsItem::mouseDoubleClickEvent(event);
}

void DiagramItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  // qDebug() <<"move yapıldı";
 //  qDebug() <<"üzerine geldi"<<drm<<rotateState;
   if(drm)
   {
    QPointF pt = event->pos();
    if(m_actionFlags == ResizeState){
        switch (m_cornerFlags) {
        case Top:
            resizeTop(pt);
            break;
        case Bottom:
            resizeBottom(pt);
            break;
        case Left:
            resizeLeft(pt);
            break;
        case Right:
            resizeRight(pt);
            break;
        case TopLeft:
            resizeTop(pt);
            resizeLeft(pt);
            break;
        case TopRight:
           resizeTop(pt);
            resizeRight(pt);
            rotateItem(pt);
            break;
        case BottomLeft:
            resizeBottom(pt);
            resizeLeft(pt);
            break;
        case BottomRight:
            resizeBottom(pt);
            resizeRight(pt);
            break;
        default:
            if (m_leftMouseButtonPressed&&drm) {
               // qDebug() <<"taşınıyor";
                setCursor(Qt::ClosedHandCursor);
                auto dx = event->scenePos().x() - m_previousPosition.x();
                auto dy = event->scenePos().y() - m_previousPosition.y();
                moveBy(dx,dy);
                setPreviousPosition(event->scenePos());
                emit signalMove(this, dx, dy);

            }
            break;
        }
    } else {
        //qDebug() <<"abc";
        switch (m_cornerFlags) {
        case Top:{

            resizeTop(pt);
            break;
        }
        case Right:{
            resizeRight(pt);

            break;
        }
        case Left: {
          //  Scene * _scene = dynamic_cast<Scene *>(parent);
            ///if(sekilTr!=Diagram::DiagramType::Pdf) scn->removeItem(this);
            resizeLeft(pt);

           // qDebug() <<"siliniyorrr";

           break;
        }
        case Bottom: {
            resizeBottom(pt);
            break;
        }
        default:
           if (m_leftMouseButtonPressed&&drm) {
             //  qDebug() <<"gidiyor";
                setCursor(Qt::ClosedHandCursor);
                auto dx = event->scenePos().x() - m_previousPosition.x();
                auto dy = event->scenePos().y() - m_previousPosition.y();
                moveBy(dx,dy);
                setPreviousPosition(event->scenePos());
                //emit signalMove(this, dx, dy);

            }

            break;
        }
    }
   }
   ///scn->update();
   QGraphicsItem::mouseMoveEvent(event);
}

void DiagramItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if (event->button() & Qt::LeftButton) {
        drm=true;
        m_leftMouseButtonPressed = true;
        setPreviousPosition(event->scenePos());
        emit clicked(this);
       // qDebug() <<"rectangle nesnesine tıklama yapıldı";
    }
    QGraphicsItem::mousePressEvent(event);
//QGraphicsScene::mousePressEvent(event);
}

void DiagramItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() <<"rectangle nesnesine release yapıldı";
    if (event->button() & Qt::LeftButton) {
        m_leftMouseButtonPressed = false;
        drm=false;

        /****************************************************/

        /* dclick=(!dclick)?true:false;
        if(dclick)
        {
            setPositionGrabbers();
            setVisibilityGrabbers();
          //  QGraphicsItem::hoverEnterEvent(event);
        }
        else
        {
            m_cornerFlags = 0;
           hideGrabbers();
           setCursor(Qt::CrossCursor);
          // drm=false;
        }
        qDebug() <<"tek tıklama tıklama "<<dclick;*/

        /*****************************************************/
    }
    QGraphicsItem::mouseReleaseEvent(event);
}
void DiagramItem::renk()
{
    qDebug() <<"renk"<<drm;//<<myDiagramType;
    renkdrm=true;
}
void DiagramItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug() <<"üzerine geldi"<<drm;
  /* if(drm)
   {
    setPositionGrabbers();
    setVisibilityGrabbers();

   }*/
    //QGraphicsItem::hoverEnterEvent(event);
}

void DiagramItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
//renkdrm=false;
//update();
   //  qDebug() <<"üzerine ayrıldı"<<drm;
  /*
     m_cornerFlags = 0;
    hideGrabbers();
    setCursor(Qt::CrossCursor);
    drm=false;
*/
    //QGraphicsItem::hoverLeaveEvent( event );

}

void DiagramItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
   // renkdrm=true;
   // qDebug() <<"üzerindesin"<<drm<<renkdrm;

  /*  if(drm)
    {
    //qDebug() <<"hoverMoveEvent";
    QPointF pt = event->pos();              // The current position of the mouse
    qreal drx = pt.x() - this->boundingRect().right();    // Distance between the mouse and the right
    qreal dlx = pt.x() - this->boundingRect().left();     // Distance between the mouse and the left

    qreal dby = pt.y() - this->boundingRect().top();      // Distance between the mouse and the top
    qreal dty = pt.y() - this->boundingRect().bottom();   // Distance between the mouse and the bottom

    // If the mouse position is within a radius of 7
    // to a certain side( top, left, bottom or right)
    // we set the Flag in the Corner Flags Register

    m_cornerFlags = 0;
    int alan=25;
    if( (dby < alan && dby > -alan)||(dby < -alan && dby > alan) ) m_cornerFlags |= Top;       // Top side
    if( (dty < alan && dty > -alan)|| (dty < -alan && dty > alan)) m_cornerFlags |= Bottom;    // Bottom side
    if( (drx < alan && drx > -alan )||(drx < -alan && drx > alan )) m_cornerFlags |= Right;     // Right side
    if( (dlx < alan && dlx > -alan)||  (dlx < -alan && dlx > alan)) m_cornerFlags |= Left;      // Left side


        switch (m_cornerFlags) {
        case Top:
        {
            qDebug() <<"top";
            break;

        }
        case Right: {
                  qDebug() <<"right";
                    break;
                }
        case Left:
        {
            qDebug() <<"Left";
            break;
                }
        case Bottom: {
            qDebug() <<"bottom";
            break;
        }
        default:
            setCursor(Qt::ArrowCursor);
            break;
        }

    }
*/
  //  QGraphicsItem::hoverMoveEvent( event );
}

void DiagramItem::resizeLeft(const QPointF &pt)
{
    qDebug()<<"resizeLeft";
    /*
    QRectF tmpRect = rect();
    // if the mouse is on the right side we return
    if( pt.x() > tmpRect.right() )
        return;
    qreal widthOffset =  ( pt.x() - tmpRect.right() );
    // limit the minimum width
    if( widthOffset > -10 )
        return;
    // if it's negative we set it to a positive width value
    if( widthOffset < 0 )
        tmpRect.setWidth( -widthOffset );
    else
        tmpRect.setWidth( widthOffset );
    // Since it's a left side , the rectange will increase in size
    // but keeps the topLeft as it was
    tmpRect.translate( rect().width() - tmpRect.width() , 0 );
    prepareGeometryChange();
    // Set the ne geometry
    setRect( tmpRect );
    // Update to see the result
    update();
    setPositionGrabbers();
    */
}

void DiagramItem::resizeRight(const QPointF &pt)
{
      qDebug()<<"resizeRight";
    /*
    QRectF tmpRect = rect();
    if( pt.x() < tmpRect.left() )
        return;
    qreal widthOffset =  ( pt.x() - tmpRect.left() );
    if( widthOffset < 10 ) /// limit
        return;
    if( widthOffset < 10)
        tmpRect.setWidth( -widthOffset );
    else
        tmpRect.setWidth( widthOffset );
    prepareGeometryChange();
    setRect( tmpRect );
    update();
    setPositionGrabbers();
    */
}

void DiagramItem::resizeBottom(const QPointF &pt)
{
     qDebug()<<"resizeBottom";
    /*QRectF tmpRect = rect();
    if( pt.y() < tmpRect.top() )
        return;
    qreal heightOffset =  ( pt.y() - tmpRect.top() );
    if( heightOffset < 11 ) /// limit
        return;
    if( heightOffset < 0)
        tmpRect.setHeight( -heightOffset );
    else
        tmpRect.setHeight( heightOffset );
    prepareGeometryChange();
    setRect( tmpRect );
    update();
    setPositionGrabbers();
    */
}

void DiagramItem::resizeTop(const QPointF &pt)
{
     qDebug()<<"resizeTop";
   /* DiagramItem
    QRectF tmpRect = rect();
    if( pt.y() > tmpRect.bottom() )
        return;
    qreal heightOffset =  ( pt.y() - tmpRect.bottom() );
    if( heightOffset > -11 ) /// limit
        return;
    if( heightOffset < 0)
        tmpRect.setHeight( -heightOffset );
    else
        tmpRect.setHeight( heightOffset );
    tmpRect.translate( 0 , rect().height() - tmpRect.height() );
    prepareGeometryChange();
    setRect( tmpRect );
    update();
    setPositionGrabbers();
    */
}

void DiagramItem::rotateItem(const QPointF &pt)
{
     qDebug()<<"rotateItem";
   /* QRectF tmpRect = rect();
    QPointF center;
    if(rotateState)    center = boundingRect().bottomRight();
    else center = boundingRect().center();

    QPointF corner;
    switch (m_cornerFlags) {
    case TopLeft:
        corner = tmpRect.topLeft();
        break;
    case TopRight:
        corner = tmpRect.topRight();
        break;
    case BottomLeft:
        corner = tmpRect.bottomLeft();
        break;
    case BottomRight:
        corner = tmpRect.bottomRight();
        break;
    default:
        break;
    }

    QLineF lineToTarget(center,corner);
    QLineF lineToCursor(center, pt);
    // Angle to Cursor and Corner Target points
    qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
    qreal angleToCursor = ::acos(lineToCursor.dx() / lineToCursor.length());

    if (lineToTarget.dy() < 0)
        angleToTarget = TwoPi - angleToTarget;
    angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);

    if (lineToCursor.dy() < 0)
        angleToCursor = TwoPi - angleToCursor;
    angleToCursor = normalizeAngle((Pi - angleToCursor) + Pi / 2);

    // Result difference angle between Corner Target point and Cursor Point
    auto resultAngle = angleToTarget - angleToCursor;

    QTransform trans = transform();
    trans.translate( center.x(), center.y());
    trans.rotateRadians(rotation() + resultAngle, Qt::ZAxis);
    trans.translate( -center.x(),  -center.y());
    setTransform(trans);
   // cx= boundingRect().bottomRight().x();
   // cy= boundingRect().bottomRight().y();
    cx=corner.x();//boundingRect().width();
    cy=corner.y();//-boundingRect().height();
    */
}

void DiagramItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{

    QFont f1=f;
    f1.setUnderline(false);
    painter->setFont(f1);
    painter->setBrush(myBackgroundColor);
    painter->setPen(QPen(myBorderColor, 3, Qt::SolidLine));
    if (isSelected())
        painter->setPen(QPen(QColor(255,0,0,75), 3, Qt::DashLine));
    else
        painter->setPen(QPen(myBorderColor, 3, Qt::SolidLine));

    painter->drawPolygon(myPolygon);
    setPolygon(myPolygon);
    painter->setPen(QPen(myTextColor, 3, Qt::SolidLine));
    int polarCount=0;
    if(startArrow!=0) polarCount++;
    if(endArrow!=0) polarCount++;
    if(leftArrow!=0) polarCount++;
    if(rightArrow!=0) polarCount++;

    Diagram *item=new Diagram();
    if(this->myDiagramType==Diagram::DiagramType::Start
        ||this->myDiagramType==Diagram::DiagramType::End)
    {
        //tek bağlantı olmalı
        if(polarCount>0)renkdrm= false;
        else  renkdrm= true;
    }
    if(this->myDiagramType==Diagram::DiagramType::Input
        ||this->myDiagramType==Diagram::DiagramType::Process
        ||this->myDiagramType==Diagram::DiagramType::Output
        ||this->myDiagramType==Diagram::DiagramType::Link)
    {
        if(polarCount>1)renkdrm= false;
        else  renkdrm= true;
    }
    if(this->myDiagramType==Diagram::DiagramType::Conditional)
    {
        if(polarCount>2)renkdrm= false;
        else  renkdrm= true;
    }

    if(this->myDiagramType==Diagram::DiagramType::Loop)
    {
        if(polarCount>3)renkdrm= false;
        else  renkdrm= true;
    }
    myPolygon=item->sekilStore(myDiagramType,QRectF(0,0,myDiagramWidth,myDiagramHeight));
    if(myDiagramType==Diagram::DiagramType::Start){
        label.setText("Başla");labelAlgoritma.setText("Başla");
    }
    if(myDiagramType==Diagram::DiagramType::End){
        label.setText("Son");labelAlgoritma.setText("Son");
    }
    if(myDiagramType==Diagram::DiagramType::Link){
        label.setText("Bağ");labelAlgoritma.setText("Bağ");
    }

    //if(myDiagramType==Diagram::DiagramType::Input)label.setText("Input");
    //if(myDiagramType==Diagram::DiagramType::Conditional)label.setText("Conditional");
    if(myDiagramType==Diagram::DiagramType::Loop){
        QRectF rect=this->boundingRect();
        // "In" yazısını üst merkeze yaz
        QPointF inPos(rect.center().x()+5, rect.top()-15);
        painter->drawStaticText(inPos, QStaticText("In"));
        // "Out" yazısını alt sağ köşeye yaz
        painter->drawStaticText(rect.right(),rect.bottom()-rect.height()/2-15, QStaticText("Out"));
    }

    // Çokgenin alanı
    QTextOption opt;
    opt.setAlignment(Qt::AlignCenter);
    label.setTextOption(opt);
    QRectF rect = this->boundingRect();    // label boyutunu al
    QSizeF textSize = label.size();  // QStaticText boyutu
    // Ortalanmış pozisyon (x, y)
    QPointF textPos;
    if(myDiagramType==Diagram::DiagramType::Output){
        qreal y = rect.top() + (rect.height()*0.1);
        textPos = QPointF(rect.left() + (rect.width() - textSize.width()) / 2, y);
    }
    else
    {
        qreal y = (rect.height() - textSize.height()) / 2;
        textPos = QPointF(rect.left() + (rect.width() - textSize.width()) / 2, y);
    }

    label.setTextWidth(this->boundingRect().width());
    painter->drawStaticText(textPos, label);// Metni çiz



    if(drm)
    {
         //painter->setPen(QPen(QColor(255,0,0,75), 3, Qt::DashLine));
        // renkdrm=false;
        cornerGrabber[GrabberRight]->renkdrm=renkdrm;
        cornerGrabber[GrabberLeft]->renkdrm=renkdrm;
        cornerGrabber[GrabberBottom]->renkdrm=renkdrm;
        cornerGrabber[GrabberTop]->renkdrm=renkdrm;

    }else
    {
        //painter->setPen(QPen(QColor(0,0,0,255), 3, Qt::SolidLine));
        cornerGrabber[GrabberRight]->renkdrm=renkdrm;
        cornerGrabber[GrabberLeft]->renkdrm=renkdrm;
        cornerGrabber[GrabberBottom]->renkdrm=renkdrm;
        cornerGrabber[GrabberTop]->renkdrm=renkdrm;

    }


    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
 setPositionGrabbers();



    update();

    // setVisibilityGrabbers();
}



