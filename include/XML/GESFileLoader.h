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

#include <QByteArray>
#include <qdom.h>

#include "XML/GESTag.h"

#include "Gui/PageSettings.h"
#include "Gui/GESPage.h"

#include "Graph/Graph.h"
#include "Graph/Node.h"
#include "Graph/Edge.h"

class GESFileLoader {
public:
	GESFileLoader();
	~GESFileLoader();

	bool load(GESPage*& page);
	bool loadFromByte(Graph*& graph, const QByteArray& bt);

	void showError();

protected:
	bool parseGraph( QDomElement& root, Graph*& graph );

	bool parseNode(QDomElement& node, QMap<int, Node*>& list);
	bool parseEdge(QDomElement& edge, QMap<int, Node*>& list
			, QList<Edge*>& edges);

	bool parsePointF( QDomElement& node, QPointF& point );
	bool parseSettings( QDomElement& node, PageSettings* set );

	void missingAttr( const QString& tag, const QString& attr, int line  );

private	:
	QString mErrorMessage;
	QDomDocument mDocument;

};

#endif /* GESFILELOADER_H_ */
