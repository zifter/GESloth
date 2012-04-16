/*
 * GESFileLoader.h
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#ifndef GESFILELOADER_H_
#define GESFILELOADER_H_

#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <QByteArray>

class GESFileLoader {
public:
	GESFileLoader();
	~GESFileLoader();

	bool load(Graph*& graph, const QString& name);
	bool load(Graph*& graph, const QByteArray& bt);

	void showError();

protected:
	bool parseNode(QDomElement& node, QMap<int, Node*>& list);
	bool parseEdge(QDomElement& edge, QMap<int, Node*>& list
			, QList<Edge*>& edges);

private	:
	QString mErrorMessage;

};

#endif /* GESFILELOADER_H_ */
