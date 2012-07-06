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
 * SimpleDrawNodeAlgo.cpp
 *
 *  Created on: Apr 22, 2012
 *      Author: zifter
 */

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "Algorithm/SimpleDrawNodeAlgo.h"
#include "Graph/Node.h"
#include "Macros.h"

SimpleDrawNodeAlgo::SimpleDrawNodeAlgo() : mBoRect(-20 - 2, -33 - 2, 100 + 2, 70 + 2)
{
}

SimpleDrawNodeAlgo::~SimpleDrawNodeAlgo() {
}

//! Перегруз области прорисовки
QRectF SimpleDrawNodeAlgo::boundingRect( const Node* node ) const  {
	return mBoRect;
}

//! Перегрузка прорисовки
void SimpleDrawNodeAlgo::paint( const Node* node, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)  {
	painter->setPen(Qt::NoPen);
	QRadialGradient gradient(-10, 0, 20);
	if (option->state & QStyle::State_Sunken) {
		gradient.setColorAt(1, QColor(Qt::white).light(120));
		gradient.setColorAt(0, QColor(Qt::gray).light(120));
	} else {
		gradient.setColorAt(0, Qt::gray);
		gradient.setColorAt(1, Qt::white);
	}
	painter->setBrush(gradient);
	painter->setPen(QPen(Qt::black, 0));
	painter->drawEllipse(-10, -10, 20, 20);


	//QRectF a(-20 - 2, -33 - 2, 50 + 2, 50 + 2);
	//painter->drawRect(a);

	if (node->isSelected()) {
		painter->setPen(QPen(Qt::blue, 1, Qt::DotLine));
		painter->setBrush(Qt::NoBrush);
		painter->drawEllipse(-12, -12, 24, 24);
		painter->setPen(QPen(Qt::blue, 1, Qt::SolidLine));
		painter->drawEllipse(-1, -1, 1, 1);
	}
	qreal adj = 0;
	if (node->getShortText().size() < 5)
		adj = 10;
	else
		adj = 0;
	painter->setPen(Qt::red);
	painter->setFont(QFont("Arial", 7));
	painter->drawText(QPointF(-20 + adj, 20), node->getShortText());
}

//! Определение формы объекта
QPainterPath SimpleDrawNodeAlgo::shape( const Node* node ) const  {
	QPainterPath path;
	path.addEllipse(-10, -10, 20, 20);
	return path;
}

