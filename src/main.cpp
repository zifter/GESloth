/****************************************************************************
 **
 ** GESloth - Graph Editor Sloth
 ** For the latest info, see https://github.com/zifter/GESloth
 **
 ** Copyright (c) 2012 zifter
 **
 ** GESloth free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** GESloth is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

/*
 * GESView.cpp
 *
 *  Created on: Apr 15, 2012
 *      Author: zifter
 */

#include <QtGui>

#include "Gui/GESloth.h"

#include "Macros.h"

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

    GESloth widget(&app);
    widget.show();

    return app.exec();
}
