#include "mainwindow.h"
#include <QApplication>
#include "ImageComposer.h"
#include <QtGlobal>
int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(imagecomposition);

    QApplication a(argc, argv);
    ImageComposer composer;
    composer.show();
    //MainWindow w;
    //w.show();
    return a.exec();
}
