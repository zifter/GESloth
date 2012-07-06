/*
 * SimpleDrawNodeAlgo.h
 *
 *  Created on: Apr 22, 2012
 *      Author: zifter
 */

#ifndef SIMPLEDRAWNODEALGO_H_
#define SIMPLEDRAWNODEALGO_H_

#include "AbstractDrawNodeAlgorithm.h"

class Node;

class SimpleDrawNodeAlgo: public AbstractDrawNodeAlgorithm {
public:
	SimpleDrawNodeAlgo();
	virtual ~SimpleDrawNodeAlgo();

	//! Перегруз области прорисовки
	virtual QRectF boundingRect( const Node* node) const;

	//! Перегрузка прорисовки
	virtual void paint( const Node*, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	//! Определение формы объекта
	virtual QPainterPath shape( const Node* node) const;
private:
	QRectF mBoRect;
};

#endif /* SIMPLEDRAWNODEALGO_H_ */
