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
 * GESTabWidget.h
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#ifndef GESTABWIDGET_H_
#define GESTABWIDGET_H_

#include <QTabWidget>
#include <QPixmap>

#include "Gui/GESPage.h"

class Graph;
class GESloth;

class GESTabWidget: public QTabWidget {
Q_OBJECT
public:
	GESTabWidget( );
	~GESTabWidget();

	void addNewPage();
	void addPage( GESPage* page );
	void removePage(int index);

	QImage* getSceneImage();

	GESPage* getCurrentPage();

public slots:

	void setScale(const QString&);

	void setSceneState(int st);

	void selectAll();

	void copyObj();

	void pasteObj();

	void redoCommand();

	void undoCommand();

	void deleteSelectedObj();

	void cutObj();

	void clear();
};

#endif /* GESTABWIDGET_H_ */
