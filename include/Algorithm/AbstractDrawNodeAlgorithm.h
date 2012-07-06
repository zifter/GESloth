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
 * AbstractDrawNodeAlgorithm.h
 *
 *  Created on: Apr 22, 2012
 *      Author: zifter
 */

#ifndef ABSTRACTDRAWNODEALGORITHM_H_
#define ABSTRACTDRAWNODEALGORITHM_H_

#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

#include "Graph/Node.h"

class AbstractDrawNodeAlgorithm {
public:
	AbstractDrawNodeAlgorithm();
	virtual ~AbstractDrawNodeAlgorithm();

	//! Перегруз области прорисовки
	virtual QRectF boundingRect(const Node* node ) const = 0 ;

	//! Перегрузка прорисовки
	virtual void paint( const Node*, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)  = 0 ;

	//! Определение формы объекта
	virtual QPainterPath shape( const Node* node ) const  = 0 ;
};

#endif /* ABSTRACTDRAWNODEALGORITHM_H_ */
