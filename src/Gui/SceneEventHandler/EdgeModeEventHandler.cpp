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
 * EdgeModeEventHanlder.cpp
 *
 *  Created on: Apr 19, 2012
 *      Author: zifter
 */

#include <QGraphicsLineItem>

#include "Gui/SceneEventHandler/EdgeModeEventHandler.h"
#include "Gui/GESScene.h"
#include "Graph/Node.h"
#include "Macros.h"

EdgeModeEventHanlder::EdgeModeEventHanlder( GESScene* sc ) : AbstractSceneEventHandler(sc) {
}

EdgeModeEventHanlder::~EdgeModeEventHanlder() {
}

void EdgeModeEventHanlder::mousePressEvent( QGraphicsSceneMouseEvent *mouseEvent){
	if (mouseEvent->button() == Qt::LeftButton
			&& !mScene->items(mouseEvent->scenePos()).isEmpty()) {
		if (mScene->items(mouseEvent->scenePos()).at(0)->type() == Node::Type) {
			mScene->pre_createEdge( mouseEvent->scenePos(), true ) ;
		}
	}
}

void EdgeModeEventHanlder::mouseMoveEvent( QGraphicsSceneMouseEvent *mouseEvent){
	mScene->pre_createEdge( mouseEvent->scenePos(), false ) ;
}

void EdgeModeEventHanlder::mouseReleaseEvent( QGraphicsSceneMouseEvent *mouseEvent){
	if (mouseEvent->button() == Qt::LeftButton)
		mScene->createEdge();
}

void EdgeModeEventHanlder::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
	mScene->createContextMenu( event->scenePos(), event->screenPos() );
}
