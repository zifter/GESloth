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
 * Graph.cpp
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#include "Graph/Graph.h"
#include "Macros.h"
#include "Algorithm/SimpleDrawEdgeAlgo.h"
#include "Algorithm/SimpleDrawNodeAlgo.h"

Graph::Graph() :
		mScene(0) {

	mEdgeAlgo = new SimpleDrawEdgeAlgo();
	mNodeAlgo = new SimpleDrawNodeAlgo();

}

Graph::~Graph() {
	foreach(Node* node, mNodes ) {
		delete node;
	}
	foreach(Edge* edge, mEdges ) {
		delete edge;
	}
}

void Graph::setScene(GESScene* scn) {
	mScene = scn;
	foreach(Node* node, mNodes ) {
		mScene->addItem(node);
	}
	foreach(Edge* edge, mEdges ) {
		mScene->addItem(edge);
	}
}

//! Add node in graph
void Graph::add(Node* nd) {
	mNodes << nd;
	if (mScene != 0 && mScene != nd->scene())
		mScene->add(nd);
	nd->setGraph(this);
}

//! Add edge in graph
void Graph::add(Edge* dg) {
	mEdges << dg;
	if (mScene != 0 && mScene != dg->scene())
		mScene->add(dg);

	dg->setGraph(this);
}

void Graph::add(Graph* gr) {
	foreach(Edge* edge, gr->edges()) {
		if (!mEdges.contains(edge)) {
			edge->destNode()->addInEdge(edge);
			edge->sourceNode()->addOutEdge(edge);
			add(edge);
		}
	}
	foreach(Node* nd, gr->nodes()) {
		if (!mNodes.count(nd))
			add(nd);
	}
}

bool Graph::remove(Node* node) {
	if (mScene != 0)
		mScene->removeItem(node);
	foreach(Edge* edge, node->OutEdges()) {
		remove(edge);
	}
	foreach(Edge* edge, node->InEdges()) {
		remove(edge);
	}
	node->del();
	node->setGraph(0);
	return mNodes.removeOne(node);
}

//! Remove edge
bool Graph::remove(Edge* ed) {
	if (mScene != 0)
		mScene->removeItem(ed);
	ed->del();
	ed->setGraph(0);
	return mEdges.removeOne(ed);
}

bool Graph::remove(Graph* gr) {
	foreach(Node* node, gr->nodes()) {
		if (mNodes.contains(node)) {
			remove(node);
		}
	}

	foreach(Edge* edge, gr->edges()) {
		if (mEdges.contains(edge)) {
			remove(edge);
		}
	}

	return true;
}

void Graph::removeAll() {
	remove(this);
}

void Graph::fixEdge() {
	foreach(Edge* edge, mEdges) {
		if (!mNodes.contains(edge->destNode()) || !mNodes.contains(edge->sourceNode())) {
			mEdges.removeOne(edge);
		}
	}
}

Graph* Graph::toGraph(QList<QGraphicsItem*>& itemList) {
	Graph* retGraph = new Graph();
	foreach(QGraphicsItem *item, itemList) {
		if (item->type() == Node::Type)
			retGraph->add(qgraphicsitem_cast<Node*>(item));
		if (item->type() == Edge::Type)
			retGraph->add(qgraphicsitem_cast<Edge*>(item));
	}
	return retGraph;
}

