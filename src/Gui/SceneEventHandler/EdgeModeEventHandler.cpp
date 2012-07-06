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
#include "Gui/Command.h"
#include "Graph/Node.h"
#include "Graph/Graph.h"

#include "Macros.h"

EdgeModeEventHanlder::EdgeModeEventHanlder(GESScene* sc) :
		AbstractSceneEventHandler(sc), mLine(NULL) {
}

EdgeModeEventHanlder::~EdgeModeEventHanlder() {
}

void EdgeModeEventHanlder::activate() {
	foreach (Node *node, mScene->getGraph()->nodes() )
		node->setFlag(QGraphicsItem::ItemIsMovable, false);
}

void EdgeModeEventHanlder::deactivate() {
	foreach (Node *node, mScene->getGraph()->nodes() )
		node->setFlag(QGraphicsItem::ItemIsMovable, true);
}

void EdgeModeEventHanlder::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	if (mouseEvent->button() == Qt::LeftButton && !mScene->items(mouseEvent->scenePos()).isEmpty()) {

		if (mScene->items(mouseEvent->scenePos()).at(0)->type() == Node::Type) {

			if (mLine != 0) {
				mScene->removeItem(mLine);
				delete mLine;
				mLine = 0;
			}

			mLine = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
			mLine->setPen(QPen(Qt::black, 1));
			mScene->addItem(mLine);
		}
	}
}

void EdgeModeEventHanlder::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	if (mLine == 0)
		return;
	QLineF newLine(mLine->line().p1(), mouseEvent->scenePos());
	mLine->setLine(newLine);
}

void EdgeModeEventHanlder::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	if (mouseEvent->button() != Qt::LeftButton)
		return;
	if (mLine != 0) {

		QList<QGraphicsItem *> startItems = mScene->items(mLine->line().p1());
		if (startItems.count() && startItems.first() == mLine)
			startItems.removeFirst();
		QList<QGraphicsItem *> endItems = mScene->items(mLine->line().p2());
		if (endItems.count() && endItems.first() == mLine)
			endItems.removeFirst();

		mScene->removeItem(mLine);
		delete mLine;
		mLine = NULL;

		if (startItems.count() > 0 && endItems.count() > 0 && startItems.first()->type() == Node::Type
				&& endItems.first()->type() == Node::Type && startItems.first() != endItems.first()) {
			Node* startNode = qgraphicsitem_cast<Node *>(startItems.first());
			Node* endNode = qgraphicsitem_cast<Node *>(endItems.first());
			bool flag = true;
			foreach(Edge* edge, mScene->getGraph()->edges())
				if (edge->checkEdge(startNode, endNode)) {
					flag = false;
					break;
				}
			if (flag) {
				Edge *edge = new Edge(startNode, endNode);
				QList<QGraphicsItem*> a;
				a << edge;
				addItemCommand* command = new addItemCommand(Graph::toGraph(a), mScene->getGraph());
				mScene->addCommand(command);
			}
		}
	}
	mLine = 0;

}

void EdgeModeEventHanlder::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
	mScene->createContextMenu(event->scenePos(), event->screenPos());
}
