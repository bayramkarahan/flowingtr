#include "../include/mainwindow.h"
#include "../include/arrow.h"
#include "../include/diagramitem.h"
#include "../include/DiagramScene.h"
#include "../include/diagramtextitem.h"
#include <QtWidgets>
#include <QThread>
const int InsertTextButton = 10;


//! [0]
MainWindow::MainWindow()
{
    //qDebug() << QIcon(":/icons/undo.svg").isNull();
    undoStack = new QUndoStack(this);

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
    setWindowIcon(QIcon(":/icons/flowingtr.svg"));
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
        scene->setBackgroundBrush(QPixmap(":/icons/background1.png"));
    else if (text == tr("Beyaz Izgara"))
        scene->setBackgroundBrush(QPixmap(":/icons/background2.png"));
    else if (text == tr("Gri Izgara"))
        scene->setBackgroundBrush(QPixmap(":/icons/background3.png"));
    else
        scene->setBackgroundBrush(QPixmap(":/icons/background4.png"));

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
        //undoStack->push(new AddItemCommand(scene, Diagram::DiagramType(id)));
    } else {

        if(Diagram::DiagramType(id)==Diagram::DiagramType::Input)
        {scene->myDiagramWidth=200;scene->myDiagramHeight=90;
        }
        if(Diagram::DiagramType(id)==Diagram::DiagramType::Start)
        {scene->myDiagramWidth=100;scene->myDiagramHeight=60;}

        if(Diagram::DiagramType(id)==Diagram::DiagramType::End)
        {scene->myDiagramWidth=100;scene->myDiagramHeight=60;}

        if(Diagram::DiagramType(id)==Diagram::DiagramType::Process)
        {scene->myDiagramWidth=200;scene->myDiagramHeight=90;}

        if(Diagram::DiagramType(id)==Diagram::DiagramType::Conditional)
        {scene->myDiagramWidth=200;scene->myDiagramHeight=90;}

        if(Diagram::DiagramType(id)==Diagram::DiagramType::Loop)
        {scene->myDiagramWidth=200;scene->myDiagramHeight=75;}

        if(Diagram::DiagramType(id)==Diagram::DiagramType::Output)
        {scene->myDiagramWidth=200;scene->myDiagramHeight=90;}

        if(Diagram::DiagramType(id)==Diagram::DiagramType::Link)
        {scene->myDiagramWidth=50;scene->myDiagramHeight=50;}

        scene->setItemType(Diagram::DiagramType(id));
        scene->setMode(DiagramScene::InsertItem);
        //undoStack->push(new AddItemCommand(scene, Diagram::DiagramType(id)));
    }
}

void MainWindow::deleteArrow(Arrow *item)
{
    if (!item) return;

    Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);

    if (arrow->startItem())
        arrow->startItem()->removeArrowPolar(arrow, arrow->myStartPolar);

    if (arrow->endItem())
        arrow->endItem()->removeArrowPolar(arrow, arrow->myEndPolar);

    delete arrow;
}

void MainWindow::removeItemSafe(QGraphicsItem *item)
{
    if (!item) return;

    // Arrow için
    if (item->type() == Arrow::Type) {
        Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
        if (!arrow) return;

        // Start ve End ilişkilerini kaldır
        if (arrow->startItem())
            arrow->startItem()->removeArrowPolar(arrow, arrow->myStartPolar);
        if (arrow->endItem())
            arrow->endItem()->removeArrowPolar(arrow, arrow->myEndPolar);

        // Sahneden çıkar ve güvenle sil
        if (arrow->scene())
            arrow->scene()->removeItem(arrow);
        delete arrow;
        return;
    }

    // DiagramItem için
    if (item->type() == DiagramItem::Type) {
        DiagramItem *ditem = qgraphicsitem_cast<DiagramItem *>(item);
        if (!ditem) return;

        // Bağlı okları sil
        if (ditem->leftArrow) {
            removeItemSafe(ditem->leftArrow);
            ditem->leftArrow = nullptr;
        }
        if (ditem->rightArrow) {
            removeItemSafe(ditem->rightArrow);
            ditem->rightArrow = nullptr;
        }
        if (ditem->startArrow) {
            removeItemSafe(ditem->startArrow);
            ditem->startArrow = nullptr;
        }
        if (ditem->endArrow) {
            removeItemSafe(ditem->endArrow);
            ditem->endArrow = nullptr;
        }

        if (ditem->scene())
        {
            //ditem->scene()->removeItem(ditem);
             undoStack->push(new DeleteCommand(scene, item));
        }
        //delete ditem;
        return;
    }

    // DiagramTextItem veya diğerleri için
    if (item->scene())
        item->scene()->removeItem(item);
    delete item;
}

void MainWindow::deleteItem()
{
    QList<QGraphicsItem*> selected = scene->selectedItems();
    if (selected.isEmpty()) return;



   // QList<QGraphicsItem*> selected = scene->selectedItems();
    for (QGraphicsItem *item : selected) {
        if (item->type() == Arrow::Type)
        {
            deleteArrow(qgraphicsitem_cast<Arrow *>(item));
        }
    }
    QList<QGraphicsItem*> selectedItem = scene->selectedItems();

    for (QGraphicsItem *item : selectedItem) {
        if (item->type() == DiagramItem::Type){
            removeItemSafe(item);
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
    DiagramItem *item = scene->lastCreatedItem;
    if (!item)
        return;
    undoStack->push(new AddItemCommand(scene, item));

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
    double newScale = scale.left(scale.indexOf("%")).toDouble() / 100.0;

    QTransform oldTransform = view->transform();

    view->resetTransform();
    view->translate(oldTransform.dx(), oldTransform.dy());
    view->scale(newScale, newScale);
}
//! [11]

//! [12]
void MainWindow::textColorChanged()
{
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/icons/textpointer.png",
                                     qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}
//! [12]

//! [13]
void MainWindow::itemColorChanged()
{
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/icons/floodfill.png",
                                     qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}
//! [13]

//! [14]
void MainWindow::lineColorChanged()
{
    lineAction = qobject_cast<QAction *>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/icons/linecolor.png",
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
    QString yil = QString::number(QDate::currentDate().year());

    QString aboutText = QStringLiteral(R"(
<b>FlowingTR</b> - Akış Diyagram Uygulaması<br>
Sürüm: 3.2.0<br><br>

Bu uygulama Linux tabanlı sistemlerde (<b>Pardus</b>) çalışır;<br>
Programlama mantığını anlama, <b>Akış Diyagramı</b> oluşturmak ve<br>
<b>Gerçek Zamanlı Akış Diyagramı Çalıştırmak</b> için geliştirilmiştir.<br><br>

*****************************************************************************<br>
Built with Qt (LGPLv3)<br>
© The Qt Company Ltd<br>
*****************************************************************************<br>

Copyright (C) %1 Bayram KARAHAN<br>
github.com/bayramkarahan/flowingtr<br>
bayramkarahan.blogspot.com<br>
bayramk@gmail.com<br>
*****************************************************************************<br>
This program is free software: you can redistribute it and/or modify it under the terms of the<br>
GNU General Public License as published by the Free Software Foundation, either version 3 of the License,<br>
or (at your option) any later version.<br><br>
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;<br>
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.<br>
See the GNU General Public License for more details.<br><br>
You should have received a copy of the GNU General Public License along with this program;<br>
if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.
)").arg(yil);

    QMessageBox::about(this, tr("FlowingTR Hakkında"), aboutText);
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
            //qDebug()<<"kullanılan değişkenim:"<<varselect.label<<varselect.valueType<<varselect.value<<varselect.isInput;
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
           /// qDebug()<<"kullanılan değişkenim: "<<varselect.label<<varselect.operationType<<varselect.value<<varselect.expression;
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
    scene->scaleSelectedItems(0.9);
}
void MainWindow::maximizeSlot()
{
    scene->scaleSelectedItems(1.1);
}
void MainWindow::newFile()
{
    scene->clear();
            loadFileName="";
}

void MainWindow::saveFile()
{
    QString defaultFileName;
    if(loadFileName=="")
    {
    defaultFileName= QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
    + "/flowingtr.ftr";
    }
    else
    {
        defaultFileName= QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
        + "/"+loadFileName;
    }

    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Diyagramı Kaydet"),
        defaultFileName,
        tr("ftr (JSON) Dosyaları (*.ftr)"),
        nullptr,
        QFileDialog::DontUseNativeDialog
        );

    if (!filePath.isEmpty()) {
        QFileInfo fi(filePath);
        if (fi.suffix().isEmpty()) {
            filePath += ".ftr"; // Uzantı yoksa .ftr ekle
        }
        scene->saveScene(filePath);
        loadFileName=filePath;
        this->setWindowTitle(apptTitle+" "+filePath);
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
        loadFileName=filePath;
        qDebug()<<this->windowTitle()<<loadFileName;
    }
}

void MainWindow::loadFile(QString filePath)
{


    if (!filePath.isEmpty()) {
        scene->loadScene(filePath);
        variableWidget->loadVariables();
        this->setWindowTitle(apptTitle+" "+filePath);
        loadFileName=filePath;
        qDebug()<<this->windowTitle()<<loadFileName;
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
                       // Variable::onlineVariableList[j].value=rec.startValue;
                        Variable::onlineVariableList[j].value = QString::number(rec.startValue);
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
                        //Variable::onlineVariableList[j].value=rec.startValue;
                        Variable::onlineVariableList[j].value = QString::number(rec.startValue);
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
   // connect(buttonGroup, SIGNAL(buttonClicked(int)),
    //        this, SLOT(buttonGroupClicked(int)));

    connect(buttonGroup, &QButtonGroup::idClicked,
            this, &MainWindow::buttonGroupClicked);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createCellWidget(tr("Başla"), Diagram::Start,100,75), 0, 0);
    layout->addWidget(createCellWidget(tr("Son"), Diagram::End,100,75), 0, 1);
    layout->addWidget(createCellWidget(tr("İşlem"), Diagram::Process,200,100),1, 0);
    layout->addWidget(createCellWidget(tr("Giriş"), Diagram::Input,200,100), 1, 1);
    layout->addWidget(createCellWidget(tr("Karar"), Diagram::Conditional,200,100), 2, 0);
     layout->addWidget(createCellWidget(tr("Bağlantı"), Diagram::Link,20,20), 2,1);
    layout->addWidget(createCellWidget(tr("Çıktı"), Diagram::Output,200,90), 3,0);
    layout->addWidget(createCellWidget(tr("Döngü"), Diagram::Loop,200,75), 3,1);

//!
//! [21]

    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    textButton->setFixedSize(75,40);
    textButton->setAutoRaise(true);

    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/icons/textpointer.png")));
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
                                                           ":/icons/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Beyaz Izgara"),
                                                           ":/icons/background2.png"),1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gri Izgara"),
                                                           ":/icons/background3.png"), 2, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Izgarasız"),
                                                           ":/icons/background4.png"),3,0);

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
    undoAction = new QAction(QIcon(":/icons/undo.svg"),
                                tr("Geri Al"), this);

    undoAction->setShortcut(tr("Ctrl+Z"));
    undoAction->setStatusTip(tr("Geri Al"));
    connect(undoAction, &QAction::triggered,undoStack, &QUndoStack::undo);

    redoAction = new QAction(QIcon(":/icons/redo.svg"),
                                tr("İleri Al"), this);

    redoAction->setShortcut(tr("Ctrl+Y"));
    redoAction->setStatusTip(tr("İleri Al"));
    connect(redoAction, &QAction::triggered,undoStack, &QUndoStack::redo);


    horizontalAlignmentAction = new QAction(QIcon(":/icons/yatay.svg"),
                                tr("Yatay Hizala"), this);

    horizontalAlignmentAction->setShortcut(tr("Ctrl+H"));
    horizontalAlignmentAction->setStatusTip(tr("Yatay Hizala"));
    connect(horizontalAlignmentAction, SIGNAL(triggered()), this, SLOT(horizontalAlignmentSlot()));

    verticalAlignAction = new QAction(QIcon(":/icons/dikey.svg"),
                                tr("Dikey Hizala"), this);
    verticalAlignAction->setShortcut(tr("Ctrl+H"));
    verticalAlignAction->setStatusTip(tr("Dikey Hizala"));
    connect(verticalAlignAction, SIGNAL(triggered()), this, SLOT(verticalAlignSlot()));


    maximizeAction = new QAction(QIcon(":/icons/maximize.svg"),
                                            tr("Büyült"), this);
    maximizeAction->setShortcut(tr("Ctrl++"));
    maximizeAction->setStatusTip(tr("Büyült"));
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(maximizeSlot()));


    minimizeAction = new QAction(QIcon(":/icons/minimize.svg"),
                                            tr("Küçült"), this);
    minimizeAction->setShortcut(tr("Ctrl+-"));
    minimizeAction->setStatusTip(tr("Küçült"));
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(minimizeSlot()));




    toFrontAction = new QAction(QIcon(":/icons/bringtofront.png"),
                                tr("Öne Ge&tir"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Öne Getir"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));
//! [23]

    sendBackAction = new QAction(QIcon(":/icons/sendtoback.png"), tr("Al&ta Al"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Alta Al"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon(":/icons/delete.png"), tr("&Sil"), this);
    deleteAction->setShortcut(tr("Sil"));
    deleteAction->setStatusTip(tr("Akış Nesnesi Siler"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));


    runAction = new QAction(QIcon(":/icons/run.png"), tr("&Çalıştır"), this);
    runAction->setShortcut(tr("Çalıştır"));
    runAction->setStatusTip(tr("Akış Diyagramını Çalıştırır"));
    connect(runAction, SIGNAL(triggered()), this, SLOT(run()));

    stopAction = new QAction(QIcon(":/icons/stop.png"), tr("&Dur"), this);
    stopAction->setShortcut(tr("Dur"));
    stopAction->setStatusTip(tr("Çalışmayı Durdurur"));
    connect(stopAction, SIGNAL(triggered()), this, SLOT(stop()));

    saveFileAction = new QAction(QIcon(":/icons/save.png"),tr("Kaydet"), this);
    saveFileAction->setShortcuts(QKeySequence::Save);
    saveFileAction->setStatusTip(tr("Diyagramı Kaydet"));
    connect(saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    savePngFileAction = new QAction(QIcon(":/icons/savepicture.png"),tr("Resim Kaydet"), this);
    savePngFileAction->setShortcuts(QKeySequence::Save);
    savePngFileAction->setStatusTip(tr("Resim Kaydet"));
    connect(savePngFileAction, SIGNAL(triggered()), this, SLOT(saveImageFile()));

    newFileAction = new QAction(QIcon(":/icons/new.png"),tr("Yeni"), this);
    newFileAction->setShortcuts(QKeySequence::New);
    newFileAction->setStatusTip(tr("Yeni Belge"));
    connect(newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openFileAction = new QAction(QIcon(":/icons/open.png"),tr("Aç"), this);
    openFileAction->setShortcuts(QKeySequence::Open);
    openFileAction->setStatusTip(tr("Diyagramı Yükle"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));


    exitAction = new QAction(QIcon(":/icons/close.png"),tr("Ç&ıkış"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Uygulamadan Çıkar"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    boldAction = new QAction(tr("Kalın"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/icons/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon(":/icons/italic.png"), tr("Italik"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/icons/underline.png"), tr("Altı Çizgili"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    aboutAction = new QAction(QIcon(":/icons/about.png"),tr("H&akkında"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

   }

//! [24]
void MainWindow::createMenus()
{

   /* newFileAction = new QAction(QIcon(":/icons/new.png"),tr("Yeni"), this);
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
    programExample=menuBar()->addMenu(tr("&Örnekler"));

    action = programExample->addAction("1- Merhaba Dünya");
    action->setData("merhaba.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("2- Selamlama");
    action->setData("selamlama.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("3- İki Sayının Toplamı");
    action->setData("iki-sayi-toplama.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("4- İki Sayının Ortalaması");
    action->setData("ortalama.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("5- Dikdörtgenin Çevresi");
    action->setData("dikdortgen-cevre.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("6- Karenini Alanı");
    action->setData("karenin-alani.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("7- Beş Defa Merhaba");
    action->setData("5-defa-merhaba.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("8- Seri Toplamı");
    action->setData("seri-toplam.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("9- Dersten Geçme Durumu");
    action->setData("gecti-kaldi.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("10- Sayının Pozitif-Negatif Testi");
    action->setData("pozitif-negatif.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("11- Büyük Sayıyı Bulma");
    action->setData("buyuk-kucuk.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("12- Tek-Çift Sayı Tespiti");
    action->setData("tek-cift.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("13- Askerlik Durumu");
    action->setData("askerlik-durumu.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("14- Mutlak Değer");
    action->setData("mutlak-deger.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("15- 5'in Faktoriyeli");
    action->setData("faktoriyel.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("16- Üçgen Türü");
    action->setData("ucgen-turu.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("17- Rast Gele Sayı");
    action->setData("rast-gele-sayi.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("18- 10'a Kadar Olan Çift Sayılar Toplamı");
    action->setData("cift-sayi-toplam.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("19- 10'a Kadar Olan Tek Sayılar Toplamı");
    action->setData("teksayitoplam.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);

    action = programExample->addAction("20- 1-5 Arasındaki Sayıyı Yazıya Çevirme");
    action->setData("sayi-yazi.ftr");
    connect(action, &QAction::triggered, this, &MainWindow::loadExampleFile);


    itemMenu = menuBar()->addMenu(tr("&Düzen"));
    itemMenu->addAction(undoAction);
    itemMenu->addAction(redoAction);
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
            loadFileName=filePath;
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
    pointerButton->setIcon(QIcon(":/icons/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/icons/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    //connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
    //        this, SLOT(pointerGroupClicked(int)));
    connect(pointerTypeGroup, &QButtonGroup::idClicked,
            this, &MainWindow::pointerGroupClicked);

    connect(buttonGroup, SIGNAL(idClicked(int)),
            this, SLOT(buttonGroupClicked(int)));

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
    editToolBar->addAction(undoAction);
    editToolBar->addAction(redoAction);

    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 1)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    fontSizeCombo->setCurrentText("12");
    //connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
    //        this, SLOT(fontSizeChanged(QString)));
    connect(fontSizeCombo, &QComboBox::currentTextChanged,
            this, &MainWindow::fontSizeChanged);

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/icons/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()),
            this, SLOT(textButtonTriggered()));

//! [26]
    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/icons/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()),
            this, SLOT(fillButtonTriggered()));
//! [26]

    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/icons/linecolor.png", Qt::black));
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
    //connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
    //        this, SLOT(sceneScaleChanged(QString)));
    connect(sceneScaleCombo, &QComboBox::currentTextChanged,
            this, &MainWindow::sceneScaleChanged);

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
QWidget *MainWindow::createCellWidget(const QString &text, Diagram::DiagramType type,int w, int h)
{

    // DiagramItem item(type, itemMenu);
     Diagram *item=new Diagram();
    QWidget *widget = new QWidget;
    widget->setFixedSize(80,50);
     QIcon icon(item->image(type,w,h));

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

    QList<QColor> colors = {
        Qt::black, Qt::white, Qt::red, Qt::green, Qt::blue,
        Qt::yellow, Qt::cyan, Qt::magenta, Qt::gray, Qt::darkRed,
        Qt::darkGreen, Qt::darkBlue, Qt::darkCyan, Qt::darkMagenta,
        Qt::darkYellow, Qt::lightGray, Qt::transparent
    };

   QStringList names = {
       tr("Siyah"),       // Black
       tr("Beyaz"),       // White
       tr("Kırmızı"),     // Red
       tr("Yeşil"),       // Green
       tr("Mavi"),        // Blue
       tr("Sarı"),        // Yellow
       tr("Cam Göbeği"),  // Cyan
       tr("Eflatun"),     // Magenta
       tr("Gri"),         // Gray
       tr("Koyu Kırmızı"),// Dark Red
       tr("Koyu Yeşil"),  // Dark Green
       tr("Koyu Mavi"),   // Dark Blue
       tr("Koyu Cam Göbeği"), // Dark Cyan
       tr("Koyu Eflatun"),    // Dark Magenta
       tr("Koyu Sarı"),       // Dark Yellow
       tr("Açık Gri"),        // Light Gray
       tr("Saydam")           // Transparent
   };

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
    qDebug()<<"güncellenecek: "<<oldRec.label<<newRec.label<<newRec.value<<newRec.valueType<<newRec.isSecret;

    for (int i=0;i<Variable::onlineVariableList.size();i++) {
        if(Variable::onlineVariableList[i].label==oldRec.label)
        {
            Variable::onlineVariableList[i].label=newRec.label;
            Variable::onlineVariableList[i].value=newRec.value;
            Variable::onlineVariableList[i].valueType=newRec.valueType;
            Variable::onlineVariableList[i].isSecret=newRec.isSecret;
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
                    dItem->selectedVariables[i].isSecret=newRec.isSecret;
                }
            }
        }
    }

}
