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
 * InsertModeEventHandler.cpp
 *
 *  Created on: Apr 19, 2012
 *      Author: zifter
 */
#include <QByteArray>
#include <QApplication>
#include <QClipboard>

#include "Gui/SceneEventHandler/InsertModeEventHandler.h"
#include "Gui/Command.h"
#include "Gui/GESScene.h"
#include "Macros.h"

#include "XML/GESFileLoader.h"

InsertModeEventHandler::InsertModeEventHandler(GESScene* sc) :
		AbstractSceneEventHandler(sc) {

}

InsertModeEventHandler::~InsertModeEventHandler() {
}

void InsertModeEventHandler::activate() {
	if (!(QApplication::clipboard()->mimeData()->hasFormat("Graph Editor Sloth/items")))
		return;
	QByteArray bt;
	bt = QApplication::clipboard()->mimeData()->data("Graph Editor Sloth/items");

	GESFileLoader loader;
	mGraph = new Graph();
	if (!loader.loadFromByte(mGraph, bt))
		loader.showError();

	QList<QGraphicsItem*> objs;

	foreach( QGraphicsItem* item, mGraph->edges() ) {
		objs.append(item);
	}

	qreal x=mGraph->nodes().at(0)->pos().x();
	qreal y=mGraph->nodes().at(0)->pos().y();

	foreach( QGraphicsItem* item, mGraph->nodes() ) {
		objs.append(item);
		if( x > item->pos().x())
			x = item->pos().x();
		if( y > item->pos().y())
					y = item->pos().y();
	}
	QGraphicsItem* mappedItem = new Node(0);
	mappedItem->setPos(x, y);

	foreach( QGraphicsItem* item, mGraph->nodes() )
		item->setPos( item->mapToItem( mappedItem, 0,0 ) );

	mPastingGroup = mScene->createItemGroup(objs);

	QGraphicsView* v = mScene->views().at(0);
	QPointF p = v->mapToScene(v->mapFromGlobal(QCursor::pos()));
	mPastingGroup->setPos(p);

	mPastingGroup->setOpacity(0.5);
}

void InsertModeEventHandler::deactivate() {
}

void InsertModeEventHandler::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	mScene->destroyItemGroup(mPastingGroup);
	if( mouseEvent->button() == Qt::LeftButton )
	{
		addItemCommand* command = new addItemCommand(mGraph, mScene->getGraph() );
		mScene->addCommand(command);
	}
	else
		delete mGraph;
	mScene->setEditMode(mScene->getPreviousEditMode());
}

void InsertModeEventHandler::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	if (mPastingGroup)
		mPastingGroup->setPos(mouseEvent->scenePos());
}
