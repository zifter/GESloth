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
 * NodeModeEventHandler.h
 *
 *  Created on: Apr 19, 2012
 *      Author: zifter
 */

#ifndef NODEMODEEVENTHANDLER_H_
#define NODEMODEEVENTHANDLER_H_

#include "Gui/SceneEventHandler/AbstractSceneEventHandler.h"

class NodeModeEventHandler : public AbstractSceneEventHandler {
public:
	explicit NodeModeEventHandler( GESScene* sc );
	~NodeModeEventHandler();

	//! Событие двойного щелчка
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);

	//! Событие вызова контекстного меню
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif /* NODEMODEEVENTHANDLER_H_ */
