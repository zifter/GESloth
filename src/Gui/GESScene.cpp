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

#include <QContextMenuEvent>
#include <QMenu>
#include <QApplication>
#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QCompleter>
#include <QMainWindow>
#include <QVariant>
#include <QClipboard>
#include <QMimeData>
#include <QBuffer>
#include <QByteArray>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QTextCodec>
#include <QtXml/QDomDocument>
#include <QMessageBox>
#include <QComboBox>

#include "Gui/GESScene.h"
#include "Gui/GESPage.h"
#include "Gui/Command.h"
#include "Gui/SceneEventHandler/EdgeModeEventHandler.h"
#include "Gui/SceneEventHandler/NodeModeEventHandler.h"
#include "Gui/SceneEventHandler/InsertModeEventHandler.h"

#include "Graph/Node.h"
#include "Graph/Edge.h"

#include "XML/GESFileLoader.h"
#include "XML/GESFileWriter.h"

#include "Macros.h"

GESScene::GESScene(GESPage *prnt) :
		QGraphicsScene(prnt), line(NULL), menu(NULL) {
	QPainterPath path;
	mGraph = new Graph();
	mGraph->setScene(this);
	mMode = NodeMode;

	STATIC_ASSERT(LAST == 4, _COUNT_OF_EDIT_MODE_DOESNT_MATCH)

	hanlder.fill(0, (int) LAST);

	hanlder[NoneMode] = new AbstractSceneEventHandler(this);
	hanlder[NodeMode] = new NodeModeEventHandler(this);
	hanlder[EdgeMode] = new EdgeModeEventHanlder(this);
	hanlder[InsertMode] = new InsertModeEventHandler(this);
}

GESScene::~GESScene() {
	delete mGraph;
	delete line;
	delete menu;
}
void GESScene::setState(EditMode state) {
	qobject_cast<GESPage*>(parent())->getSettings()->setState(state);
	mMode = state;
	foreach (Node *node, mGraph->nodes() )
		node->setFlag(QGraphicsItem::ItemIsMovable, mMode != EdgeMode);
}

void GESScene::setGraph(Graph* gr) {
	mGraph = gr;
	mGraph->setScene(this);
}

void GESScene::add(Edge* edge) {
	addItem(edge);
}
void GESScene::add(Node* node) {
	addItem(node);
	node->setFlag(QGraphicsItem::ItemIsMovable, mMode != EdgeMode);
}

void GESScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	hanlder[mMode]->mousePressEvent(mouseEvent);
	QGraphicsScene::mousePressEvent(mouseEvent);
}

void GESScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	hanlder[mMode]->mouseMoveEvent(mouseEvent);
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GESScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	hanlder[mMode]->mouseReleaseEvent(mouseEvent);
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GESScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
	hanlder[mMode]->contextMenuEvent(event);
	QGraphicsScene::contextMenuEvent(event);
}

void GESScene::clear() {
	mGraph->removeAll();
}

void GESScene::setName() {

	QWidget txt;
	QDialog dialog(&txt);
	dialog.setWindowTitle(tr("Set name"));

	Object* node = qgraphicsitem_cast<Object*>(currentItem);

	QLabel label(tr("Name:"), &dialog);
	QLineEdit lineEdit(&dialog);

	QDialogButtonBox buttonBox(
			QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	buttonBox.setParent(&dialog);

	connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
	connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

	QVBoxLayout layout;
	layout.addWidget(&label);
	layout.addWidget(&lineEdit);
	layout.addWidget(&buttonBox);

	QCompleter completer;
	completer.setCaseSensitivity(Qt::CaseInsensitive);
	lineEdit.setCompleter(&completer);
	lineEdit.setText(node->getText());
	lineEdit.selectAll();

	dialog.setLayout(&layout);
	lineEdit.setFocus();

	if (dialog.exec()) {
		setTextCommand* command = new setTextCommand(node, lineEdit.text());
		stackCommand.push(command);
		node->update();
	}
}

void GESScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent) {
	hanlder[mMode]->mouseDoubleClickEvent(mouseEvent);
	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void GESScene::copyObj() {
	QList<QGraphicsItem*> Buf = selectedItems();

	QMimeData *mData = new QMimeData;
	GESFileWriter writer;
	Graph* graph = toGraph(Buf);
	graph->fixEdge();
	QByteArray* bt = &writer.writeGraphToByte(graph);
	mData->setData("Graph Editor Sloth/items", (*bt));
	QClipboard *pastebuf = QApplication::clipboard();
	pastebuf->setMimeData(mData);
	mMode = InsertMode;
}

void GESScene::pasteObj() {
	if (!(QApplication::clipboard()->mimeData()->hasFormat(
			"Graph Editor Sloth/items")))
		return;
	QByteArray bt;
	bt = QApplication::clipboard()->mimeData()->data(
			"Graph Editor Sloth/items");

	GESFileLoader loader;
	Graph* pasteGr = new Graph();
	if (!loader.loadFromByte(pasteGr, bt))
		loader.showError();

	addItemCommand* command = new addItemCommand(pasteGr, mGraph);
	stackCommand.push(command);
}

//! Копировать элементы
void GESScene::cutObj() {
	copyObj();
	deleteSelectedObj();
}

void GESScene::selectAll() {
	foreach(QGraphicsItem* item, items())
		item->setSelected(true);
}

void GESScene::undoCommand() {
	stackCommand.undo();
}

void GESScene::redoCommand() {
	stackCommand.redo();
}

void GESScene::deleteSelectedObj() {
	currentList = selectedItems();
	delItemCommand* command = new delItemCommand(toGraph(currentList), mGraph);
	stackCommand.push(command);
	//deleteObj();
}

void GESScene::deleteUnderMouseObj() {
	QList<QGraphicsItem*> a;
	a << currentItem;
	delItemCommand* command = new delItemCommand(toGraph(a), mGraph);
	stackCommand.push(command);
	//deleteObj();
}

Graph* GESScene::toGraph(QList<QGraphicsItem*>& itemList) {
	Graph* retGraph = new Graph();
	foreach(QGraphicsItem *item, itemList) {
		if (item->type() == Node::Type)
			retGraph->add(qgraphicsitem_cast<Node*>(item));
		if (item->type() == Edge::Type)
			retGraph->add(qgraphicsitem_cast<Edge*>(item));
	}
	return retGraph;
}

void GESScene::renderToImage(QPainter *painter, const QRectF &target,
		const QRectF &source) {
	QBrush brush = backgroundBrush();
	setBackgroundBrush(QBrush(Qt::NoBrush));
	render(painter, target, source, Qt::KeepAspectRatioByExpanding);
	setBackgroundBrush(brush);
}

void GESScene::pre_createEdge(const QPointF& p, bool newEdge) {
	if (newEdge) {
		if (line != 0) {
			removeItem(line);
			delete line;
			line = 0;
		}
		line = new QGraphicsLineItem(QLineF(p, p));
		line->setPen(QPen(Qt::black, 1));
		addItem(line);
	} else if (line != 0) {
		QLineF newLine(line->line().p1(), p);
		line->setLine(newLine);
	}
}

void GESScene::createEdge() {
	if (line != 0) {

		QList<QGraphicsItem *> startItems = items(line->line().p1());
		if (startItems.count() && startItems.first() == line)
			startItems.removeFirst();
		QList<QGraphicsItem *> endItems = items(line->line().p2());
		if (endItems.count() && endItems.first() == line)
			endItems.removeFirst();

		removeItem(line);
		delete line;
		line = NULL;

		if (startItems.count() > 0 && endItems.count() > 0
				&& startItems.first()->type() == Node::Type
				&& endItems.first()->type() == Node::Type
				&& startItems.first() != endItems.first()) {
			Node* startNode = qgraphicsitem_cast<Node *>(startItems.first());
			Node* endNode = qgraphicsitem_cast<Node *>(endItems.first());
			bool flag = true;
			foreach(Edge* edge, mGraph->edges())
				if (edge->checkEdge(startNode, endNode)) {
					flag = false;
					break;
				}
			if (flag) {
				Edge *edge = new Edge(startNode, endNode);
				QList<QGraphicsItem*> a;
				a << edge;
				addItemCommand* command = new addItemCommand(toGraph(a),
						mGraph);
				stackCommand.push(command);
			}
		}
	}
	line = 0;
}

void GESScene::createNode(const QPointF& point) {
	Node *node = new Node(0);
	node->setPos(point);
	QList<QGraphicsItem*> a;
	a << node;
	addItemCommand* command = new addItemCommand(toGraph(a), mGraph);
	stackCommand.push(command);
}

void GESScene::createContextMenu(const QPointF& scenePoint,
		const QPoint& screenPoint) {
	QList<QGraphicsItem*> itemsUnderMouse = items(scenePoint,
			Qt::IntersectsItemShape, Qt::DescendingOrder);
	if (menu) {
		delete menu;
		menu = NULL;
	}
	menu = new QMenu();
	if (itemsUnderMouse.isEmpty()) {
		menu->addAction(tr("Paste"), this, SLOT(pasteObj()),
				QKeySequence::Paste);
	} else {
		currentItem = itemsUnderMouse.at(0);
		menu->addAction(tr("Set name"), this, SLOT(setName()));
		menu->addAction(tr("Delete"), this, SLOT(deleteUnderMouseObj()),
				QKeySequence::Delete);
		if (currentItem->type() == Node::Type)
			menu->addAction(tr("Copy"), this, SLOT(copyObj()),
					QKeySequence::Copy);
	}
	menu->addAction(tr("Undo"), this, SLOT(undoCommand()), QKeySequence::Undo);
	menu->addAction(tr("Redo"), this, SLOT(redoCommand()), QKeySequence::Redo);
	menu->addAction(tr("Select All"), this, SLOT(selectAll()),
			QKeySequence::SelectAll);
	menu->exec(screenPoint);
}
