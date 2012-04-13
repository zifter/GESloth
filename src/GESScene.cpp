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

void GraphRedactorView::wheelEvent(QWheelEvent *event)
{
    if(event->delta()>0)
    {
        if(sceneScaleCombo->currentIndex() == 9)
            return;
        sceneScaleCombo->setCurrentIndex(sceneScaleCombo->currentIndex()+1);
    }
    else
    {
        if(sceneScaleCombo->currentIndex() == 0)
            return;
        sceneScaleCombo->setCurrentIndex(sceneScaleCombo->currentIndex()-1);
    }
}
GraphRedactorScene::GraphRedactorScene( GraphWidget *prnt)
    : Parent(prnt), line(NULL), point(NULL), menu(NULL)
{
    QPainterPath path;
    stackCommand = new QUndoStack;
    myState = InsertNode;
}

void GraphRedactorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
    if(mouseEvent->button() == Qt::LeftButton)
        QApplication::setOverrideCursor(Qt::PointingHandCursor);
    if(myState == InsertEdge && mouseEvent->button()==Qt::LeftButton && !items(mouseEvent->scenePos()).isEmpty())
    {
        if(items(mouseEvent->scenePos()).at(0)->type() == Node::Type )
        {
            if( line != NULL )
            {
                removeItem(line);
                delete line;
                line = NULL;
            }
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(Qt::black, 1));
            addItem(line);
        }
    }

    if(myState == InsertNode )
    {
        if( point != NULL )
        {
            delete point;
            point = NULL;
        }
        point = new QPointF( mouseEvent->scenePos() );
    }
}

void GraphRedactorScene::addItems(QList<QGraphicsItem*> list)
{
    foreach( QGraphicsItem* item, list)
    {
        if(item->type() == Edge::Type)
        {
            Edge* edge = qgraphicsitem_cast< Edge* >(item);
            edges << edge;
            addItem(edge);
        }
        if(item->type() == Node::Type)
        {
            Node* node = qgraphicsitem_cast< Node* >(item);
            addItem(node);
            addNode(node);
        }
    }
    if( 1 == myState)
        foreach (Node *node, getNodes())
            node->setFlag(QGraphicsItem::ItemIsMovable, false);
}

void GraphRedactorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myState == InsertEdge && line != NULL)
    {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphRedactorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QApplication::restoreOverrideCursor();
    if(mouseEvent->button()==Qt::LeftButton)
    {
        if (line != 0 && myState == InsertEdge)
        {
            QList<QGraphicsItem *> startItems = items(line->line().p1());
            if (startItems.count() && startItems.first() == line)
                startItems.removeFirst();
            QList<QGraphicsItem *> endItems = items(line->line().p2());
            if (endItems.count() && endItems.first() == line)
                endItems.removeFirst();

            removeItem(line);
            delete line;
            line = NULL;

            if (startItems.count() > 0 && endItems.count() > 0 &&
                startItems.first()->type() == Node::Type &&
                endItems.first()->type() == Node::Type &&
                startItems.first() != endItems.first())
            {
                Node* startNode =
                    qgraphicsitem_cast<Node *>(startItems.first());
                Node* endNode =
                    qgraphicsitem_cast<Node *>(endItems.first());
                bool flag = true;
                foreach(Edge* edge, edges)
                    if(edge->checkEdge(startNode, endNode))
                    {
                        flag = false;
                        break;
                    }
                if( flag )
                {
                    Edge *edge = new Edge(startNode, endNode);
                    QList<QGraphicsItem*> a;
                    a << edge;
                    addItemCommand* command = new addItemCommand( a, this );
                    stackCommand->push(command);
                }
            }
        }
    }
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphRedactorScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QList< QGraphicsItem* > itemsUnderMouse = items(event->scenePos(), Qt::IntersectsItemShape, Qt::DescendingOrder);
    if(menu)
    {
        delete menu;
        menu = NULL;
    }
    menu = new QMenu();
    if(itemsUnderMouse.isEmpty())
    {
        menu->addAction(tr("Paste"), this, SLOT(pasteObj()), QKeySequence::Paste);
    }
    else
    {
        currentItem = itemsUnderMouse.at( 0 );
        menu->addAction(tr("Set name"), this, SLOT(setName()));
        menu->addAction(tr("Delete"), this, SLOT(deleteUnderMouseObj()), QKeySequence::Delete);
        if(currentItem->type() == Node::Type)
            menu->addAction(tr("Copy"), this, SLOT(copyObj()), QKeySequence::Copy);
    }
    menu->addAction(tr("Undo"), this, SLOT(undoCommand()), QKeySequence::Undo);
    menu->addAction(tr("Redo"), this, SLOT(redoCommand()), QKeySequence::Redo);
    menu->addAction(tr("Select All"), this, SLOT(selectAll()), QKeySequence::SelectAll);
    menu->exec(event->screenPos());
}

void GraphRedactorScene::deleteObj(QList<QGraphicsItem *> items)
{
    currentList = items;
    deleteObj();
}
void GraphRedactorScene::deleteObj()
{

    foreach(QGraphicsItem *item, currentList)
    {
        if(item->type() == Node::Type)
        {
            Node* node = qgraphicsitem_cast<Node*>(item);
            foreach(Edge* edge, node->OutEdges())
            {
                edges.removeOne(edge);
            }
            foreach(Edge* edge, node->InEdges())
            {
                edges.removeOne(edge);
            }
            node->del();
            nodes.removeOne(node);
        }
        if(item->type() == Edge::Type)
        {
            Edge* edge = qgraphicsitem_cast<Edge*>(item);
            edge->del();
            edges.removeOne(edge);
        }
        removeItem(item);
    }
}
void GraphRedactorScene::setName()
{

    QWidget* txt = new QWidget();
    QDialog dialog( txt );
    dialog.setWindowTitle(tr("Set name"));

    Object* node = qgraphicsitem_cast<Object*>(currentItem);

    QLabel* label = new QLabel(tr("Name:"),&dialog);
    QLineEdit* lineEdit = new QLineEdit(&dialog);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);
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

    if (dialog.exec())
    {
        setTextCommand* command = new setTextCommand(node, lineEdit->text());
        stackCommand->push(command);
        node->update();
    }
}

void GraphRedactorScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
    if(mouseEvent->button()==Qt::LeftButton && myState == InsertNode)
    {
        Node *node = new Node(Parent);
        node->setPos( mouseEvent->scenePos() );
        QList<QGraphicsItem*> a;
        a << node;
        addItemCommand* command = new addItemCommand( a, this );
        stackCommand->push(command);
        QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
    }
}

void GraphRedactorScene::copyObj()
{

    QList< Node* > nodeScn;
    QList< Node* > nodeBuf;
    QList< QGraphicsItem* > Buf;
    unsigned i = 0;
    foreach(QGraphicsItem* item, selectedItems())
    {
        if(item->type() == Node::Type)
        {
            nodeScn.insert( i, qgraphicsitem_cast<Node*>(item));
            Node* nd = new Node( Parent );
            Node* nodeForText = qgraphicsitem_cast<Node*>(item);
            nd->setText(nodeForText->getText());
            nodeBuf.insert( i, nd );
            nd->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
            //node << nd;
            Buf << nd;
            i++;
        }
    }
    foreach(QGraphicsItem* item, selectedItems())
    {
        if(item->type() == Edge::Type)
        {
            Edge* edge = qgraphicsitem_cast<Edge*>(item);
            if(nodeScn.contains(edge->sourceNode())
            && nodeScn.contains(edge->destNode()))
            {
                Edge* edgeCreate = new Edge(nodeBuf[nodeScn.indexOf(edge->sourceNode())],
                nodeBuf[nodeScn.indexOf(edge->destNode())]);
                edgeCreate->setText(edge->getText());
                Buf << edgeCreate;
            }
        }
    }
    QMimeData *mData = new QMimeData;
    QByteArray* bt = new QByteArray;
    saveToByte(Buf, (*bt) );
    mData->setData("graphics_Redactor/items", (*bt) );
    QClipboard *pastebuf = QApplication::clipboard();
    pastebuf->setMimeData( mData );
}

void GraphRedactorScene::pasteObj()
{
    if(!(QApplication::clipboard()->mimeData()->hasFormat("graphics_Redactor/items")))
        return;
    QList<QGraphicsItem*> list;
    QByteArray bt;
    bt = QApplication::clipboard()->mimeData()->data("graphics_Redactor/items");
    openFromByte(list, bt);
    addItemCommand* command = new addItemCommand( list, this );
    stackCommand->push(command);
}

void GraphRedactorScene::selectAll()
{
    foreach(QGraphicsItem* item, items())
        item->setSelected(true);
}

void GraphRedactorScene::saveToByte( QList< QGraphicsItem* >& itemsList, QByteArray& bt)
{
    QXmlStreamWriter stream(&bt);
    QTextCodec *codec = QTextCodec::codecForName("windows-1251");
    stream.setCodec(codec);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("GRAPH");

    foreach(QGraphicsItem* item, itemsList)
    {
        if(item->type() == Node::Type)
        {
            Node* node = qgraphicsitem_cast<Node*>(item);
            stream.writeStartElement("node");
            stream.writeAttribute("text", node->getText() );
            stream.writeAttribute("id", QString::number(itemsList.indexOf(item)));
            stream.writeEndElement ();
        }
        if(item->type() == Edge::Type)
        {
            Edge* edge = qgraphicsitem_cast<Edge*>(item);
            stream.writeStartElement("edge");
            stream.writeAttribute("text", edge->getText() );
            stream.writeAttribute("idsource", QString::number(itemsList.indexOf(edge->sourceNode())) );
            stream.writeAttribute("iddest", QString::number(itemsList.indexOf(edge->destNode())) );
            stream.writeEndElement();
        }
    }

    stream.writeEndElement();
    stream.writeEndDocument();
    return;
}


bool GraphRedactorScene::openFromByte( QList< QGraphicsItem* >& itemList, QByteArray& bt )
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument document;
    if (!document.setContent(bt, &errorStr, &errorLine, &errorColumn))
    {
        QMessageBox::information(0, qAppName(),
                                 tr("Error while opening this file.\nParse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        return false;
    }

    QDomElement root = document.documentElement();
    if (root.tagName() != "GRAPH")
    {
        QMessageBox::critical(0,tr("Error!"),tr("Error parsing XML-file!"),QMessageBox::Close);
        return false;
    }
    QMap< int, Node* > nodes;
    QList< Edge* > edges;
    QDomElement element = root.firstChildElement("node");
    while (!element.isNull())
    {
        if (!parseNode(element, nodes))
        {
            QMessageBox::critical(0,tr("Error!"),tr("Error parsing node!"),QMessageBox::Close);
            return false;
        }
        element = element.nextSiblingElement("node");
    }
    element = root.firstChildElement("edge");
    while (!element.isNull())
    {
        if (!parseEdge(element, nodes, edges))
        {
            QMessageBox::critical(0,tr("Error!"),tr("Error parsing edge!"),QMessageBox::Close);
            return false;
        }
        element = element.nextSiblingElement("edge");
    }
    foreach(Node* item, nodes)
        itemList << item;
    foreach(Edge* item, edges)
        itemList << item;
    return true;
}

bool GraphRedactorScene::parseNode(QDomElement& node, QMap<int, Node*>& list)
{
    if (node.hasAttribute("text"))
    {
        QString text = node.attribute("text");
        if (node.hasAttribute("id"))
        {
            QString id = node.attribute("id");
            Node* nd = new Node(Parent);
            nd->setText(text);
            nd->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
            list.insert(id.toInt(), nd);
            return true;
        }
    }
    return false;
}

bool GraphRedactorScene::parseEdge(QDomElement& edge, QMap<int, Node*>&  list, QList< Edge* >& edges )
{
    if (edge.hasAttribute("text"))
    {
        QString text = edge.attribute("text");
        if (edge.hasAttribute("idsource"))
        {
            QString idsource = edge.attribute("idsource");
            if(edge.hasAttribute("iddest"))
            {
                QString iddest = edge.attribute("iddest");
                int a = idsource.toInt();
                int b = iddest.toInt();

                if( list.contains( a ) && list.contains( b ) )
                {
                    Node* nd1 = list.value(a);
                    Node* nd2 = list.value(b);
                    Edge* ed = new Edge( nd1, nd2 );
                    ed->setText(text);
                    edges << ed;
                    return true;
                }
                else
                    return false;
            }
        }
    }
    return false;
}

void GraphRedactorScene::undoCommand()
{
    stackCommand->undo();
}

void GraphRedactorScene::redoCommand()
{
    stackCommand->redo();
}

void GraphRedactorScene::deleteSelectedObj()
{
    currentList = selectedItems();
    delItemCommand* command = new delItemCommand(currentList, this);
    stackCommand->push(command);
    deleteObj();
}

void GraphRedactorScene::deleteUnderMouseObj()
{
    QList< QGraphicsItem* > a;
    a << currentItem;
    delItemCommand* command = new delItemCommand( a, this );
    stackCommand->push(command);
    deleteObj();
}
