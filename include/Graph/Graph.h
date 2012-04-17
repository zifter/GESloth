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
 * Graph.h
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <QList>
#include "Macros.h"

#include "Gui/GESScene.h"

#include "Graph/Edge.h"
#include "Graph/Node.h"
#include "Graph/Object.h"


class Graph {
public:
	Graph();
	~Graph();

	void setScene(GESScene* scn );

	//! Add node in graph
	void add( Node* nd) {
		mNodes << nd;
		if( mScene != 0 )
			mScene->add( nd );
	}

	//! Add edge in graph
	void add(Edge* dg) {
		mEdges << dg;
		if( mScene != 0 )
			mScene->add( dg );
	}

	//! Add another graph
	void add( Graph* gr );

	//! Get list of all nodes in graph
	QList<Node*> nodes() const {
		return mNodes;
	}

	//! Get list of all edges in graph
	QList<Edge*> edges() const {
		return mEdges;
	}

	//! Remove edge
	bool remove( Edge* ed ){
		if( mScene != 0 )
			mScene->removeItem( ed );
		ed->del();
		return mEdges.removeOne( ed );
	}

	//! Remove node
	bool remove( Node* node );

	//! Remove graph
	bool remove( Graph* gr );

	//! Clean all graph
	void removeAll();

	//! Delete all edges, which doesn't have source and dest Node in this graph
	void fixEdge();
private:
	//! Лист вершин
	QList<Node*> mNodes;

	//! Лист ребер
	QList<Edge*> mEdges;

	GESScene* mScene;

};

#endif /* GRAPH_H_ */
