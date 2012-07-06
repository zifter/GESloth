

/*
 * AbstractDrawEdgeAlgorithm.h
 *
 *  Created on: Apr 22, 2012
 *      Author: zifter
 */

#ifndef ABSTRACTDRAWEDGEALGORITHM_H_
#define ABSTRACTDRAWEDGEALGORITHM_H_

#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

#include "Graph/Edge.h"

class AbstractDrawEdgeAlgorithm {
public:
	AbstractDrawEdgeAlgorithm();
	virtual ~AbstractDrawEdgeAlgorithm();

	//! Перегруз области прорисовки
	virtual QRectF boundingRect( const Edge* edge ) const = 0 ;

	//! Перегрузка прорисовки
	virtual void paint( const Edge* edge, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)  = 0 ;

	//! Определение формы объекта
	virtual QPainterPath shape( const Edge* edge ) const  = 0 ;
};

#endif /* ABSTRACTDRAWEDGEALGORITHM_H_ */
