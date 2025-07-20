QT += widgets
requires(qtConfig(fontcombobox))

HEADERS	    =   mainwindow.h \
    DiagramScene.h \
		diagramitem.h \
		arrow.h \
		diagramtextitem.h \
    diagram.h \
    dotsignal.h \
    variable.h \
    variableconditiondialog.h \
    variableeditform.h \
    variableeditordialog.h \
    variableinputdialog.h \
    variableloopdialog.h \
    variableoutputdialog.h \
    variableprocessdialog.h
SOURCES	    =   mainwindow.cpp \
    DiagramScene.cpp \
		diagramitem.cpp \
		main.cpp \
		arrow.cpp \
		diagramtextitem.cpp \
    dotsignal.cpp \
    variable.cpp \
    variableconditiondialog.cpp \
    variableeditform.cpp \
    variableeditordialog.cpp \
    variableinputdialog.cpp \
    variableloopdialog.cpp \
    variableoutputdialog.cpp \
    variableprocessdialog.cpp
RESOURCES   = \
    icons.qrc


# install
target.path = /usr/bin
desktop_file.files = flowingtr.desktop
desktop_file.path = /usr/share/applications/

icon.files = images/flowingtr.svg
icon.path = /usr/share/icons/hicolor/scalable/apps

mimetype.files = ftr.xml
mimetype.path = /usr/share/mime/packages/

doc.files = doc/*.ftr
doc.path = /usr/share/flowingtr/doc/

#target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/diagramscene
INSTALLS += target desktop_file icon doc mimetype

DISTFILES += \
    aaa.md
