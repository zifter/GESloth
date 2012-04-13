
#include <QtGui>

#include "GESloth.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    QFile fileIni("Graphics_Redactor.ini");
    fileIni.open(QIODevice::ReadOnly);
    QTextStream in(&fileIni);
    QString tranas = in.readAll();
    fileIni.close();

    QTranslator translator;
    translator.load("media/translations/" + tranas);
    app.installTranslator(&translator);

    GraphWidget widget(&app);
    widget.show();

    return app.exec();
}