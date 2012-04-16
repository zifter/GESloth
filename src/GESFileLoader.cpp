/*
 * GESFileLoader.cpp
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#include "GESFileLoader.h"
#include "GESTag.h"
#include "Macros.h"

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <qdom.h>

GESFileLoader::GESFileLoader() {

}

GESFileLoader::~GESFileLoader() {
}

void GESFileLoader::showError() {
	QMessageBox::information(0, qAppName(), mErrorMessage);
}

bool GESFileLoader::load(Graph*& graph, const QString& name) {
	QFile fileIn(name);

	if (!fileIn.open(QFile::ReadOnly | QFile::Text)) {
		mErrorMessage = QObject::tr("Error while opening file.");
		return false;
	}
	QByteArray bt = fileIn.readAll();

	return load(graph, bt);
}
bool GESFileLoader::load(Graph*& graph, const QByteArray& bt) {
	QString errorStr;
	int errorLine;
	int errorColumn;

	QDomDocument document;
	if (!document.setContent(bt, &errorStr, &errorLine, &errorColumn)) {
		mErrorMessage = QObject::tr(
				"There was an error setting the contents of the XML model.");
		return false;
	}

	QDomElement root = document.documentElement();
	if (root.tagName() != tag::XML_GRAPH) {
		mErrorMessage = QObject::tr("The unknown contents of the XML model");
		return false;
	}

	QMap<int, Node*> nodes;
	QList<Edge*> edges;

	QDomElement element = root.firstChildElement(tag::XML_NODE);
	while (!element.isNull()) {
		if (!parseNode(element, nodes)) {
			return false;
		}
		element = element.nextSiblingElement(tag::XML_NODE);
	}
	element = root.firstChildElement("edge");
	while (!element.isNull()) {
		if (!parseEdge(element, nodes, edges)) {
			return false;
		}
		element = element.nextSiblingElement("edge");
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

bool GESFileLoader::parseNode(QDomElement& node, QMap<int, Node*>& list) {
	QString sAttr = attr::XML_TEXT;
	bool parseRezult = node.hasAttribute(sAttr);

	if (parseRezult) {

		QString text = node.attribute(sAttr);
		sAttr = attr::XML_ID;
		parseRezult = node.hasAttribute(attr::XML_ID);

		if (parseRezult) {
			QString id = node.attribute(attr::XML_ID);
			Node* nd = new Node(0);
			nd->setText(text);
			nd->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
			list.insert(id.toInt(), nd);
			return true;
		}
	}

	if (!parseRezult)
		mErrorMessage =
				QObject::tr(
						"Tag with name %1 doesn't have attribute %2  in line %3 in XML model.").arg(
						tag::XML_NODE).arg(sAttr).arg(node.lineNumber());
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
					mErrorMessage = QObject::tr( "The unknown id of node in XML model.");
					return false;
				}

			}
		}
	}
	if (!parseRezult)
		mErrorMessage = QObject::tr(
				"Tag with name %1 doesn't have attribute %2  in line %3 in XML model.").arg(
				tag::XML_EDGE).arg(sAttr).arg(edge.lineNumber());
	return false;
}
