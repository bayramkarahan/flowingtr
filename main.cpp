#include <QApplication>
#include <QFile>
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // Argümanları al
    QStringList args = QCoreApplication::arguments();
    if (args.size() > 1) {
        QString filePath = args.at(1);
        qDebug() << "Açılacak dosya:" << filePath;

        if (QFile::exists(filePath)) {
            // MainWindow'da loadScene() gibi bir fonksiyonun olduğunu varsayıyorum
            w.loadFile(filePath);
        } else {
            qWarning() << "Dosya bulunamadı:" << filePath;
        }
    }

    w.show();
    return a.exec();
}
