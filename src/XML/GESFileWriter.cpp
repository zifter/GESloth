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
 * GESFileWriter.cpp
 *
 *  Created on: Apr 17, 2012
 *      Author: zifter
 */


#include <QTextCodec>
#include <QFile>
#include <qdom.h>
#include "Graph/Graph.h"
#include "Graph/Edge.h"
#include "Graph/Node.h"

#include "XML/GESFileWriter.h"
#include "XML/GESTag.h"

#include "Macros.h"

GESFileWriter::GESFileWriter() : mStream(&mByte) {

}

GESFileWriter::~GESFileWriter() {
}

void GESFileWriter::write(GESPage* page) {
	QFile fileOut(page->getFileName());
	if (!fileOut.open(QFile::WriteOnly | QFile::Text))
		return;

	QTextCodec *codec = QTextCodec::codecForName("windows-1251");
	mStream.setCodec(codec);
	mStream.setAutoFormatting(true);
	mStream.writeStartDocument();

	writeGraph(page->getScene()->getGraph());

	writeSettings( page->getSettings() );

	mStream.writeEndDocument();

	fileOut.write(mByte);
	fileOut.close();
}

QByteArray& GESFileWriter::writeGraphToByte(Graph* graph) {
	QTextCodec *codec = QTextCodec::codecForName("windows-1251");
	mStream.setCodec(codec);
	mStream.setAutoFormatting(true);
	mStream.writeStartDocument();

	writeGraph(graph);

	mStream.writeEndDocument();

	return mByte;
}

void GESFileWriter::writeGraph(Graph* graph) {
	mStream.writeStartElement(tag::XML_GRAPH);

	foreach(Node* node, graph->nodes())
	{
		mStream.writeStartElement(tag::XML_NODE);
			mStream.writeAttribute(attr::XML_TEXT, node->getText());
			mStream.writeAttribute(attr::XML_ID,
					QString::number(graph->nodes().indexOf(node)));
				mStream.writeStartElement(tag::XML_CENTER);
					writePointF( node->pos());
				mStream.writeEndElement();
		mStream.writeEndElement();
	}
	foreach(Edge* edge, graph->edges())
	{
		mStream.writeStartElement(tag::XML_EDGE);
			mStream.writeAttribute(attr::XML_TEXT, edge->getText());
			mStream.writeAttribute(attr::XML_ID_SOURCE,
					QString::number(graph->nodes().indexOf(edge->sourceNode())));
			mStream.writeAttribute(attr::XML_ID_DEST,
					QString::number(graph->nodes().indexOf(edge->destNode())));
		mStream.writeEndElement();
	}

	mStream.writeEndElement();
}

void GESFileWriter::writeSettings(PageSettings* set) {
	mStream.writeStartElement(tag::XML_SETTINGS);

		// scene rect
		mStream.writeStartElement(tag::XML_SCENE_RECT);

			mStream.writeStartElement(tag::XML_SIZE);
				QSizeF size = set->getSceneRect().size();
				writePointF( QPointF( size.width(), size.height() ) );
			mStream.writeEndElement();

			mStream.writeStartElement(tag::XML_CENTER);
				writePointF( set->getSceneRect().center());
			mStream.writeEndElement();

		mStream.writeEndElement();

	mStream.writeEndElement();
}

void  GESFileWriter::writePointF(QPointF p) {
	mStream.writeStartElement(tag::XML_POINTF);

	mStream.writeAttribute(attr::XML_X, QString::number( p.x() ));
	mStream.writeAttribute(attr::XML_Y, QString::number( p.y() ));

	mStream.writeEndElement();
}
