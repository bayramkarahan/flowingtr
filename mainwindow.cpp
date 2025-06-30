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

#include "arrow.h"
#include "diagramitem.h"
#include "DiagramScene.h"
#include "diagramtextitem.h"
#include "mainwindow.h"

#include <QtWidgets>
#include <QThread>
#include "variableeditordialog.h"
const int InsertTextButton = 10;


//! [0]
MainWindow::MainWindow()
{
    variableWidget=new VariableEditorDialog();
    connect(variableWidget, SIGNAL(variableUpdateRecord(VariableRecord,VariableRecord)),
            this, SLOT(variableUpdateRecordSlot(VariableRecord,VariableRecord)));
    createActions();
    createToolBox();
    createMenus();

    scene = new DiagramScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));

    connect(scene, SIGNAL(itemInserted(Diagram::DiagramType)),
            this, SLOT(itemInserted(Diagram::DiagramType)));

    connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)),
            this, SLOT(textInserted(QGraphicsTextItem*)));

    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
            this, SLOT(itemSelected(QGraphicsItem*)));

    connect(scene, SIGNAL(deleteItem()),
            this, SLOT(deleteItem()));


    createToolbars();

    /*QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBoxLeft);
    view = new QGraphicsView(scene);
    layout->addWidget(view);
    layout->addWidget(toolBoxRight);
    */
    view = new QGraphicsView(scene);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(toolBoxLeft);
    splitter->addWidget(view);
    splitter->addWidget(toolBoxRight);

    // İsteğe bağlı: Minimum genişlik ayarı
    toolBoxLeft->setMinimumWidth(100);
    toolBoxRight->setMinimumWidth(150);
    view->setMinimumWidth(300);  // Orta view için de gerekirse

    // Stretch ayarı (başlangıçta orta view büyük başlasın)
    splitter->setStretchFactor(0, 0);  // Sol
    splitter->setStretchFactor(1, 1);  // Orta (view)
    splitter->setStretchFactor(2, 0);  // Sağ

    // Son olarak Central Widget ayarı:
    setCentralWidget(splitter);

   // QWidget *widget = new QWidget;
   // widget->setLayout(layout);

    /************************version*******************************************/
    QStringList arguments;
    arguments << "-c" << "dpkg -s flowingtr|grep -i version";
    QString resultVersion;
    QProcess process;
    process.start("/bin/bash",arguments);
    if(process.waitForFinished())
    {
        resultVersion = process.readAll();
    }
    resultVersion.chop(1);
    QString version = resultVersion.right(5);
    apptTitle="flowingtr"+version;
    setWindowTitle(apptTitle);
   //// setCentralWidget(widget);
    setWindowIcon(QIcon(":/images/flowingtr.svg"));
    setUnifiedTitleAndToolBarOnMac(true);

    this->setWindowState(Qt::WindowMaximized);
    sceneScaleChanged("75%");
    stopAction->setEnabled(false);
   // auto  dlg=new VariableEditorDialog(this);
    //dlg->show();
}
//! [0]

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
//qDebug()<<"main move";
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
//qDebug()<<"mainpress";
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
//qDebug()<<"mainrelease";
}

//! [1]
void MainWindow::backgroundButtonGroupClicked(QAbstractButton *button)
{

    QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
    foreach (QAbstractButton *myButton, buttons) {
        if (myButton != button)
            button->setChecked(false);
    }
    QString text = button->text();
    if (text == tr("Mavi Izgara"))
        scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
    else if (text == tr("Beyaz Izgara"))
        scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
    else if (text == tr("Gri Izgara"))
        scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
    else
        scene->setBackgroundBrush(QPixmap(":/images/background4.png"));

    scene->update();
    view->update();
}
//! [1]

//! [2]
void MainWindow::buttonGroupClicked(int id)
{
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    foreach (QAbstractButton *button, buttons) {
        if (buttonGroup->button(id) != button)
            button->setChecked(false);
    }
    if (id == InsertTextButton) {
        scene->setMode(DiagramScene::InsertText);
    } else {
        scene->setItemType(Diagram::DiagramType(id));
        scene->setMode(DiagramScene::InsertItem);
    }
}
//! [2]
void MainWindow::deleteArrow(Arrow *item)
{
    scene->removeItem(item);
    Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
    arrow->startItem()->removeArrowPolar(arrow,arrow->myStartPolar);
    arrow->endItem()->removeArrowPolar(arrow,arrow->myEndPolar);
    delete item;
}

//! [3]
void MainWindow::deleteItem()
{

    bool delStatus=false;
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        delStatus=false;
        if (item->type() == Arrow::Type)
        {
            deleteArrow(qgraphicsitem_cast<Arrow *>(item));
            delStatus=true;
        }
        if(delStatus==false)
        {
            if (item->type() == DiagramItem::Type){
                // qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
                DiagramItem *ditem=qgraphicsitem_cast<DiagramItem *>(item);
                if(ditem->leftArrow!=0)deleteArrow(ditem->leftArrow);
                if(ditem->rightArrow!=0)deleteArrow(ditem->rightArrow);
                if(ditem->startArrow!=0)deleteArrow(ditem->startArrow);
                if(ditem->endArrow!=0)deleteArrow(ditem->endArrow);
                scene->removeItem(item);
                delete item;
            }
            if (item->type() == DiagramTextItem::Type){
                // qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();

                scene->removeItem(item);
                delete item;
            }
        }
    }
    scene->update();
}
//! [3]

//! [4]
void MainWindow::pointerGroupClicked(int)
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [4]

//! [5]
void MainWindow::bringToFront()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        // if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
        if (item->zValue() >= zValue)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}
//! [5]

//! [6]
void MainWindow::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        //if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
         if (item->zValue() >= zValue)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}
//! [6]

//! [7]
void MainWindow::itemInserted(Diagram::DiagramType diagramItemType)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(diagramItemType))->setChecked(false);
}
//! [7]

//! [8]
void MainWindow::textInserted(QGraphicsTextItem *)
{
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [8]

//! [9]
void MainWindow::currentFontChanged(const QFont &)
{
    handleFontChange();
}
//! [9]

//! [10]
void MainWindow::fontSizeChanged(const QString &)
{
    handleFontChange();
}
//! [10]

//! [11]
void MainWindow::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}
//! [11]

//! [12]
void MainWindow::textColorChanged()
{
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/textpointer.png",
                                     qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}
//! [12]

//! [13]
void MainWindow::itemColorChanged()
{
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png",
                                     qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}
//! [13]

//! [14]
void MainWindow::lineColorChanged()
{
    lineAction = qobject_cast<QAction *>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png",
                                     qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}
//! [14]

//! [15]
void MainWindow::textButtonTriggered()
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}
//! [15]

//! [16]
void MainWindow::fillButtonTriggered()
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}
//! [16]

//! [17]
void MainWindow::lineButtonTriggered()
{
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}
//! [17]

//! [18]
void MainWindow::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    scene->setFont(font);
}
//! [18]

//! [19]
void MainWindow::itemSelected(QGraphicsItem *item)
{
    DiagramTextItem *textItem =
    qgraphicsitem_cast<DiagramTextItem *>(item);

    QFont font = textItem->font();
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}
//! [19]

//! [20]
void MainWindow::about()
{
    QString yil=QString::number( QDateTime::currentDateTime().date().year());
    QMessageBox::about(this, tr("flowingtr"),
                       tr("Bu uygulama Linux tabanlı sistemlerde(<b>Pardus</b>); "
                          "<br/>Programlama mantığını anlama, <b>Akış Diyagramı</b> oluştumak ve"
                          "<br/><b>Gerçek Zamanlı Akış Diyagramı Çalıştırmak</b> için yazılmıştır"
                          "<br/>"
                          "<br/>*****************************************************************************"
                          "<br/>   Copyright (C) ")+yil+tr(" by Bayram KARAHAN                                    "
                          "<br/>\tkod.pardus.org.tr/karahan/flowingtr"
                          "<br/>\tgithub.com/bayramkarahan/flowingtr"
                          "<br/>\tbayramkarahan.blogspot.com"
                          "<br/>\tbayramk@gmail.com  "
                          "<br/>*****************************************************************************"

                          "<br/>flowingtr 1.0: Temel özellikler."
                          "<br/>flowingtr 1.1: Bağlantı renklendirmeleri eklendi."
                          "<br/>flowingtr 1.2: Ok ve text nesnesine sağtuş menüsü eklendi."
                          "<br/>flowingtr 1.3: Sağtuş menü özellikleri bütün nesnelere eklendi."
                          "<br/>flowingtr 1.4: Nesneler arası birleştirme iyileştirildi."
                          "<br/>flowingtr 1.5: Dinamik Değişken yapısı eklendi."
                          "<br/>flowingtr 1.6: Giriş, İşlem, Karar ve Çıktı işlemleri eklendi."
                          "<br/>flowingtr 1.7: Değişkenler ve Algoritma sağ bölüme eklendi."
                          "<br/>flowingtr 1.8: Yeni, Aç ve Kaydet seçenekleri eklendi."

                          "<br/>*****************************************************************************"
                           "<br/>   This program is free software; you can redistribute it and/or modify    "
                           "<br/>   it under the terms of the GNU General Public License as published by    "
                           "<br/>   the Free Software Foundation; either version 3 of the License, or       "
                           "<br/>   (at your option) any later version.                                     "
                           "<br/>                                                                           "
                           "<br/>   This program is distributed in the hope that it will be useful,         "
                           "<br/>   but WITHOUT ANY WARRANTY; without even the implied warranty of          "
                           "<br/>   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           "
                           "<br/>   GNU General Public License for more details.                            "
                           "<br/>                                                                           "
                           "<br/>  You should have received a copy of the GNU General Public License       "
                           "<br/>   along with this program; if not, write to the                          "
                           "<br/>  Free Software Foundation, Inc.,                                         "
                           "<br/>  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .          "));
}
//! [20]

void MainWindow::worker()
{

    diagramItem->setText("step",QColor(255,0,0,100));
    /*************************************************/
    if(!loopRunState) diagramItem->loopItemRun=false;
    if(diagramItem->myDiagramType==Diagram::DiagramType::Input)
    {
        for (int j = 0; j <diagramItem->selectedVariables.size(); ++j) {
            VariableRecord varselect =diagramItem->selectedVariables[j];
            qDebug()<<"kullanılan değişkenim:"<<varselect.label<<varselect.valueType<<varselect.value<<varselect.isInput;
            if(varselect.isInput)
            {
                bool ok = false;
                QString text;

                QDialog dialog;
                dialog.setWindowTitle("Girdi");
                dialog.setFixedSize(200,100);
                // Geçerli ekranı al (çoklu ekran varsa aktif ekranı alır)
                QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

                // Açıklama (label)
                QLabel *label = new QLabel(varselect.inputMessage+" ", &dialog);

                // Giriş kutusu
                QLineEdit *lineEdit = new QLineEdit(&dialog);
                lineEdit->setPlaceholderText("Değer giriniz");

                // Butonlar
                QPushButton *okButton = new QPushButton("Tamam", &dialog);
                QPushButton *cancelButton = new QPushButton("İptal", &dialog);

                // Buton düzeni
                QHBoxLayout *buttonLayout = new QHBoxLayout;
                buttonLayout->addStretch();
                buttonLayout->addWidget(okButton);
                buttonLayout->addWidget(cancelButton);

                // Ana düzen
                QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
                mainLayout->addWidget(label);
                mainLayout->addWidget(lineEdit);
                mainLayout->addLayout(buttonLayout);

                // Bağlantılar
                QObject::connect(okButton, &QPushButton::clicked, [&]() {
                    text = lineEdit->text();
                    ok = true;
                    dialog.accept();
                    // varselect.value = text;
                });
                QObject::connect(cancelButton, &QPushButton::clicked, [&]() {
                    dialog.reject();
                });
                // Dialog boyutunu hesaplat
               // dialog.adjustSize();

                // Ortaya taşı
                int x = (screenGeometry.width() - dialog.width()) / 2;
                int y = (screenGeometry.height() - dialog.height()) / 2;
                dialog.move(x, y);

                // Diyaloğu göster

                if (dialog.exec() == QDialog::Accepted && ok && !text.isEmpty()) {

                    varselect.value = text;
                }

            }
            for (int j = 0; j < Variable::onlineVariableList.size(); ++j) {
                const VariableRecord var = Variable::onlineVariableList[j];
                if(varselect.label==var.label)
                {
                    Variable::onlineVariableList[j].value=varselect.value;
                    variableWidget->loadVariables();
                }
            }
        }

    }
    if(diagramItem->myDiagramType==Diagram::DiagramType::Output)
    {
        for (int j = 0; j <diagramItem->selectedVariables.size(); ++j) {
            VariableRecord varselect =diagramItem->selectedVariables[j];
            qDebug()<<"kullanılan değişkenim: "<<varselect.label<<varselect.operationType<<varselect.value<<varselect.expression;
            for (int j = 0; j < Variable::onlineVariableList.size(); ++j) {
                VariableRecord var = Variable::onlineVariableList[j];
                if(varselect.label==var.label)
                {
                    QLabel *label=new QLabel;
                    if(varselect.operationType==0)
                    {
                         label->setText(QString(varselect.outputMessage+" "+var.value));
                    }
                    if(varselect.operationType==1)
                    {
                        label->setText(QString(varselect.outputMessage+var.label+"= %1").arg( var.value));
                    }
                    QDialog dialog;
                    dialog.setWindowTitle("Çıktı");
                    dialog.setFixedSize(200,100);
                    // Geçerli ekranı al (çoklu ekran varsa aktif ekranı alır)
                    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

                    // İçerik metni
                    // "Tamam" butonu
                    QPushButton *button = new QPushButton("Tamam", &dialog);
                    QObject::connect(button, &QPushButton::clicked, &dialog, &QDialog::accept);

                    // Dikey düzen yerleşimi
                    QVBoxLayout *layout = new QVBoxLayout(&dialog);
                    layout->addWidget(label);
                    layout->addWidget(button);
                    layout->setAlignment(button, Qt::AlignRight);
                    // Dialog boyutunu hesaplat
                    //dialog.adjustSize();

                    // Ortaya taşı
                    int x = (screenGeometry.width() - dialog.width()) / 2;
                    int y = (screenGeometry.height() - dialog.height()) / 2;
                    dialog.move(x, y);
                    // Diyaloğu göster
                    dialog.exec();

                }



            }
            if(varselect.operationType==2)
            {
                QDialog dialog;
                dialog.setWindowTitle("Çıktı");
                // İçerik metni
                dialog.resize(200,100);
                // Geçerli ekranı al (çoklu ekran varsa aktif ekranı alır)
                QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

                QLabel *label = new QLabel(varselect.outputMessage+varselect.expression, &dialog);

                // "Tamam" butonu
                QPushButton *button = new QPushButton("Tamam", &dialog);
                QObject::connect(button, &QPushButton::clicked, &dialog, &QDialog::accept);

                // Dikey düzen yerleşimi
                QVBoxLayout *layout = new QVBoxLayout(&dialog);
                layout->addWidget(label);
                layout->addWidget(button);
                layout->setAlignment(button, Qt::AlignRight);
                // Ortaya taşı
                int x = (screenGeometry.width() - dialog.width()) / 2;
                int y = (screenGeometry.height() - dialog.height()) / 2;
                dialog.move(x, y);
                // Diyaloğu göster
                dialog.exec();

            }
        }

    }
    if(diagramItem->myDiagramType==Diagram::DiagramType::Process)
    {
        qDebug() << diagramItem->selectedVariables.size();
        for (int j = 0; j < diagramItem->selectedVariables.size(); ++j) {
            VariableRecord varselect = diagramItem->selectedVariables[j];
           /// qDebug() << "tanımlı işlemler: "<<varselect.label << varselect.operationType << varselect.expression;

            bool test=varselect.evaluate(Variable::onlineVariableList);
            if (test) {
                for (int j = 0; j < Variable::onlineVariableList.size(); ++j) {
                    VariableRecord var = Variable::onlineVariableList[j];
                    if(varselect.label==var.label)
                    {
                        // qDebug() << "Sonuç:" << varselect.value;
                        Variable::onlineVariableList[j].value=varselect.value;
                        variableWidget->loadVariables();
                    }
                }
            }
        }
    }
    /*************************************************/
    scene->update();
    timer.start(1000);
    loop.exec();
    if(diagramItem->myDiagramType==Diagram::DiagramType::Start||
        diagramItem->myDiagramType==Diagram::DiagramType::Input||
        diagramItem->myDiagramType==Diagram::DiagramType::Process||
        //diagramItem->myDiagramType==Diagram::DiagramType::Loop||
        diagramItem->myDiagramType==Diagram::DiagramType::Conditional||
        diagramItem->myDiagramType==Diagram::DiagramType::Output)
            {
        if(diagramItem->loopItemRun==false)
        {
        if(loopRunState&&diagramItem->loopItemRun==false)
            diagramItem->loopItemRun=true;


            QString text = algoritmaText->toPlainText();
            //QStringList lines = text.contains.split('Adım');
            int lineCount = text.count("Adım");
            QString escText;
            escText=diagramItem->labelAlgoritma.text();
            //if(escText.left(4)=="<br>")
             //   escText=escText.mid(4);
            escText=escText.replace("<br>","\n");
            algoritmaText->insertPlainText("Adım"+QString::number(lineCount)+": "+escText+"\n");
          }
        }

    diagramItem->setText("step",QColor(255,255,0,0));
}
void MainWindow::stop()
{
    runState=false;
}
void MainWindow::verticalAlignSlot()
{//dikey
    scene->alignCenterVertical();
}
void MainWindow::horizontalAlignmentSlot()
{//yatay
    scene->alignCenterHorizontal();
}
void MainWindow::minimizeSlot()
{
    scene->scaleSelectedItems(0.75);
}
void MainWindow::maximizeSlot()
{
    scene->scaleSelectedItems(1.25);
}
void MainWindow::newFile()
{
    scene->clear();
}

void MainWindow::saveFile()
{
    QString defaultFileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
    + "/flowingtr.ftr";

   /* QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Diyagramı Kaydet"),
        defaultFileName,
        tr("ftr(json) Dosyaları (*.ftr)")
        );*/

    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Diyagramı Kaydet"),
        defaultFileName,
        tr("ftr (JSON) Dosyaları (*.ftr)"),
        nullptr,
        QFileDialog::DontUseNativeDialog
        );

    if (!filePath.isEmpty()) {
        scene->saveScene(filePath);
    }
}

void MainWindow::saveImageFile()
{
    QString defaultFileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
    + "/flowingtr.png";

    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Diyagramı Resim Olarak Kaydet"),
        defaultFileName,
        tr("Png Dosyaları (*.png)")
        );

    if (!filePath.isEmpty()) {
        scene->saveAsPng(filePath);
    }
}
void MainWindow::openFile()
{
    QString startDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Diyagram Aç"),
        startDir,
        tr("JSON Dosyaları (*.ftr)")
        );

    if (!filePath.isEmpty()) {
        scene->loadScene(filePath);
        variableWidget->loadVariables();
        this->setWindowTitle(apptTitle+" "+filePath);
        qDebug()<<this->windowTitle();
    }
}



bool MainWindow::runTest()
{
    qDebug()<<"test başladı";
    bool startItemHaveState=true;
    bool endItemHaveState=true;
    bool arrowHaveState=true;
    bool loopSetHaveState=true;
     foreach (QGraphicsItem *item, scene->items()) {
         if (item->type() == Arrow::Type) arrowHaveState=false;
         if (item->type() == DiagramItem::Type)
         {
             if(qgraphicsitem_cast<DiagramItem *>(item)->myDiagramType==Diagram::Start) startItemHaveState=false;
             if(qgraphicsitem_cast<DiagramItem *>(item)->myDiagramType==Diagram::End) endItemHaveState=false;
             if(qgraphicsitem_cast<DiagramItem *>(item)->myDiagramType==Diagram::Loop)
             {
                 qgraphicsitem_cast<DiagramItem *>(item)->selectedVariables.first().counter=0;
                 if(qgraphicsitem_cast<DiagramItem *>(item)->label.text()=="")
                 {
                     loopSetHaveState=false;
                   //  qDebug()<<"loop test"<<qgraphicsitem_cast<DiagramItem *>(item)->label.text();
                 }
             }

         }
     }
     if(startItemHaveState)
     {
         QMessageBox::information (0, "Hata\t","Başla Nesnesi Yok.");return false;
     }

     if(endItemHaveState)
     {QMessageBox::information (0, "Hata\t","Son Nesnesi Yok.");return false; }

      if(arrowHaveState)
      {QMessageBox::information (0, "Hata\t","Bağlantı Yok.");return false;}

      if(loopSetHaveState==false)
      {QMessageBox::information (0, "Hata\t","Döngü Düzenlenmemiş.");return false;}

      return true;
}
void MainWindow::run()
{
    algoritmaText->clear();
    runAction->setEnabled(false);
    stopAction->setEnabled(true);
   // qDebug()<<"run:"<<runTest();
    if(runTest()==false){ stopAction->setEnabled(false);
                          runAction->setEnabled(true);return;}
    runState=true;
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));



    foreach (QGraphicsItem *item, scene->items()) {
        if (item->type() == DiagramItem::Type)
        {
            if(qgraphicsitem_cast<DiagramItem *>(item)->myDiagramType==Diagram::Start)
            {
                diagramItem=qgraphicsitem_cast<DiagramItem *>(item);
                worker();
                qDebug()<<"Başla öğesi tespit edilerek akış başlatılıyor..";
            }
        }
    }

   do{
        if(runState){
        diagramItem=detectRouteItem(diagramItem);
           // worker();
        if(diagramItem!=0)worker();
            else{
            QMessageBox::information (0, "Hata\t","Akış nesnesini başka bir akış nesnesine bağlamamışsınız.\nBağlantılarınızı kontrol ediniz!!");
            runAction->setEnabled(true);
            stopAction->setEnabled(false);
            return;
        }

        }else break;

        //detectRouteItem(diagramItem)
    }while(diagramItem->myDiagramType!=Diagram::End);
    diagramItem=detectRouteItem(diagramItem);
    //worker();
    if(diagramItem!=0)worker();
        else{
        QMessageBox::information (0,"Hata\t","Akış nesnesini başka bir akış nesnesine bağlamamışsınız.\nBağlantılarınızı kontrol ediniz!!");
        runAction->setEnabled(true);
        stopAction->setEnabled(false);
        return;
    }
     runAction->setEnabled(true);
     stopAction->setEnabled(false);
     /*****************************************/
     QString text = algoritmaText->toPlainText();
     //QStringList lines = text.split('\n');
     int lineCount = text.count("Adım");
     algoritmaText->insertPlainText("Adım"+QString::number(lineCount)+": "+diagramItem->labelAlgoritma.text());

    // algoritmaText->appendPlainText("Son");
}
DiagramItem* MainWindow::detectRouteItem(DiagramItem *item)
{
    qDebug()<<"Rota Kontrol..";
    ///burada rota belirlenmesi yapılıyor.
    /// burada kıyaslama işlemleri yapılıyor
    if(item->myDiagramType==Diagram::End) return item;
    /***********************************************/
    bool result=true;
    if(item->myDiagramType==Diagram::DiagramType::Conditional)
    {
        QString expression=item->selectedVariables.first().expression;
        int operationType=item->selectedVariables.first().operationType;
        //qDebug()<<"işlem"<<item->selectedVariables.first().expression;
        //qDebug()<<"işlem türü"<<item->selectedVariables.first().operationType;
        QStringList parts = expression.split(' ');
        switch(operationType) {
        case 0: // Sabit Atama (var0 > 5)
        {
            QString var1 = parts[0].trimmed();
            QString opt1 = parts[1].trimmed();
            QString num1 = parts[2].trimmed();
            double var1val = 0, numval1 = 0;
            bool ok;
            //qDebug()<<"şart parçalanmış: "<<var1<<opt1<<num1;
            for (const auto& v : Variable::onlineVariableList) {
                if (v.label == var1) {
                    qDebug()<<"var1 değer: "<<var1<<v.value;
                    var1val = v.value.toDouble(&ok);
                }
            }
            numval1 = num1.toDouble(&ok);

            if (opt1 == "<") result = var1val < numval1;
            else if (opt1 == "<=") result = var1val <= numval1;
            else if (opt1 == ">") result = var1val > numval1;
            else if (opt1 == ">=") result = var1val >= numval1;
            else if (opt1 == "==") result = var1val == numval1;
            else if (opt1 == "!=") result = var1val != numval1;
            qDebug()<<"şart sonucu: "<<var1val<<numval1<<result;
        }
        break;
        case 1: // var1 == var2
        {
            QString var1 = parts[0].trimmed();
            QString opt1 = parts[1].trimmed();
            QString var2 = parts[2].trimmed();
            double var1val = 0, var2val = 0;
            bool ok;
            //qDebug()<<"şart parçalanmış: "<<var1<<opt1<<num1;
            for (const auto& v : Variable::onlineVariableList) {
                if (v.label == var1) {
                    qDebug()<<"var1 değer: "<<var1<<v.value;
                    var1val = v.value.toDouble(&ok);
                }
            }
            for (const auto& v : Variable::onlineVariableList) {
                if (v.label == var2) {
                    qDebug()<<"var2 değer: "<<var2<<v.value;
                    var2val = v.value.toDouble(&ok);
                }
            }

            if (opt1 == "<") result = var1val < var2val;
            else if (opt1 == "<=") result = var1val <= var2val;
            else if (opt1 == ">") result = var1val > var2val;
            else if (opt1 == ">=") result = var1val >= var2val;
            else if (opt1 == "==") result = var1val == var2val;
            else if (opt1 == "!=") result = var1val != var2val;
            qDebug()<<"şart sonucu: "<<var1val<<var2val<<result;
        }
        break;
        case 2: // var0 > 5 && var3 > var4
        {
            QString var1 = parts[0].trimmed();
            QString opt1 = parts[1].trimmed();
            QString num1 = parts[2].trimmed();
            QString logic = parts[3].trimmed();
            QString var3 = parts[4].trimmed();
            QString opt2 = parts[5].trimmed();
            QString var4 = parts[6].trimmed();

            double var1val = 0,num1val = 0, var3val = 0,var4val = 0;
            bool ok;
            //qDebug()<<"şart parçalanmış: "<<var1<<opt1<<num1;
            for (const auto& v : Variable::onlineVariableList) {
                if (v.label == var1) {
                    qDebug()<<"var1 değer: "<<var1<<v.value;
                    var1val = v.value.toDouble(&ok);
                }
                if (v.label == var3) {
                    qDebug()<<"var3 değer: "<<var3<<v.value;
                    var3val = v.value.toDouble(&ok);
                }
                if (v.label == var4) {
                    qDebug()<<"var4 değer: "<<var4<<v.value;
                    var4val = v.value.toDouble(&ok);
                }
            }

           num1val = num1.toDouble(&ok);
            bool result1=false;
            if (opt1 == "<") result1 = var1val < num1val;
            else if (opt1 == "<=") result1 = var1val <= num1val;
            else if (opt1 == ">") result1 = var1val > num1val;
            else if (opt1 == ">=") result1 = var1val >= num1val;
            else if (opt1 == "==") result1 = var1val == num1val;
            else if (opt1 == "!=") result1 = var1val != num1val;
            qDebug()<<"şart sonucu1: "<<var1val<<num1val<<result1;
            bool result2=false;
            if (opt2 == "<") result2 = var3val < var4val;
            else if (opt2 == "<=") result2 = var3val <= var4val;
            else if (opt2 == ">") result2 = var3val > var4val;
            else if (opt2 == ">=") result2 = var3val >= var4val;
            else if (opt2 == "==") result2 = var3val == var4val;
            else if (opt2 == "!=") result2 = var3val != var4val;
            qDebug()<<"şart sonucu2: "<<var3val<<var4val<<result2;

            if (logic == "&&") result = result1 && result2;
            else if (logic == "||") result = result1 || result2;


        }
        break;
        case 3: // var1 > 5 && var3 > 6
        {
            QString var1 = parts[0].trimmed();
            QString opt1 = parts[1].trimmed();
            QString num1 = parts[2].trimmed();
            QString logic = parts[3].trimmed();
            QString var3 = parts[4].trimmed();
            QString opt2 = parts[5].trimmed();
            QString num2 = parts[6].trimmed();

            double var1val = 0,num1val = 0, var3val = 0,num2val = 0;
            bool ok;
            //qDebug()<<"şart parçalanmış: "<<var1<<opt1<<num1;
            for (const auto& v : Variable::onlineVariableList) {
                if (v.label == var1) {
                    qDebug()<<"var1 değer: "<<var1<<v.value;
                    var1val = v.value.toDouble(&ok);
                }
                if (v.label == var3) {
                    qDebug()<<"var3 değer: "<<var3<<v.value;
                    var3val = v.value.toDouble(&ok);
                }
            }

            num1val = num1.toDouble(&ok);
            num2val = num2.toDouble(&ok);
            bool result1=false;
            if (opt1 == "<") result1 = var1val < num1val;
            else if (opt1 == "<=") result1 = var1val <= num1val;
            else if (opt1 == ">") result1 = var1val > num1val;
            else if (opt1 == ">=") result1 = var1val >= num1val;
            else if (opt1 == "==") result1 = var1val == num1val;
            else if (opt1 == "!=") result1 = var1val != num1val;
            qDebug()<<"şart sonucu1: "<<var1val<<num1val<<result1;
            bool result2=false;
            if (opt2 == "<") result2 = var3val < num2val;
            else if (opt2 == "<=") result2 = var3val <= num2val;
            else if (opt2 == ">") result2 = var3val > num2val;
            else if (opt2 == ">=") result2 = var3val >= num2val;
            else if (opt2 == "==") result2 = var3val == num2val;
            else if (opt2 == "!=") result2 = var3val != num2val;
            qDebug()<<"şart sonucu2: "<<var3val<<num2val<<result2;

            if (logic == "&&") result = result1 && result2;
            else if (logic == "||") result = result1 || result2;


         }
        break;
       case 4: // var0 > var1 && var2 < var
        {
             QString var1 = parts[0].trimmed();
             QString opt1 = parts[1].trimmed();
             QString var2 = parts[2].trimmed();
             QString logic = parts[3].trimmed();
             QString var3 = parts[4].trimmed();
             QString opt2 = parts[5].trimmed();
             QString var4 = parts[6].trimmed();

             double var1val = 0,var2val = 0, var3val = 0,var4val = 0;
             bool ok;
             //qDebug()<<"şart parçalanmış: "<<var1<<opt1<<num1;
             for (const auto& v : Variable::onlineVariableList) {
                 if (v.label == var1) {
                     qDebug()<<"var1 değer: "<<var1<<v.value;
                     var1val = v.value.toDouble(&ok);
                 }
                 if (v.label == var2) {
                     qDebug()<<"var2 değer: "<<var2<<v.value;
                     var2val = v.value.toDouble(&ok);
                 }
                 if (v.label == var3) {
                     qDebug()<<"var3 değer: "<<var3<<v.value;
                     var3val = v.value.toDouble(&ok);
                 }
                 if (v.label == var4) {
                     qDebug()<<"var4 değer: "<<var4<<v.value;
                     var4val = v.value.toDouble(&ok);
                 }
             }

             bool result1=false;
             if (opt1 == "<") result1 = var1val < var2val;
             else if (opt1 == "<=") result1 = var1val <= var2val;
             else if (opt1 == ">") result1 = var1val > var2val;
             else if (opt1 == ">=") result1 = var1val >= var2val;
             else if (opt1 == "==") result1 = var1val == var2val;
             else if (opt1 == "!=") result1 = var1val != var2val;
             qDebug()<<"şart sonucu1: "<<var1val<<var2val<<result1;
             bool result2=false;
             if (opt2 == "<") result2 = var3val < var4val;
             else if (opt2 == "<=") result2 = var3val <= var4val;
             else if (opt2 == ">") result2 = var3val > var4val;
             else if (opt2 == ">=") result2 = var3val >= var4val;
             else if (opt2 == "==") result2 = var3val == var4val;
             else if (opt2 == "!=") result2 = var3val != var4val;
             qDebug()<<"şart sonucu2: "<<var3val<<var4val<<result2;

             if (logic == "&&") result = result1 && result2;
             else if (logic == "||") result = result1 || result2;
        }
        break;
        }

    }
    if(item->myDiagramType==Diagram::DiagramType::Loop)
    {   bool ok;
        VariableRecord rec=item->selectedVariables.first();
        QStringList parts = rec.expression.split(' ');
        if(rec.operationType==0)
        {
            int counterVariable = rec.value.toInt(&ok,10);
            int endValue = rec.endValue;
            int stepValue = rec.stepValue;
            qDebug()<<"for"<<"label:"<<rec.label
                <<"counter:"<<rec.counter
                <<"value:"<<rec.value
                <<"endValue:"<<endValue
                <<"stepValue:"<<stepValue;
            if(rec.counter==0)
            {
                for (int j = 0; j < Variable::onlineVariableList.size(); ++j) {
                    VariableRecord var = Variable::onlineVariableList[j];
                    if(rec.label==var.label)
                    {
                        Variable::onlineVariableList[j].value=rec.startValue;
                        Variable::onlineVariableList[j].counter=0;
                        variableWidget->loadVariables();
                    }
                }
                qDebug()<<"atama yapıldı"<<rec.counter<<counterVariable;
                item->selectedVariables.first().counter++;
                item->selectedVariables.first().value=QString::number(rec.startValue);
                counterVariable = rec.startValue;

                /*********************algoritma ilk *****************************/
                QString text = algoritmaText->toPlainText();
                //QStringList lines = text.contains.split('Adım');
                int lineCount = text.count("Adım");
                QString escText;
                escText=diagramItem->labelAlgoritma.text();
                //if(escText.left(4)=="<br>")
                //   escText=escText.mid(4);
                escText=escText.replace("<br>","\n");
                algoritmaText->insertPlainText("Adım"+QString::number(lineCount)+": loop:\n");
                ///algoritmaText->insertPlainText("Adım"+QString::number(lineCount+1)+": "+escText+"\n");
                /*******************************************************/
                loopStep="Adım"+QString::number(lineCount);
                loopRunState=true;
            }else
            {
                qDebug()<<"adım sayısı:"<<rec.counter;
                counterVariable=counterVariable+stepValue;
                item->selectedVariables.first().counter++;
                item->selectedVariables.first().value=QString::number(counterVariable);
            }
            result=counterVariable<endValue;
            /*********************onlineVariableList güncelleniyor**********************/
            if(result)
            {
                for (int j = 0; j < Variable::onlineVariableList.size(); ++j) {
                    VariableRecord var = Variable::onlineVariableList[j];
                    if(rec.label==var.label)
                    {
                        Variable::onlineVariableList[j].value=item->selectedVariables.first().value;
                        Variable::onlineVariableList[j].counter=item->selectedVariables.first().counter;
                        variableWidget->loadVariables();
                    }
                }
            }else
            {
                /*********************algoritma döngü sonu*****************************/
                QString text = algoritmaText->toPlainText();
                //QStringList lines = text.contains.split('Adım');
                int lineCount = text.count("Adım");
                QString escText;
                escText=diagramItem->labelAlgoritma.text();
                escText=escText.replace("<br>","\n");
                /*******************************************************/
                loopRunState=false;
                QString sart="";//= item->selectedVariables.first().expression;
                sart = rec.label + "<" + QString::number(rec.endValue);
                algoritmaText->insertPlainText("Adım"+QString::number(lineCount)+": Eğer "+sart+" ise, "+loopStep+" Git \n");
            }
        }
        if(rec.operationType==1)
        {
            int counterVariable = rec.value.toInt(&ok,10);
            int endValue = rec.endValue;
            int stepValue = rec.stepValue;
            if(stepValue>0)stepValue=stepValue*-1;
            qDebug()<<"for"<<"label:"<<rec.label
                     <<"counter:"<<rec.counter
                     <<"value:"<<rec.value
                     <<"endValue:"<<endValue
                     <<"stepValue:"<<stepValue;
            if(rec.counter==0)
            {
                for (int j = 0; j < Variable::onlineVariableList.size(); ++j) {
                    VariableRecord var = Variable::onlineVariableList[j];
                    if(rec.label==var.label)
                    {
                        Variable::onlineVariableList[j].value=rec.startValue;
                        Variable::onlineVariableList[j].counter=0;
                        variableWidget->loadVariables();
                    }
                }
                qDebug()<<"atama yapıldı"<<rec.counter<<counterVariable;
                item->selectedVariables.first().counter++;
                item->selectedVariables.first().value=QString::number(rec.startValue);
                counterVariable = rec.startValue;

                /*********************algoritma ilk *****************************/
                QString text = algoritmaText->toPlainText();
                //QStringList lines = text.contains.split('Adım');
                int lineCount = text.count("Adım");
                QString escText;
                escText=diagramItem->labelAlgoritma.text();
                //if(escText.left(4)=="<br>")
                //   escText=escText.mid(4);
                escText=escText.replace("<br>","\n");
                algoritmaText->insertPlainText("Adım"+QString::number(lineCount)+": loop:\n");
                ///algoritmaText->insertPlainText("Adım"+QString::number(lineCount+1)+": "+escText+"\n");
                /*******************************************************/
                loopStep="Adım"+QString::number(lineCount);
                loopRunState=true;
                ///loopRunStateOtherRun=false;
                /* for (QGraphicsItem *item : scene->items()) {
                    // qDebug()<<"scene nesnesi: "<<item;
                    if (auto dItem = qgraphicsitem_cast<DiagramItem*>(item)) {
                        dItem
                    }*/
            }else
            {
                qDebug()<<"adım sayısı:"<<rec.counter;
                counterVariable=counterVariable+stepValue;
                item->selectedVariables.first().counter++;
                item->selectedVariables.first().value=QString::number(counterVariable);
            }
            result=counterVariable>endValue;
            /*********************onlineVariableList güncelleniyor**********************/
            if(result)
            {
                for (int j = 0; j < Variable::onlineVariableList.size(); ++j) {
                    VariableRecord var = Variable::onlineVariableList[j];
                    if(rec.label==var.label)
                    {
                        Variable::onlineVariableList[j].value=item->selectedVariables.first().value;
                        Variable::onlineVariableList[j].counter=item->selectedVariables.first().counter;
                        variableWidget->loadVariables();
                    }
                }
            }else
            {
                /*********************algoritma döngü sonu*****************************/
                QString text = algoritmaText->toPlainText();
                int lineCount = text.count("Adım");
                QString escText;
                escText=diagramItem->labelAlgoritma.text();
                escText=escText.replace("<br>","\n");
                /*******************************************************/
                loopRunState=false;
                QString sart="";//= item->selectedVariables.first().expression;
                sart = rec.label + ">" + QString::number(rec.endValue);
                algoritmaText->insertPlainText("Adım"+QString::number(lineCount)+": Eğer "+sart+" ise, "+loopStep+" Git \n");
            }
        }
        if(rec.operationType==2)
        {
            qDebug()<<"while"<<rec.label<<rec.expression<<parts[0]<<parts[1]<<parts[2];
            QString var1 = parts[0].trimmed();
            QString opt1 = parts[1].trimmed();
            int var1val;
            /************************************************/
            for (int j = 0; j < Variable::onlineVariableList.size(); ++j) {
                VariableRecord var = Variable::onlineVariableList[j];
                if(rec.label==var.label)
                {
                    var1val=Variable::onlineVariableList[j].value.toInt(&ok,10);
                }
            }
            /**************************************************/
            if (opt1 == "<") result = var1val < rec.endValue;
            else if (opt1 == "<=") result = var1val <= rec.endValue;
            else if (opt1 == ">") result = var1val > rec.endValue;
            else if (opt1 == ">=") result = var1val >= rec.endValue;
            else if (opt1 == "==") result = var1val == rec.endValue;
            else if (opt1 == "!=") result = var1val != rec.endValue;
            qDebug()<<"şart sonucu: "<<var1val<<rec.endValue<<result;
            if(!loopRunState) //döngü başı
            {
                /*********************algoritma ilk *****************************/
                QString text = algoritmaText->toPlainText();
                //QStringList lines = text.contains.split('Adım');
                int lineCount = text.count("Adım");
                QString escText;
                escText=diagramItem->labelAlgoritma.text();
                escText=escText.replace("<br>","\n");
                algoritmaText->insertPlainText("Adım"+QString::number(lineCount)+": loop:\n");
                 /*******************************************************/
                loopStep="Adım"+QString::number(lineCount);
                loopRunState=true;
            }
           if(!result)// döngü sonu
           {
                /*********************algoritma döngü sonu*****************************/
                QString text = algoritmaText->toPlainText();
                int lineCount = text.count("Adım");
                QString escText;
                escText=diagramItem->labelAlgoritma.text();
                escText=escText.replace("<br>","\n");
                /*******************************************************/
                loopRunState=false;
                algoritmaText->insertPlainText("Adım"+QString::number(lineCount)+": Eğer "+rec.expression+" ise, "+loopStep+" Git \n");
            }


        }
        //qDebug()<<"loop:"<<rec.name<<rec.label<<rec.value<<rec.endValue<<result;
    }

    /***********************************************/
      if(item->startArrow!=0&&item->startArrowRota=="O"
            &&item->startArrow->answer=="Y"&&result==true)
   return item->startArrow->myEndItem;

    if(item->endArrow!=0&&item->endArrowRota=="O"
            &&item->endArrow->answer=="Y"&&result==true)
        return item->endArrow->myEndItem;

    if(item->leftArrow!=0&&item->leftArrowRota=="O"
            &&item->leftArrow->answer=="Y"&&result==true)
   return item->leftArrow->myEndItem;

    if(item->rightArrow!=0&&item->rightArrowRota=="O"
            &&item->rightArrow->answer=="Y"&&result==true)
   return item->rightArrow->myEndItem;

    /*****************************************/
    if(item->startArrow!=0&&item->startArrowRota=="O"
          &&item->startArrow->answer=="N"&&result==false)
 return item->startArrow->myEndItem;

  if(item->endArrow!=0&&item->endArrowRota=="O"
          &&item->endArrow->answer=="N"&&result==false)
      return item->endArrow->myEndItem;

  if(item->leftArrow!=0&&item->leftArrowRota=="O"
          &&item->leftArrow->answer=="N"&&result==false)
 return item->leftArrow->myEndItem;

  if(item->rightArrow!=0&&item->rightArrowRota=="O"
          &&item->rightArrow->answer=="N"&&result==false)
 return item->rightArrow->myEndItem;

    // qDebug()<<"right"<<item->rightArrow->answer;
  item=0;

    return item;
}

///! [21]
void MainWindow::createToolBox()
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createCellWidget(tr("Başla"), Diagram::Start), 0, 0);
    layout->addWidget(createCellWidget(tr("Son"), Diagram::End), 0, 1);
    layout->addWidget(createCellWidget(tr("İşlem"), Diagram::Process),1, 0);
    layout->addWidget(createCellWidget(tr("Giriş"), Diagram::Input), 1, 1);
    layout->addWidget(createCellWidget(tr("Karar"), Diagram::Conditional), 2, 0);
     layout->addWidget(createCellWidget(tr("Bağlantı"), Diagram::Link), 2,1);
    layout->addWidget(createCellWidget(tr("Çıktı"), Diagram::Output), 3,0);
    layout->addWidget(createCellWidget(tr("Döngü"), Diagram::Loop), 3,1);

//!
//! [21]

    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    textButton->setFixedSize(75,40);
    textButton->setAutoRaise(true);

    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")));
    textButton->setIconSize(QSize(50, 25));
    QGridLayout *textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
    textLayout->setContentsMargins(1,0, 0,0);
    textLayout->setSpacing(1);

    QWidget *textWidget = new QWidget;
    textWidget->setFixedSize(80,60);
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 8, 0);

    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);
    layout->setContentsMargins(1,0, 0,0);
    layout->setSpacing(1);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    backgroundButtonGroup = new QButtonGroup(this);
    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));

    QGridLayout *backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Mavi Izgara"),
                                                           ":/images/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Beyaz Izgara"),
                                                           ":/images/background2.png"),1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gri Izgara"),
                                                           ":/images/background3.png"), 2, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Izgarasız"),
                                                           ":/images/background4.png"),3,0);

    //backgroundLayout->setRowStretch(2, 10);
   // backgroundLayout->setColumnStretch(2, 10);
    backgroundLayout->setContentsMargins(1,0, 0,0);
    backgroundLayout->setSpacing(1);

    QWidget *backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);


//! [22]
    toolBoxLeft = new QToolBox;
    toolBoxLeft->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBoxLeft->setMinimumWidth(itemWidget->sizeHint().width()*1.2);
    //toolBoxLeft->addItem(itemWidget, tr("Akış Şekilleri"));


    // Ana konteyner
    QWidget *mainWidget = new QWidget;
    QVBoxLayout *maintWidgetslayout = new QVBoxLayout(mainWidget);
    maintWidgetslayout->setContentsMargins(0, 0, 0, 0); // Genel boşluk yok

    // variableWidget doğrudan eklenir (kenar boşluğu yok)
    maintWidgetslayout->addWidget(itemWidget, 1);

    // QTextEdit için iç widget ve layout
    //algoritmaText=new QPlainTextEdit();

    toolBoxLeft->addItem(mainWidget, tr("Akış Şekilleri"));
    toolBoxLeft->addItem(backgroundWidget, tr("Arka Planlar"));

/*
    toolBoxRight = new QToolBox;
    toolBoxRight->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBoxRight->setMinimumWidth(variableWidget->width());
    toolBoxRight->setFixedWidth(variableWidget->width());
    toolBoxRight->addItem(variableWidget, tr("Değişkenler"));
*/
    /*
    toolBoxRight = new QToolBox;
    toolBoxRight->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBoxRight->setMinimumWidth(variableWidget->width());
    toolBoxRight->setFixedWidth(variableWidget->width());

    // Yeni bir widget ve layout oluştur
    QWidget *compositeWidget = new QWidget;
    QVBoxLayout *layout1 = new QVBoxLayout(compositeWidget);
    layout1->setContentsMargins(0, 0, 0, 0); // Sağ ve sol padding: 10px
    // variableWidget ve textEdit ekle
    layout1->addWidget(variableWidget, 1); // stretch: 1
    layout1->addWidget(new QLabel("Algoritma"), 0.1);
    layout1->addWidget(new QTextEdit, 1);  // eşit oranda yer kaplasın (ya da 2 yaparsın daha fazla yer kaplar)
    toolBoxRight->addItem(compositeWidget, tr("Değişkenler ve Açıklama"));
*/

    algoritmaText=new QPlainTextEdit();
    QWidget *textEditContainer = new QWidget;

    QWidget *variablesWidget = new QWidget;
    QVBoxLayout *variableLayout = new QVBoxLayout(variablesWidget);
    variableLayout->setContentsMargins(0, 0, 0, 0); // Sadece burada sağ/sol padding
    //variableLayout->addWidget(new QLabel("Değişkenler"));
    variableLayout->addWidget(variableWidget);
    //maintWidgetslayout->addWidget(variablesWidget, 1);

    //textEditContainer->setFixedWidth(240);
    QVBoxLayout *textEditContainerLayout1 = new QVBoxLayout(textEditContainer);
    textEditContainerLayout1->setContentsMargins(0, 0, 0, 0); // Sadece burada sağ/sol padding
    textEditContainerLayout1->addWidget(variablesWidget, 1);
    textEditContainerLayout1->addWidget(new QLabel("Algoritma"));
    textEditContainerLayout1->addWidget(algoritmaText,1);

    toolBoxRight = new QToolBox;
    toolBoxRight->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBoxRight->setMinimumWidth(textEditContainer->width());
    toolBoxRight->addItem(textEditContainer, tr("Değişkenler"));
/*
    // Ana konteyner
    QWidget *compositeWidget = new QWidget;
    QVBoxLayout *layout1 = new QVBoxLayout(compositeWidget);
    layout1->setContentsMargins(0, 0, 0, 0); // Genel boşluk yok

    // variableWidget doğrudan eklenir (kenar boşluğu yok)
    layout1->addWidget(variableWidget, 1);

    // QTextEdit için iç widget ve layout
    algoritmaText=new QPlainTextEdit();
    QWidget *textEditContainer = new QWidget;
    QVBoxLayout *textLayout1 = new QVBoxLayout(textEditContainer);
    textLayout1->setContentsMargins(0, 0, 0, 0); // Sadece burada sağ/sol padding
    textLayout1->addWidget(new QLabel("Algoritma"));
    textLayout1->addWidget(algoritmaText);

    layout1->addWidget(textEditContainer, 1); // Ekleniyor
toolBoxRight->addItem(compositeWidget, tr("Değişkenler ve Açıklama"));
*/
    // QTextEdit için iç widget ve layout


}
//! [22]

//! [23]
void MainWindow::createActions()
{
    horizontalAlignmentAction = new QAction(QIcon(":/images/yatay.svg"),
                                tr("Yatay Hizala"), this);
    horizontalAlignmentAction->setShortcut(tr("Ctrl+H"));
    horizontalAlignmentAction->setStatusTip(tr("Yatay Hizala"));
    connect(horizontalAlignmentAction, SIGNAL(triggered()), this, SLOT(horizontalAlignmentSlot()));

    verticalAlignAction = new QAction(QIcon(":/images/dikey.svg"),
                                tr("Dikey Hizala"), this);
    verticalAlignAction->setShortcut(tr("Ctrl+H"));
    verticalAlignAction->setStatusTip(tr("Dikey Hizala"));
    connect(verticalAlignAction, SIGNAL(triggered()), this, SLOT(verticalAlignSlot()));


    maximizeAction = new QAction(QIcon(":/images/maximize.svg"),
                                            tr("Büyült"), this);
    maximizeAction->setShortcut(tr("Ctrl++"));
    maximizeAction->setStatusTip(tr("Büyült"));
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(maximizeSlot()));


    minimizeAction = new QAction(QIcon(":/images/minimize.svg"),
                                            tr("Küçült"), this);
    minimizeAction->setShortcut(tr("Ctrl+-"));
    minimizeAction->setStatusTip(tr("Küçült"));
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(minimizeSlot()));




    toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("Öne Ge&tir"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Öne Getir"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));
//! [23]

    sendBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("Al&ta Al"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Alta Al"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Sil"), this);
    deleteAction->setShortcut(tr("Sil"));
    deleteAction->setStatusTip(tr("Akış Nesnesi Siler"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));


    runAction = new QAction(QIcon(":/images/run.png"), tr("&Çalıştır"), this);
    runAction->setShortcut(tr("Çalıştır"));
    runAction->setStatusTip(tr("Akış Diyagramını Çalıştırır"));
    connect(runAction, SIGNAL(triggered()), this, SLOT(run()));

    stopAction = new QAction(QIcon(":/images/stop.png"), tr("&Dur"), this);
    stopAction->setShortcut(tr("Dur"));
    stopAction->setStatusTip(tr("Çalışmayı Durdurur"));
    connect(stopAction, SIGNAL(triggered()), this, SLOT(stop()));

    saveFileAction = new QAction(QIcon(":/images/save.png"),tr("Kaydet"), this);
    saveFileAction->setShortcuts(QKeySequence::Save);
    saveFileAction->setStatusTip(tr("Diyagramı Kaydet"));
    connect(saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    savePngFileAction = new QAction(QIcon(":/images/savepicture.png"),tr("Resim Kaydet"), this);
    savePngFileAction->setShortcuts(QKeySequence::Save);
    savePngFileAction->setStatusTip(tr("Resim Kaydet"));
    connect(savePngFileAction, SIGNAL(triggered()), this, SLOT(saveImageFile()));

    newFileAction = new QAction(QIcon(":/images/new.png"),tr("Yeni"), this);
    newFileAction->setShortcuts(QKeySequence::New);
    newFileAction->setStatusTip(tr("Yeni Belge"));
    connect(newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openFileAction = new QAction(QIcon(":/images/open.png"),tr("Aç"), this);
    openFileAction->setShortcuts(QKeySequence::Open);
    openFileAction->setStatusTip(tr("Diyagramı Yükle"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));


    exitAction = new QAction(QIcon(":/images/close.png"),tr("Ç&ıkış"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Uygulamadan Çıkar"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    boldAction = new QAction(tr("Kalın"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon(":/images/italic.png"), tr("Italik"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Altı Çizgili"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    aboutAction = new QAction(QIcon(":/images/about.png"),tr("H&akkında"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

   }

//! [24]
void MainWindow::createMenus()
{

   /* newFileAction = new QAction(QIcon(":/images/new.png"),tr("Yeni"), this);
    newFileAction->setShortcuts(QKeySequence::New);
    newFileAction->setStatusTip(tr("Yeni Belge"));
    connect(newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));*/

    fileMenu = menuBar()->addMenu(tr("&Dosya"));
    fileMenu->addAction(newFileAction);
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(savePngFileAction);

    fileMenu->addAction(exitAction);
 QAction* action;
    programExample=menuBar()->addMenu(tr("&Kodlama"));

    action = programExample->addAction("1- Merhaba Dünya");
    action->setData("c1.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("2- Selamlama");
    action->setData("c2.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("3- İki Sayının Toplamı");
    action->setData("c3.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("4- İki Sayının Ortalaması");
    action->setData("c4.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("5- Dikdörtgenin Çevresi");
    action->setData("c5.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("6- Karenini Alanı");
    action->setData("c6.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("7- ---------");
    action->setData("c7.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("8- ---------");
    action->setData("c8.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("9- ---------");
    action->setData("c9.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("10- ---------");
    action->setData("c10.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("11- ---------");
    action->setData("c11.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("12- ---------");
    action->setData("c12.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);


    mathExample=menuBar()->addMenu(tr("&Matematik"));
    action = mathExample->addAction("1- Sayının İki Katı");
    action->setData("m1.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = mathExample->addAction("2- İki Sayının  Toplamı");
    action->setData("m2.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = mathExample->addAction("3- Sayının Karesi");
    action->setData("m3.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = mathExample->addAction("4- İki Sayının Ortalaması");
    action->setData("m4.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = mathExample->addAction("5- -----------");
    action->setData("m5.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = mathExample->addAction("6- ---------");
    action->setData("m6.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = mathExample->addAction("7- -----------");
    action->setData("m7.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = mathExample->addAction("8- -----------");
    action->setData("m8.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = mathExample->addAction("9- -----------");
    action->setData("m9.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = mathExample->addAction("10- -----------");
    action->setData("m10.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = mathExample->addAction("11- -----------");
    action->setData("m11.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = mathExample->addAction("12- -----------");
    action->setData("m12.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = mathExample->addAction("13- -----------");
    action->setData("m13.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = mathExample->addAction("14- -----------");
    action->setData("m14.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);


    itemMenu = menuBar()->addMenu(tr("&Düzen"));

    itemMenu->addAction(horizontalAlignmentAction);
    itemMenu->addAction(verticalAlignAction);
    itemMenu->addAction(maximizeAction);
    itemMenu->addAction(minimizeAction);
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);
    itemMenu->addSeparator();
    itemMenu->addAction(deleteAction);


    runMenu = menuBar()->addMenu(tr("&Çalıştır"));
    runMenu->addAction(runAction);
    runMenu->addAction(stopAction);



    aboutMenu = menuBar()->addMenu(tr("&Yardım"));
    aboutMenu->addAction(aboutAction);
   // aboutMenu->addAction(runAction);


}

void MainWindow::loadExampleFile()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        QString filePath ="/usr/share/flowingtr/doc/"+action->data().toString();
        qDebug()<<filePath;
        if (!filePath.isEmpty()) {
            this->setWindowTitle(apptTitle+" "+filePath);
            scene->loadScene(filePath);
            variableWidget->loadVariables();
        }

    }
}

//! [24]

//! [25]
void MainWindow::createToolbars()
{
//! [25]
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newFileAction);
    fileToolBar->addAction(openFileAction);
    fileToolBar->addAction(saveFileAction);
    fileToolBar->addAction(savePngFileAction);

    runToolBar = addToolBar(tr("Run"));
    runToolBar->addAction(runAction);
    runToolBar->addAction(stopAction);

    /***************************************************************/
    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));


    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    /*************************************************************/
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(horizontalAlignmentAction);
    editToolBar->addAction(verticalAlignAction);
    editToolBar->addAction(maximizeAction);
    editToolBar->addAction(minimizeAction);

    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);
    editToolBar->addAction(deleteAction);



    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(fontSizeChanged(QString)));

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/images/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()),
            this, SLOT(textButtonTriggered()));

//! [26]
    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()),
            this, SLOT(fillButtonTriggered()));
//! [26]

    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png", Qt::black));
    connect(lineColorToolButton, SIGNAL(clicked()),
            this, SLOT(lineButtonTriggered()));

    textToolBar = addToolBar(tr("Font"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);

    colorToolBar = addToolBar(tr("Color"));
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);

    /******************************************************/
    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("60%") << tr("70%")<< tr("75%") << tr("80%") << tr("90%") << tr("100%") << tr("110%")<< tr("120%")<< tr("130%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(3);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(sceneScaleChanged(QString)));

    viewToolbar = addToolBar(tr("View"));
    viewToolbar->addWidget(sceneScaleCombo);

    //! [27]
}
//! [27]

//! [28]
QWidget *MainWindow::createBackgroundCellWidget(const QString &text, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
//! [28]

//! [29]
QWidget *MainWindow::createCellWidget(const QString &text, Diagram::DiagramType type)
{

    // DiagramItem item(type, itemMenu);
     Diagram *item=new Diagram();
    QWidget *widget = new QWidget;
    widget->setFixedSize(80,50);
     QIcon icon(item->image(type));

    QToolButton *button = new QToolButton(widget);
     button->setToolButtonStyle(Qt::ToolButtonIconOnly);
     button->setFixedSize(75,50);
     button->setIconSize(QSize(70, 45));
     button->setIcon(icon);
     button->setCheckable(true);
     button->setAutoRaise(true);
     //button->setText(text);
     buttonGroup->addButton(button, int(type));

     QGridLayout *layout = new QGridLayout;
     layout->addWidget(button, 0, 0, Qt::AlignHCenter);
     layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);
     layout->setContentsMargins(1,0, 0,0);
     layout->setSpacing(0);
     widget->setLayout(layout);

     return widget;
}
//! [29]

//! [30]
QMenu *MainWindow::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}
//! [30]

//! [31]
QIcon MainWindow::createColorToolButtonIcon(const QString &imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    // Draw icon centred horizontally on button.
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}
//! [31]

//! [32]
QIcon MainWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}
//! [32]

void MainWindow::variableUpdateRecordSlot(VariableRecord oldRec, VariableRecord newRec)
{
    qDebug()<<"güncellenecek: "<<oldRec.label<<newRec.label<<newRec.value<<newRec.valueType;

    for (int i=0;i<Variable::onlineVariableList.size();i++) {
        if(Variable::onlineVariableList[i].label==oldRec.label)
        {
            Variable::onlineVariableList[i].label=newRec.label;
            Variable::onlineVariableList[i].value=newRec.value;
            Variable::onlineVariableList[i].valueType=newRec.valueType;
        }
    }

    for (QGraphicsItem *item : scene->items()) {
       // qDebug()<<"scene nesnesi: "<<item;
        if (auto dItem = qgraphicsitem_cast<DiagramItem*>(item)) {
            //qDebug()<<"scene nesnesi: "<<dItem->myDiagramType;
            for (int i=0;i<dItem->selectedVariables.size();i++) {
                dItem->selectedVariables[i].expression.replace(oldRec.label,newRec.label);

                QString templabel=dItem->label.text();
                dItem->label.setText(templabel.replace(oldRec.label,newRec.label));
                dItem->labelText=dItem->labelText.replace(oldRec.label,newRec.label);

                QString templabelAlgoritma=dItem->labelAlgoritma.text();
                dItem->labelAlgoritma.setText(templabelAlgoritma.replace(oldRec.label,newRec.label));
                dItem->labelAlgoritmaText=dItem->labelAlgoritmaText.replace(oldRec.label,newRec.label);


                if(dItem->selectedVariables[i].label==oldRec.label)
                {
                    dItem->selectedVariables[i].label=newRec.label;
                    dItem->selectedVariables[i].value=newRec.value;
                    dItem->selectedVariables[i].valueType=newRec.valueType;
                }
            }
        }
    }

}
