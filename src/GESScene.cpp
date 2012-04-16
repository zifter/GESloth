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

#include "GESScene.h"
#include "Command.h"
#include "Node.h"
#include "Edge.h"
#include "Macros.h"
#include "GESFileLoader.h"

GESScene::GESScene(GESloth *prnt) :
		Parent(prnt), line(NULL), point(NULL), menu(NULL) {
	QPainterPath path;
	mGraph = new Graph();
	mGraph->setScene( this );
	stackCommand = new QUndoStack;
	myState = InsertNode;
}

void GESScene::setState(State state){
	myState = state;
	bool movable = ( myState != InsertEdge );
	foreach (Node *node, mGraph->nodes() )
		node->setFlag(QGraphicsItem::ItemIsMovable, movable);
}

void GESScene::setGraph( Graph* gr ){
	mGraph = gr;
	mGraph->setScene(this);
}

void GESScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	QGraphicsScene::mousePressEvent(mouseEvent);
	/*
	 if(mouseEvent->button() == Qt::LeftButton)
	 QApplication::setOverrideCursor(Qt::PointingHandCursor);
	 */
	if (myState == InsertEdge && mouseEvent->button() == Qt::LeftButton
			&& !items(mouseEvent->scenePos()).isEmpty()) {
		if (items(mouseEvent->scenePos()).at(0)->type() == Node::Type) {
			if (line != NULL) {
				removeItem(line);
				delete line;
				line = NULL;
			}
			line = new QGraphicsLineItem(
					QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
			line->setPen(QPen(Qt::black, 1));
			addItem(line);
		}
	}

	if (myState == InsertNode) {
		if (point != NULL) {
			delete point;
			point = NULL;
		}
		point = new QPointF(mouseEvent->scenePos());
	}
}

void GESScene::addItems(QList<QGraphicsItem*> list) {
	foreach( QGraphicsItem* item, list)
	{
		if (item->type() == Edge::Type) {
			Edge* edge = qgraphicsitem_cast<Edge*>(item);
			mGraph->add(edge);
			addItem(edge);
		}
		if (item->type() == Node::Type) {
			Node* node = qgraphicsitem_cast<Node*>(item);
			addItem(node);
			mGraph->add(node);
		}
	}
	if (myState == InsertEdge)
		foreach ( Node *node, mGraph->nodes() )
			node->setFlag(QGraphicsItem::ItemIsMovable, false);
}

void GESScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	if (myState == InsertEdge && line != NULL) {
		QLineF newLine(line->line().p1(), mouseEvent->scenePos());
		line->setLine(newLine);
	}
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GESScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	//QApplication::restoreOverrideCursor();
	if (mouseEvent->button() == Qt::LeftButton) {
		if (line != 0 && myState == InsertEdge) {

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
				Node* startNode = qgraphicsitem_cast<Node *>(
						startItems.first());
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
					addItemCommand* command = new addItemCommand(toGraph(a), mGraph);
					stackCommand->push(command);
				}
			}
		}
	}
	line = 0;
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GESScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
	QList<QGraphicsItem*> itemsUnderMouse = items(event->scenePos(),
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
	menu->exec(event->screenPos());
}

void GESScene::deleteObj(QList<QGraphicsItem *> items) {
	currentList = items;
	deleteObj();
}
void GESScene::deleteObj() {
	foreach(QGraphicsItem *item, currentList)
	{
		if (item->type() == Node::Type) {
			Node* node = qgraphicsitem_cast<Node*>(item);
			foreach(Edge* edge, node->OutEdges())
			{
				mGraph->remove(edge);
			}
			foreach(Edge* edge, node->InEdges())
			{
				mGraph->remove(edge);
			}
			node->del();
			mGraph->remove(node);
		}
		if (item->type() == Edge::Type) {
			Edge* edge = qgraphicsitem_cast<Edge*>(item);
			edge->del();
			mGraph->remove(edge);
		}
		removeItem(item);
	}
}
void GESScene::setName() {

	QWidget* txt = new QWidget();
	QDialog dialog(txt);
	dialog.setWindowTitle(tr("Set name"));

	Object* node = qgraphicsitem_cast<Object*>(currentItem);

	QLabel* label = new QLabel(tr("Name:"), &dialog);
	QLineEdit* lineEdit = new QLineEdit(&dialog);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(
			QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	buttonBox->setParent(&dialog);

	connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	layout->addWidget(lineEdit);
	layout->addWidget(buttonBox);

	QCompleter *completer = new QCompleter();
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	lineEdit->setCompleter(completer);
	lineEdit->setText(node->getText());
	lineEdit->selectAll();

	dialog.setLayout(layout);
	lineEdit->setFocus();

	if (dialog.exec()) {
		setTextCommand* command = new setTextCommand(node, lineEdit->text());
		stackCommand->push(command);
		node->update();
	}
}

void GESScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent) {
	if (mouseEvent->button() == Qt::LeftButton && myState == InsertNode) {
		Node *node = new Node(Parent);
		node->setPos(mouseEvent->scenePos());
		QList<QGraphicsItem*> a;
		a << node;
		addItemCommand* command = new addItemCommand(toGraph(a), mGraph);
		stackCommand->push(command);
		QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
	}
}

void GESScene::copyObj() {

	QList<Node*> nodeScn;
	QList<Node*> nodeBuf;
	QList<QGraphicsItem*> Buf;
	unsigned i = 0;
	foreach(QGraphicsItem* item, selectedItems())
	{
		if (item->type() == Node::Type) {
			nodeScn.insert(i, qgraphicsitem_cast<Node*>(item));
			Node* nd = new Node(Parent);
			Node* nodeForText = qgraphicsitem_cast<Node*>(item);
			nd->setText(nodeForText->getText());
			nodeBuf.insert(i, nd);
			nd->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
			Buf << nd;
			i++;
		}
	}
	foreach(QGraphicsItem* item, selectedItems())
	{
		if (item->type() == Edge::Type) {
			Edge* edge = qgraphicsitem_cast<Edge*>(item);
			if (nodeScn.contains(edge->sourceNode())
					&& nodeScn.contains(edge->destNode())) {
				Edge* edgeCreate = new Edge(
						nodeBuf[nodeScn.indexOf(edge->sourceNode())],
						nodeBuf[nodeScn.indexOf(edge->destNode())]);
				edgeCreate->setText(edge->getText());
				Buf << edgeCreate;
			}
		}
	}
	QMimeData *mData = new QMimeData;
	QByteArray* bt = new QByteArray;
	saveToByte(Buf, (*bt));
	mData->setData("Graph Editor Sloth/items", (*bt));
	QClipboard *pastebuf = QApplication::clipboard();
	pastebuf->setMimeData(mData);
}

void GESScene::pasteObj() {
	if (!(QApplication::clipboard()->mimeData()->hasFormat(
			"Graph Editor Sloth/items")))
		return;
	QList<QGraphicsItem*> list;
	QByteArray bt;
	bt = QApplication::clipboard()->mimeData()->data("graphics_Redactor/items");
	GESFileLoader loader;
	Graph* pasteGr = new Graph();
	if( !loader.load( pasteGr, bt ) )
		loader.showError();
	addItemCommand* command = new addItemCommand(pasteGr, mGraph);
	stackCommand->push(command);
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

void GESScene::saveToByte(QList<QGraphicsItem*>& itemsList, QByteArray& bt) {
	QXmlStreamWriter stream(&bt);
	QTextCodec *codec = QTextCodec::codecForName("windows-1251");
	stream.setCodec(codec);
	stream.setAutoFormatting(true);
	stream.writeStartDocument();
	stream.writeStartElement("GRAPH");

	foreach(QGraphicsItem* item, itemsList)
	{
		if (item->type() == Node::Type) {
			Node* node = qgraphicsitem_cast<Node*>(item);
			stream.writeStartElement("node");
			stream.writeAttribute("text", node->getText());
			stream.writeAttribute("id",
					QString::number(itemsList.indexOf(item)));
			stream.writeEndElement();
		}
		if (item->type() == Edge::Type) {
			Edge* edge = qgraphicsitem_cast<Edge*>(item);
			stream.writeStartElement("edge");
			stream.writeAttribute("text", edge->getText());
			stream.writeAttribute("idsource",
					QString::number(itemsList.indexOf(edge->sourceNode())));
			stream.writeAttribute("iddest",
					QString::number(itemsList.indexOf(edge->destNode())));
			stream.writeEndElement();
		}
	}

	stream.writeEndElement();
	stream.writeEndDocument();
	return;
}

void GESScene::undoCommand() {
	stackCommand->undo();
}

void GESScene::redoCommand() {
	stackCommand->redo();
}

void GESScene::deleteSelectedObj() {
	currentList = selectedItems();
	delItemCommand* command = new delItemCommand(toGraph(currentList), mGraph);
	stackCommand->push(command);
	//deleteObj();
}

void GESScene::deleteUnderMouseObj() {
	QList<QGraphicsItem*> a;
	a << currentItem;
	delItemCommand* command = new delItemCommand(toGraph(a), mGraph);
	stackCommand->push(command);
	//deleteObj();
}

Graph* GESScene::toGraph( QList<QGraphicsItem*>& itemList ){
	Graph* retGraph = new Graph();
	foreach(QGraphicsItem *item, itemList)
	{
		if (item->type() == Node::Type) {
			retGraph->add(qgraphicsitem_cast<Node*>(item));
		}
		if (item->type() == Edge::Type) {
			retGraph->add(qgraphicsitem_cast<Edge*>(item));
		}
	}
	return retGraph;
}
