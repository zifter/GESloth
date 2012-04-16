/*
 * Graph.cpp
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#include "Graph.h"
#include "Macros.h"
Graph::Graph() : mScene(0) {

}

Graph::~Graph() {
}

void Graph::add(Graph* gr) {
	foreach(Edge* edge, gr->edges())
	{
		if( !mEdges.contains( edge ) ){
			edge->destNode()->addInEdge(edge);
			edge->sourceNode()->addOutEdge(edge);
			add(edge);
		}
	}
	foreach(Node* nd, gr->nodes())
	{
		if( !mNodes.count( nd ) )
				add( nd );
	}
}

bool Graph::remove( Node* node ){
	if( mScene != 0 )
		mScene->removeItem( node );
	foreach(Edge* edge, node->OutEdges())
	{
		remove(edge);
	}
	foreach(Edge* edge, node->InEdges())
	{
		remove( edge );
	}
	node->del();
	return mNodes.removeOne( node );
}

bool Graph::remove(Graph* gr) {
	foreach(Node* node, gr->nodes())
	{
		if( mNodes.contains( node ) ){
			remove(node);
		}
	}

	foreach(Edge* edge, gr->edges())
	{
		if( mEdges.contains( edge ) ){
			remove(edge);
		}
	}

	return true;

}
