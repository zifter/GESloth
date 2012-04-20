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
 * EdgeModeEventHanlder.h
 *
 *  Created on: Apr 19, 2012
 *      Author: zifter
 */

#ifndef EDGEMODEEVENTHANLDER_H_
#define EDGEMODEEVENTHANLDER_H_

#include "Gui/SceneEventHandler/AbstractSceneEventHandler.h"

class EdgeModeEventHanlder : public AbstractSceneEventHandler {
public:
	explicit EdgeModeEventHanlder( GESScene* sc );
	~EdgeModeEventHanlder();

	//! Событие нажатия кнопки
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

	//! Событие двежение зажатой мышки
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

	//! Событие отпускание кнопки
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

	//! Событие вызова контекстного меню
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

};

#endif /* EDGEMODEEVENTHANLDER_H_ */
