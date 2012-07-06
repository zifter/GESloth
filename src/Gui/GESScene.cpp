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

#include "XML/GESFileWriter.h"

#include "Macros.h"

GESScene::GESScene(GESPage *prnt) :
		QGraphicsScene(prnt), menu(NULL) {
	QPainterPath path;
	mGraph = new Graph();
	mGraph->setScene(this);
	mMode = NodeMode;

	STATIC_ASSERT(LAST == 4, _COUNT_OF_EDIT_MODE_DOESNT_MATCH)

	mHandler.fill(0, (int) LAST);

	mHandler[NoneMode] = new AbstractSceneEventHandler(this);
	mHandler[NodeMode] = new NodeModeEventHandler(this);
	mHandler[EdgeMode] = new EdgeModeEventHanlder(this);
	mHandler[InsertMode] = new InsertModeEventHandler(this);
}

GESScene::~GESScene() {
	delete mGraph;
	delete menu;
}
void GESScene::setEditMode(EditMode state) {
	qobject_cast<GESPage*>(parent())->getSettings()->setState(state);

	mPreviousMode = mMode;
	mMode = state;

	mHandler[mPreviousMode]->deactivate();
	mHandler[mMode]->activate();
}

void GESScene::setGraph(Graph* gr) {
	mGraph = gr;
	foreach(Node* node, gr->nodes()) {
		addItem(node);
	}

	foreach(Edge* edge, gr->edges()) {
		addItem(edge);
	}
}

void GESScene::add(Edge* edge) {
	addItem(edge);
}
void GESScene::add(Node* node) {
	addItem(node);
	node->setFlag(QGraphicsItem::ItemIsMovable, mMode != EdgeMode);
}

void GESScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	mHandler[mMode]->mousePressEvent(mouseEvent);
	QGraphicsScene::mousePressEvent(mouseEvent);
}

void GESScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	mHandler[mMode]->mouseMoveEvent(mouseEvent);
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GESScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	mHandler[mMode]->mouseReleaseEvent(mouseEvent);
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GESScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
	mHandler[mMode]->contextMenuEvent(event);
	QGraphicsScene::contextMenuEvent(event);
}

void GESScene::clear() {
	mGraph->removeAll();
}

void GESScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent) {
	mHandler[mMode]->mouseDoubleClickEvent(mouseEvent);
	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void GESScene::copyObj() {
	QList<QGraphicsItem*> Buf = selectedItems();

	QMimeData *mData = new QMimeData;
	GESFileWriter writer;

	Graph* graph = Graph::toGraph(Buf);
	graph->fixEdge();
	mData->setData("Graph Editor Sloth/items", writer.writeGraphToByte(graph));
	QApplication::clipboard()->setMimeData(mData);
}

void GESScene::pasteObj() {
	setEditMode(InsertMode);
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
	mStackCommand.undo();
}

void GESScene::redoCommand() {
	mStackCommand.redo();
}

void GESScene::deleteSelectedObj() {
	QList<QGraphicsItem*> selected = selectedItems();
	delItemCommand* command = new delItemCommand(Graph::toGraph(selected), mGraph);
	mStackCommand.push(command);
	//deleteObj();
}

void GESScene::deleteUnderMouseObj() {
	QList<QGraphicsItem*> a;
	a << currentItem;
	delItemCommand* command = new delItemCommand(Graph::toGraph(a), mGraph);
	mStackCommand.push(command);
	//deleteObj();
}

void GESScene::renderToImage(QPainter *painter, const QRectF &target, const QRectF &source) {
	QBrush brush = backgroundBrush();
	setBackgroundBrush(QBrush(Qt::NoBrush));
	render(painter, target, source, Qt::KeepAspectRatioByExpanding);
	setBackgroundBrush(brush);
}

void GESScene::createContextMenu(const QPointF& scenePoint, const QPoint& screenPoint) {
	QList<QGraphicsItem*> itemsUnderMouse = items(scenePoint, Qt::IntersectsItemShape, Qt::DescendingOrder);
	if (menu) {
		delete menu;
		menu = NULL;
	}
	menu = new QMenu();
	if (itemsUnderMouse.isEmpty()) {
		menu->addAction(tr("Paste"), this, SLOT(pasteObj()), QKeySequence::Paste);
	} else {
		currentItem = itemsUnderMouse.at(0);
		menu->addAction(tr("Set name"), this, SLOT(setName()));
		menu->addAction(tr("Delete"), this, SLOT(deleteUnderMouseObj()), QKeySequence::Delete);
		if (currentItem->type() == Node::Type)
			menu->addAction(tr("Copy"), this, SLOT(copyObj()), QKeySequence::Copy);
	}
	menu->addAction(tr("Undo"), this, SLOT(undoCommand()), QKeySequence::Undo);
	menu->addAction(tr("Redo"), this, SLOT(redoCommand()), QKeySequence::Redo);
	menu->addAction(tr("Select All"), this, SLOT(selectAll()), QKeySequence::SelectAll);
	menu->exec(screenPoint);
}

void GESScene::setName() {

	QWidget txt;
	QDialog dialog(&txt);
	dialog.setWindowTitle(tr("Set name"));

	Object* node = qgraphicsitem_cast<Object*>(currentItem);

	QLabel label(tr("Name:"), &dialog);
	QLineEdit lineEdit(&dialog);

	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
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
		mStackCommand.push(command);
		node->update();
	}
}

