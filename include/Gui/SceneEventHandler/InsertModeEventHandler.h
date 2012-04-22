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
 * InsertModeEventHandler.h
 *
 *  Created on: Apr 19, 2012
 *      Author: zifter
 */

#ifndef INSERTMODEEVENTHANDLER_H_
#define INSERTMODEEVENTHANDLER_H_

#include "Gui/SceneEventHandler/AbstractSceneEventHandler.h"
//class QGraphicsItemGroup;

class InsertModeEventHandler : public AbstractSceneEventHandler {
public:
	explicit InsertModeEventHandler(  GESScene* sc );
	~InsertModeEventHandler();

	virtual void activate();
	virtual void deactivate();

	//! Событие нажатия кнопки
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

	//! Событие двежение зажатой мышки
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
private:
	Graph* mGraph;

	//! Текущие объекты
	QGraphicsItemGroup* mPastingGroup;
};

#endif /* INSERTMODEEVENTHANDLER_H_ */
