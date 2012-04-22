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
 * AbstractSceneEventHandler.h
 *
 *  Created on: Apr 19, 2012
 *      Author: zifter
 */

#ifndef ABSTRACTSCENEEVENTHANDLER_H_
#define ABSTRACTSCENEEVENTHANDLER_H_

#include <QGraphicsSceneMouseEvent>
#include "Gui/GESScene.h"

class AbstractSceneEventHandler {
public:
	explicit AbstractSceneEventHandler(GESScene* sc) {
		mScene = sc;
	}
	virtual ~AbstractSceneEventHandler() {
	}

	GESScene* getScene() const {
		return mScene;
	}

	void setScene(GESScene* sc) {
		mScene = sc;
	}

	GESScene::EditMode getEditMode() const {
		return GESScene::NoneMode;
	}

	virtual void activate() {
	}
	virtual void deactivate() {
	}

	//! Событие нажатия кнопки
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	}

	//! Событие двежение зажатой мышки
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	}

	//! Событие отпускание кнопки
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	}

	//! Событие двойного щелчка
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent) {
	}
	//! Событие вызова контекстного меню
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
	}
protected:
	GESScene* mScene;
};

#endif /* ABSTRACTSCENEEVENTHANDLER_H_ */
