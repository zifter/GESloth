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
 * GESFileLoader.h
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#ifndef GESFILELOADER_H_
#define GESFILELOADER_H_

#include "GESTag.h"
#include "Graph/Graph.h"
#include "Graph/Node.h"
#include "Graph/Edge.h"
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
