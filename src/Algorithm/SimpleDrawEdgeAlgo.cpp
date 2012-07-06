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
 * SimpleDrawEdgeAlgo.cpp
 *
 *  Created on: Apr 22, 2012
 *      Author: zifter
 */

#include "Algorithm/SimpleDrawEdgeAlgo.h"
#include <qmath.h>

SimpleDrawEdgeAlgo::SimpleDrawEdgeAlgo() :
		mArrowSize(10), mPenWidth(1) {
	mPenWidth = 1;
	mExtra = mPenWidth + mArrowSize;

}

SimpleDrawEdgeAlgo::~SimpleDrawEdgeAlgo() {
}

//! Перегруз области прорисовки
QRectF SimpleDrawEdgeAlgo::boundingRect(const Edge* edge) const {
	if (!edge->destNode() || !edge->sourceNode())
		return QRectF();

	return QRectF(edge->sourcePoint(),
			QSizeF(edge->destPoint().x() - edge->sourcePoint().x(), edge->destPoint().y() - edge->sourcePoint().y())).normalized().adjusted(
			-mExtra, -mExtra, mExtra, mExtra);
}

//! Перегрузка прорисовки
void SimpleDrawEdgeAlgo::paint(const Edge* edge, QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget) {
	//edge->setZValue(-10);
	if (!edge->sourceNode() || !edge->destNode())
		return;

	QLineF line(edge->sourcePoint(), edge->destPoint());
	if (qFuzzyCompare(line.length(), qreal(0.)))
		return;

	QColor myColor(Qt::black);
	QVector<QPoint> vect;
	vect.push_back(QPoint(edge->sourcePoint().x(), edge->sourcePoint().y()));
	vect.push_back(QPoint(edge->destPoint().x(), edge->destPoint().y()));
	if (edge->isSelected())
		myColor = QColor(Qt::blue);

	// Draw the line itself
	painter->setPen(QPen(myColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(line);

	// Draw the arrows
	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0)
		angle = 2 * M_PI - angle;

	QPointF destArrowP1 = edge->destPoint() + QPointF(sin(angle - M_PI / 3) * mArrowSize, cos(angle - M_PI / 3) * mArrowSize);
	QPointF destArrowP2 = edge->destPoint()
			+ QPointF(sin(angle - M_PI + M_PI / 3) * mArrowSize, cos(angle - M_PI + M_PI / 3) * mArrowSize);

	painter->setBrush(myColor);
	painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
	painter->setPen(Qt::red);
	painter->setFont(QFont("Arial", 7));
	painter->drawText(
			QPointF(edge->destPoint().x() + (edge->sourcePoint().x() - edge->destPoint().x()) / 2,
					edge->destPoint().y() + (edge->sourcePoint().y() - edge->destPoint().y()) / 2), edge->getShortText());

}

//! Определение формы объекта
QPainterPath SimpleDrawEdgeAlgo::shape(const Edge* edge) const {
	QPainterPath path;

	QLineF line(edge->sourcePoint(),  edge->destPoint());
	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0)
		angle = 2 * M_PI - angle;

	QPointF destArrowP1 = edge->sourcePoint() + QPointF(sin(angle - M_PI / 3) * mArrowSize, cos(angle - M_PI / 3) * mArrowSize);
	QPointF destArrowP2 = edge->destPoint()
			+ QPointF(sin(angle - M_PI + M_PI / 3) * mArrowSize, cos(angle - M_PI + M_PI / 3) * mArrowSize);

	path.addPolygon(
			QPolygonF() << QPoint((destArrowP1.x() + destArrowP2.x()) / 2, (destArrowP1.y() + destArrowP2.y()) / 2)
					<< destArrowP1 << destArrowP2);

	QPolygonF pol;
	pol << QPoint(edge->sourcePoint().x() - 2, edge->sourcePoint().y() - 2) << QPoint(edge->sourcePoint().x() + 2, edge->sourcePoint().y() + 2)
			<< QPoint( edge->destPoint().x() - 2,  edge->destPoint().y() - 2) << QPoint( edge->destPoint().x() + 2,  edge->destPoint().y() + 2);
	path.addPolygon(pol);
	return path;
}
