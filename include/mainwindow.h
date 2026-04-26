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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "diagramitem.h"
#include <QMainWindow>
#include<QEventLoop>
#include"variableeditordialog.h"
#include<QPlainTextEdit>
#include<QToolButton>
#include<QMenu>
#include<QButtonGroup>
#include<QGraphicsView>
#include<QFontComboBox>
#include<QMatrix4x4>
#include<QFileDialog>
#include<QFileInfo>
#include<QStandardPaths>
#include<QToolBox>
#include<QProcess>
#include<QToolBar>
#include<QSplitter>
#include<QMenuBar>
#include<QMatrix2x2>
#include<QUndoStack>
#include"additemcommand.h"
#include"deletecommand.h"
QT_BEGIN_NAMESPACE
class DiagramScene;
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;

using namespace std;
QT_END_NAMESPACE


//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
   MainWindow();
    QList<DiagramItem*> diagramItems;
    QList<QString> outPolars;
    QEventLoop loop;
    QTimer timer;

    int index=0;
    DiagramItem* detectRouteItem(DiagramItem *item);
    void removeItemSafe(QGraphicsItem *item);
    DiagramItem *diagramItem;
        void loadFile(QString filePath);
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void variableUpdateRecordSlot(VariableRecord oldRec,VariableRecord newRec);
    void loadExampleFile();
    void backgroundButtonGroupClicked(QAbstractButton *button);
    void buttonGroupClicked(int id);
    void deleteItem();
    void deleteArrow(Arrow *item);
    void pointerGroupClicked(int id);
    void bringToFront();
    void sendToBack();
    void verticalAlignSlot();
    void horizontalAlignmentSlot();
    void unionItemSlot(const QString &direction);
    void maximizeSlot();
    void minimizeSlot();

    void itemInserted(Diagram::DiagramType diagramItemType);
    void textInserted(QGraphicsTextItem *item);
    void currentFontChanged(const QFont &font);
    void fontSizeChanged(const QString &size);
    void sceneScaleChanged(const QString &scale);
    void textColorChanged();
    void itemColorChanged();
    void lineColorChanged();
    void textButtonTriggered();
    void fillButtonTriggered();
    void lineButtonTriggered();
    void handleFontChange();
    void itemSelected(QGraphicsItem *item);
    void about();
    void run();
    bool runTest();
    void stop();

    void newFile();
    void saveFile();
    void saveImageFile();
    void openFile();

    void worker();


private:
    bool runState=true;
    void createToolBox();
    void createActions();
    void createMenus();
    void createToolbars();
    QWidget *createBackgroundCellWidget(const QString &text,
                                        const QString &image);
    QWidget *createCellWidget(const QString &text,
                              Diagram::DiagramType type, int w, int h);
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &image, QColor color);
    QIcon createColorIcon(QColor color);

    DiagramScene *scene;
    QGraphicsView *view;

    QAction *exitAction;
    QAction *newFileAction;
     QAction *saveFileAction;
    QAction *savePngFileAction;

     QAction *openFileAction;
    QAction *addAction;
    QAction *deleteAction;

    QAction *maximizeAction;
    QAction *minimizeAction;
    QAction *undoAction;
    QAction *unionItemEndToStartAction;
    QAction *unionItemLeftToRightAction;
    QAction *unionItemRightToLeftAction;



    QAction *redoAction;

    QAction *horizontalAlignmentAction;
    QAction *verticalAlignAction;
    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *aboutAction;
    QAction *runAction;
    QAction *stopAction;


    QMenu *fileMenu;
    QMenu *itemMenu;
    QMenu *runMenu;
    QMenu *programExample;
    QMenu *aboutMenu;

    QToolBar *textToolBar;
    QToolBar *fileToolBar;
    QToolBar *runToolBar;
    QToolBar *editToolBar;
    QToolBar *colorToolBar;
    QToolBar *pointerToolbar;
    QToolBar *viewToolbar;

    QComboBox *sceneScaleCombo;
    QComboBox *itemColorCombo;
    QComboBox *textColorCombo;
    QComboBox *fontSizeCombo;
    QFontComboBox *fontCombo;

    QToolBox *toolBoxLeft;
    QToolBox *toolBoxRight;
    QButtonGroup *buttonGroup;
    QButtonGroup *pointerTypeGroup;
    QButtonGroup *backgroundButtonGroup;
    QToolButton *fontColorToolButton;
    QToolButton *fillColorToolButton;
    QToolButton *lineColorToolButton;
    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *textAction;
    QAction *fillAction;
    QAction *lineAction;
    VariableEditorDialog *variableWidget;
    QPlainTextEdit *algoritmaText;
    QString loopStep="";
    bool loopRunState=false;
    QString apptTitle;
    QString loadFileName;
    QUndoStack *undoStack;
};
//! [0]

#endif // MAINWINDOW_H
