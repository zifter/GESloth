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

#include "Graph/Edge.h"
#include "Graph/Node.h"

#include <qmath.h>

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
		sourcePoint = line.p1() + edgeOffset;
		destPoint = line.p2() - edgeOffset;
	} else {
		sourcePoint = destPoint = line.p1();
	}
}

QRectF Edge::boundingRect() const {
	if (!source || !dest)
		return QRectF();

	qreal penWidth = 1;
	qreal extra = (penWidth + arrowSize) / 1.0;

	return QRectF(
			sourcePoint,
			QSizeF(destPoint.x() - sourcePoint.x(),
					destPoint.y() - sourcePoint.y())).normalized().adjusted(
			-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
		QWidget *) {
	setZValue(-10);
	if (!source || !dest)
		return;

	QLineF line(sourcePoint, destPoint);
	if (qFuzzyCompare(line.length(), qreal(0.)))
		return;

	QColor myColor(Qt::black);
	QVector<QPoint> vect;
	vect.push_back(QPoint(sourcePoint.x(), sourcePoint.y()));
	vect.push_back(QPoint(destPoint.x(), destPoint.y()));
	if (isSelected())
		myColor = QColor(Qt::blue);

	// Draw the line itself
	painter->setPen(
			QPen(myColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(line);

	// Draw the arrows
	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0)
		angle = 2 * M_PI - angle;

	QPointF destArrowP1 = destPoint
			+ QPointF(sin(angle - M_PI / 3) * arrowSize,
					cos(angle - M_PI / 3) * arrowSize);
	QPointF destArrowP2 = destPoint
			+ QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
					cos(angle - M_PI + M_PI / 3) * arrowSize);

	painter->setBrush(myColor);
	painter->drawPolygon(
			QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
	painter->setPen(Qt::red);
	painter->setFont(QFont("Arial", 7));
	painter->drawText(
			QPointF(destPoint.x() + (sourcePoint.x() - destPoint.x()) / 2,
					destPoint.y() + (sourcePoint.y() - destPoint.y()) / 2),
			getShortText());
}

QPainterPath Edge::shape() const {
	QPainterPath path;

	QLineF line(sourcePoint, destPoint);
	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0)
		angle = 2 * M_PI - angle;

	QPointF destArrowP1 = destPoint
			+ QPointF(sin(angle - M_PI / 3) * arrowSize,
					cos(angle - M_PI / 3) * arrowSize);
	QPointF destArrowP2 = destPoint
			+ QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
					cos(angle - M_PI + M_PI / 3) * arrowSize);

	path.addPolygon(
			QPolygonF()
					<< QPoint((destArrowP1.x() + destArrowP2.x()) / 2,
							(destArrowP1.y() + destArrowP2.y()) / 2)
					<< destArrowP1 << destArrowP2);

	QPolygonF pol;
	pol << QPoint(sourcePoint.x() - 2, sourcePoint.y() - 2)
			<< QPoint(sourcePoint.x() + 2, sourcePoint.y() + 2)
			<< QPoint(destPoint.x() - 2, destPoint.y() - 2)
			<< QPoint(destPoint.x() + 2, destPoint.y() + 2);
	path.addPolygon(pol);
	return path;
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

qreal Edge::length() {
	return sqrt(
			(destPoint.x() - sourcePoint.x())
					* (destPoint.x() - sourcePoint.x())
					+ (destPoint.y() - sourcePoint.y())
							* (destPoint.y() - sourcePoint.y()));
}
