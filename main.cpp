#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator Translator;
    Translator.load(":/translator/Lang_ko_KR.qm");
    QApplication::installTranslator(&Translator);

    MainWindow w;
    w.show();

    return a.exec();
}
