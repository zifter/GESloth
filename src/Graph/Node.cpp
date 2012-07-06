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

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "Algorithm/SimpleDrawNodeAlgo.h"
#include "Gui/GESloth.h"

#include "Graph/Edge.h"
#include "Graph/Node.h"
#include "Graph/Graph.h"

#include "Macros.h"

Node::Node(GESloth *graphWidget) :
		graph(graphWidget) {
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setToolTip(QObject::tr("Node"));
	setZValue(-4);
}

void Node::addInEdge(Edge *edge) {
	edge->adjust();
	if (InEdgeList.contains(edge))
		return;
	InEdgeList << edge;
	edge->adjust();
}

QList<Edge *> Node::InEdges() const {
	return InEdgeList;
}

void Node::addOutEdge(Edge *edge) {
	if (OutEdgeList.contains(edge))
		return;
	OutEdgeList << edge;
}

QList<Edge *> Node::OutEdges() const {
	return OutEdgeList;
}

void Node::delOutEdge(Edge *edge) {
	OutEdgeList.removeOne(edge);
}
void Node::delInEdge(Edge *edge) {
	InEdgeList.removeOne(edge);
}

void Node::del() {
	foreach(Edge* edge, InEdges())
	{
		edge->del();
	}
	foreach(Edge* edge, OutEdges())
	{
		edge->del();
	}
}

void Node::calculateForces() {
	if (!scene() || scene()->mouseGrabberItem() == this) {
		newPos = pos();
		return;
	}

	qreal xvel = 0;
	qreal yvel = 0;
	qreal dx ;
	qreal dy ;
	Node* node;
	foreach (QGraphicsItem *item, scene()->items())
	{
		node = qgraphicsitem_cast<Node *>(item);
		if (!node)
			continue;

		QLineF line(mapFromItem(node, 0, 0), QPointF(0, 0));
		dx = line.dx();
		dy = line.dy();
		double l = 2.0 * (dx * dx + dy * dy);
		if (l > 0) {
			xvel += (dx * 50) / l;
			yvel += (dy * 50) / l;
		}
	}

	double weight = (InEdgeList.size() + 1) * 10;
	QPointF tmpPos;
	foreach (Edge *edge, InEdgeList)
	{
		//if( edge->length() < 40 )
		//	continue;
		if (edge->sourceNode() == this)
			tmpPos = mapFromItem(edge->destNode(), 0, 0);
		else
			tmpPos = mapFromItem(edge->sourceNode(), 0, 0);
		xvel += tmpPos.x() / weight;
		yvel += tmpPos.y() / weight;
	}

	foreach (Edge *edge, OutEdgeList)
	{
		//if( edge->length() < 40 )
		//			continue;
		if (edge->sourceNode() == this)
			tmpPos = mapFromItem(edge->destNode(), 0, 0);
		else
			tmpPos = mapFromItem(edge->sourceNode(), 0, 0);
		xvel += tmpPos.x() / weight;
		yvel += tmpPos.y() / weight;
	}

	if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
		xvel = yvel = 0;

	QRectF sceneRect = scene()->sceneRect();
	newPos = pos() + QPointF(xvel, yvel);
	newPos.setX(
			qMin(qMax(newPos.x(), sceneRect.left() + 20),
					sceneRect.right() - 20));
	newPos.setY(
			qMin(qMax(newPos.y(), sceneRect.top() + 20),
					sceneRect.bottom() - 20));
}

bool Node::advance() {
	if (newPos == pos())
		return false;

	setPos(newPos);
	return true;
}

QRectF Node::boundingRect() const {
	if( !mGraph )
		return QRectF();
	return mGraph->algoNode()->boundingRect(this);
}

QPainterPath Node::shape() const {
	if( !mGraph )
		return QPainterPath();
	return mGraph->algoNode()->shape(this);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *wid) {

	if( mGraph )
		mGraph->algoNode()->paint(this, painter, option, wid);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value) {
	switch (change) {
	case ItemPositionHasChanged:
		foreach (Edge *edge, InEdgeList)
			edge->adjust();
		foreach (Edge *edge, OutEdgeList)
			edge->adjust();
		//graph->itemMoved();
		break;
	default:
		break;
	}

	return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	update();
	QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	update();
	QGraphicsItem::mouseReleaseEvent(event);
}
