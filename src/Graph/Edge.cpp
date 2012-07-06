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

#include <QPainter>
#include <qmath.h>

#include "Graph/Edge.h"
#include "Graph/Node.h"
#include "Graph/Graph.h"
#include "Algorithm/AbstractDrawEdgeAlgorithm.h"

Edge::Edge(Node *sourceNode, Node *destNode) :
		arrowSize(10) {
	source = sourceNode;
	dest = destNode;
	source->addOutEdge(this);
	dest->addInEdge(this);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setZValue(-3);
	adjust();
	setToolTip(QObject::tr("Edge"));
}

Edge::~Edge() {
}

Node *Edge::sourceNode() const {
	return source;
}

void Edge::setSourceNode(Node *node) {
	source = node;
	adjust();
}

Node *Edge::destNode() const {
	return dest;
}

void Edge::setDestNode(Node *node) {
	dest = node;
	adjust();
}

void Edge::adjust() {
	if (!source || !dest)
		return;

	QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
	qreal length = line.length();

	prepareGeometryChange();

	if (length > qreal(24.)) {
		QPointF edgeOffset((line.dx() * 10) / length,
				(line.dy() * 10) / length);
		srcPoint = line.p1() + edgeOffset;
		dstPoint = line.p2() - edgeOffset;
	} else {
		srcPoint = dstPoint = line.p1();
	}
}

QRectF Edge::boundingRect() const {
	PRINT("")
	if( !mGraph )
		return QRectF();
	return mGraph->algoEdge()->boundingRect(this);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem * op,
		QWidget *wid) {
	PRINT("")
	if( !mGraph )
		return;
	mGraph->algoEdge()->paint(this, painter, op, wid);
}

QPainterPath Edge::shape() const {
	if( !mGraph )
		return QPainterPath();
	return mGraph->algoEdge()->shape(this);
}

void Edge::del() {
	source->delOutEdge(this);
	dest->delInEdge(this);
}

bool Edge::checkEdge(Node* src, Node* dst) {
	if (source == src && dest == dst)
		return true;
	else
		return false;
}
