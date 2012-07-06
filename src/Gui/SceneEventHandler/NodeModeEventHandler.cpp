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
 * NodeModeEventHandler.cpp
 *
 *  Created on: Apr 19, 2012
 *      Author: zifter
 */

#include "Gui/SceneEventHandler/NodeModeEventHandler.h"
#include "Gui/GESScene.h"
#include "Gui/Command.h"
#include "Graph/Node.h"

#include "Macros.h"

NodeModeEventHandler::NodeModeEventHandler(GESScene* sc) :
		AbstractSceneEventHandler(sc) {

}

NodeModeEventHandler::~NodeModeEventHandler() {
}

void NodeModeEventHandler::activate() {

}

void NodeModeEventHandler::deactivate() {

}

void NodeModeEventHandler::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent) {
	if (mouseEvent->button() != Qt::LeftButton)
		return;

	Node *node = new Node(0);
	node->setPos(mouseEvent->scenePos());
	node->setFlag(QGraphicsItem::ItemIsMovable, true);

	QList<QGraphicsItem*> a;
	a << node;
	addItemCommand* command = new addItemCommand(Graph::toGraph(a), mScene->getGraph() );
	mScene->addCommand(command);
}

void NodeModeEventHandler::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
	mScene->createContextMenu(event->scenePos(), event->screenPos());
}
