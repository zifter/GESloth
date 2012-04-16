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

#include "Edge.h"
#include "Node.h"
#include "Object.h"
#include "GESScene.h"

class Graph {
public:
	Graph();
	~Graph();

	void setScene(GESScene* scn );

	//! Add node in graph
	void add( Node* nd) {
		mNodes << nd;
		if( mScene != 0 )
			mScene->addItem( nd );
	}

	//! Add edge in graph
	void add(Edge* dg) {
		mEdges << dg;
		if( mScene != 0 )
			mScene->addItem( dg );
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

	void removeAll();
private:
	//! Лист вершин
	QList<Node*> mNodes;

	//! Лист ребер
	QList<Edge*> mEdges;

	GESScene* mScene;

};

#endif /* GRAPH_H_ */
