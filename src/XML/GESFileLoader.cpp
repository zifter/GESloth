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
 * GESFileLoader.cpp
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#include <QApplication>
#include <QMessageBox>
#include <QFile>

#include "XML/GESFileLoader.h"

#include "Macros.h"

GESFileLoader::GESFileLoader() {

}

GESFileLoader::~GESFileLoader() {
}

void GESFileLoader::showError() {
	QMessageBox::information(0, qAppName(), mErrorMessage);
}

bool GESFileLoader::load(GESPage*& page) {
	QFile fileIn(page->getFileName());

	if (!fileIn.open(QFile::ReadOnly | QFile::Text)) {
		mErrorMessage = QObject::tr("Error while opening file.");
		return false;
	}

	QByteArray bt = fileIn.readAll();

	QString errorStr;
	int errorLine;
	int errorColumn;

	if (!mDocument.setContent(bt, &errorStr, &errorLine, &errorColumn)) {
		mErrorMessage = QObject::tr(
				"There was an error setting the contents of the XML model.");
		return false;
	}

	QDomElement root = mDocument.documentElement();

	if (root.tagName() != tag::XML_PAGE) {
		mErrorMessage = QObject::tr("The unknown contents of the XML model");
		return false;
	}

	QDomElement element = root.firstChildElement(tag::XML_GRAPH);

	Graph* gr = new Graph;

	if (!parseGraph(element, gr))
		return false;
	else
		page->getScene()->setGraph(gr);
	return true;
}

bool GESFileLoader::loadFromByte(Graph*& graph, const QByteArray& bt) {
	QString errorStr;
	int errorLine;
	int errorColumn;

	if (!mDocument.setContent(bt, &errorStr, &errorLine, &errorColumn)) {
		mErrorMessage = QObject::tr(
				"There was an error setting the contents of the XML model.");
		return false;
	}

	QDomElement root = mDocument.documentElement();

	return parseGraph(root, graph);

}

bool GESFileLoader::parseGraph(QDomElement& root, Graph*& graph) {
	if (root.tagName() != tag::XML_GRAPH) {
		mErrorMessage = QObject::tr("The unknown contents of the XML model");
		return false;
	}
	QMap<int, Node*> nodes;
	QList<Edge*> edges;

	QDomNodeList domNodes = root.elementsByTagName(tag::XML_NODE);
	for (int i = 0; i < domNodes.count(); ++i) {
		QDomElement element = domNodes.at(i).toElement();
		if (!parseNode(element, nodes)) {
			return false;
		}
	}

	QDomNodeList domEdges = root.elementsByTagName(tag::XML_EDGE);
	for (int i = 0; i < domEdges.count(); ++i) {
		QDomElement element = domEdges.at(i).toElement();
		if (!parseEdge(element, nodes, edges)) {
			return false;
		}
	}

	foreach( Node* node, nodes.values() )
	{
		graph->add(node);
	}
	foreach( Edge* edge, edges )
	{
		graph->add(edge);
	}

	return true;
}

bool GESFileLoader::parsePointF(QDomElement& root, QPointF& point) {
	QDomElement node = root.firstChildElement(tag::XML_POINTF);
	QString sAttr = attr::XML_X;
	bool parseRezult = node.hasAttribute(sAttr);

	if (parseRezult) {
		QString valX = node.attribute(sAttr);
		sAttr = attr::XML_Y;
		parseRezult = node.hasAttribute(sAttr);

		if (parseRezult) {
			QString valY = node.attribute(sAttr);

			point.setX(valX.toDouble());
			point.setY(valY.toDouble());
		}
	}

	if (!parseRezult)
		missingAttr(tag::XML_CENTER, sAttr, node.lineNumber());

	return true;
}

bool GESFileLoader::parseSettings(QDomElement& node, PageSettings*) {
	return true;
}

bool GESFileLoader::parseNode(QDomElement& node, QMap<int, Node*>& list) {
	QString sAttr = attr::XML_TEXT;
	bool parseRezult = node.hasAttribute(sAttr);

	if (parseRezult) {

		QString text = node.attribute(sAttr);
		sAttr = attr::XML_ID;
		parseRezult = node.hasAttribute(sAttr);

		if (parseRezult) {
			QString id = node.attribute(attr::XML_ID);
			Node* nd = new Node(0);
			nd->setText(text);
			list.insert(id.toInt(), nd);

			QPointF point;

			node = node.firstChildElement(tag::XML_CENTER);

			if (!parsePointF(node, point))
				return false;
			nd->setPos(point);
		}
	}

	if (!parseRezult)
		missingAttr(tag::XML_NODE, sAttr, node.lineNumber());
	return parseRezult;
}

bool GESFileLoader::parseEdge(QDomElement& edge, QMap<int, Node*>& list
		, QList<Edge*>& edges) {
	QString sAttr = attr::XML_TEXT;
	bool parseRezult = edge.hasAttribute(attr::XML_TEXT);

	if (parseRezult) {

		QString text = edge.attribute(sAttr);
		sAttr = attr::XML_ID_SOURCE;
		parseRezult = edge.hasAttribute(sAttr);

		if (parseRezult) {

			QString idsource = edge.attribute(sAttr);
			sAttr = attr::XML_ID_DEST;
			parseRezult = edge.hasAttribute(sAttr);

			if (parseRezult) {

				QString iddest = edge.attribute(sAttr);

				int a = idsource.toInt();
				int b = iddest.toInt();

				if (list.contains(a) && list.contains(b)) {
					Node* nd1 = list.value(a);
					Node* nd2 = list.value(b);
					Edge* ed = new Edge(nd1, nd2);
					ed->setText(text);
					edges << ed;
					return true;
				} else {
					mErrorMessage = QObject::tr(
							"The unknown id of node in XML model.");
					return false;
				}

			}
		}
	}
	if (!parseRezult)
		missingAttr(tag::XML_EDGE, sAttr, edge.lineNumber());
	return false;
}

void GESFileLoader::missingAttr(const QString& tag, const QString& attr,
		int line) {
	mErrorMessage =
			QObject::tr(
					"Tag with name %1 doesn't have attribute %2  in line %3 in XML model.").arg(
					tag).arg(attr).arg(line);
}
