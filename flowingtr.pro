QT += widgets
requires(qtConfig(fontcombobox))

PREFIX = /usr

HEADERS = \
    include/DiagramScene.h \
    include/additemcommand.h \
    include/arrow.h \
    include/deletecommand.h \
    include/diagram.h \
    include/diagramitem.h \
    include/diagramtextitem.h \
    include/dotsignal.h \
    include/mainwindow.h \
    include/variable.h \
    include/variableconditiondialog.h \
    include/variableeditform.h \
    include/variableeditordialog.h \
    include/variableinputdialog.h \
    include/variableloopdialog.h \
    include/variableoutputdialog.h \
    include/variableprocessdialog.h \

SOURCES = \
    src/DiagramScene.cpp \
    src/arrow.cpp \
    src/diagramitem.cpp \
    src/diagramtextitem.cpp \
    src/dotsignal.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/variable.cpp \
    src/variableconditiondialog.cpp \
    src/variableeditform.cpp \
    src/variableeditordialog.cpp \
    src/variableinputdialog.cpp \
    src/variableloopdialog.cpp \
    src/variableoutputdialog.cpp \
    src/variableprocessdialog.cpp

RESOURCES = \
    resources/icons.qrc

# -----------------------
# INSTALL (DEBIAN SAFE)
# -----------------------

target.path = $$DESTDIR/usr/bin

desktop_file.files = assets/flowingtr.desktop
desktop_file.path = $$DESTDIR/usr/share/applications/

icon.files = resources/icons/flowingtr.svg
icon.path = $$DESTDIR/usr/share/icons/hicolor/scalable/apps

mimeicon.files = resources/icons/application-x-flowingtr-ftr.svg
mimeicon.path = $$DESTDIR/usr/share/icons/hicolor/scalable/mimetypes

mimetype.files = assets/mime/ftr.xml
mimetype.path = $$DESTDIR/usr/share/mime/packages/

doc.files = assets/doc/*.ftr
doc.path = $$DESTDIR/usr/share/flowingtr/doc/

lisanss.files = assets/lisans/*
lisanss.path = $$DESTDIR/usr/share/doc/flowingtr/

INSTALLS += target desktop_file icon doc mimetype lisanss mimeicon

DISTFILES += \
    assets/doc/5-defa-merhaba.ftr \
    assets/doc/askerlik-durumu.ftr \
    assets/doc/buyuk-kucuk.ftr \
    assets/doc/cift-sayi-toplam.ftr \
    assets/doc/dikdortgen-cevre.ftr \
    assets/doc/faktoriyel.ftr \
    assets/doc/gecti-kaldi.ftr \
    assets/doc/iki-sayi-toplama.ftr \
    assets/doc/karenin-alani.ftr \
    assets/doc/merhaba.ftr \
    assets/doc/mutlak-deger.ftr \
    assets/doc/ortalama.ftr \
    assets/doc/pozitif-negatif.ftr \
    assets/doc/rast-gele-sayi.ftr \
    assets/doc/sayi-yazi.ftr \
    assets/doc/selamlama.ftr \
    assets/doc/seri-toplam.ftr \
    assets/doc/tek-cift.ftr \
    assets/doc/tek-sayi-toplam.ftr \
    assets/doc/ucgen-turu.ftr \
    assets/flowingtr.desktop \
    assets/icons/about.png \
    assets/icons/application-x-flowingtr-ftr.svg \
    assets/icons/background1.png \
    assets/icons/background2.png \
    assets/icons/background3.png \
    assets/icons/background4.png \
    assets/icons/bold.png \
    assets/icons/bringtofront.png \
    assets/icons/close.png \
    assets/icons/delete.png \
    assets/icons/dikey.svg \
    assets/icons/floodfill.png \
    assets/icons/flowingtr.svg \
    assets/icons/italic.png \
    assets/icons/linecolor.png \
    assets/icons/linepointer.png \
    assets/icons/maximize.svg \
    assets/icons/minimize.svg \
    assets/icons/new.png \
    assets/icons/open.png \
    assets/icons/pointer.png \
    assets/icons/redo.svg \
    assets/icons/run.png \
    assets/icons/save.png \
    assets/icons/savepicture.png \
    assets/icons/sendtoback.png \
    assets/icons/stop.png \
    assets/icons/textpointer.png \
    assets/icons/underline.png \
    assets/icons/undo.svg \
    assets/icons/yatay.svg \
    assets/mime/ftr.xml
