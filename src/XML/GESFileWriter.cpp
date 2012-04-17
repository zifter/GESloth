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

#include "Graph/Edge.h"
#include "Graph/Node.h"

#include "XML/GESFileWriter.h"
#include "XML/GESTag.h"

#include "Macros.h"

#include <QXmlStreamWriter>
#include <QTextCodec>
#include <QFile>

GESFileWriter::GESFileWriter() {
	// TODO Auto-generated constructor stub

}

GESFileWriter::~GESFileWriter() {
	// TODO Auto-generated destructor stub
}

void GESFileWriter::write(Graph* graph, QString name) {
	QFile fileOut(name);
	if (!fileOut.open(QFile::WriteOnly | QFile::Text))
		return;
	fileOut.write(writeToByte(graph));
	fileOut.close();
}

QByteArray& GESFileWriter::writeToByte(Graph* graph) {
	QByteArray* bt = new QByteArray();
	QXmlStreamWriter stream(bt);
	QTextCodec *codec = QTextCodec::codecForName("windows-1251");
	stream.setCodec(codec);
	stream.setAutoFormatting(true);
	stream.writeStartDocument();
	stream.writeStartElement(tag::XML_GRAPH);

	foreach(Node* node, graph->nodes())
	{
		stream.writeStartElement(tag::XML_NODE);
		stream.writeAttribute(attr::XML_TEXT, node->getText());
		stream.writeAttribute(attr::XML_ID,
				QString::number(graph->nodes().indexOf(node)));
		stream.writeEndElement();
	}
	foreach(Edge* edge, graph->edges())
	{
		stream.writeStartElement(tag::XML_EDGE);
		stream.writeAttribute(attr::XML_TEXT, edge->getText());
		stream.writeAttribute(attr::XML_ID_SOURCE,
				QString::number(graph->nodes().indexOf(edge->sourceNode())));
		stream.writeAttribute(attr::XML_ID_DEST,
				QString::number(graph->nodes().indexOf(edge->destNode())));
		stream.writeEndElement();
	}

	stream.writeEndElement();
	stream.writeEndDocument();
	return *bt;
}
