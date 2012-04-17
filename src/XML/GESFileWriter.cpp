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
