#include "Command.h"
#include <QGraphicsItem>
#include "Edge.h"
#include "Node.h"

void addItemCommand::redo()
{
    foreach(QGraphicsItem* item, list)
        if(item->type() == Edge::Type)
        {
            Edge* edge = qgraphicsitem_cast< Edge* >(item);
            edge->destNode()->addInEdge(edge);
            edge->sourceNode()->addOutEdge(edge);
        }
    scene->addItems( list );
    foreach (Node *node, scene->getNodes())
        node->calculateForces();

}

void delItemCommand::undo()
{
    foreach(QGraphicsItem* item, list)
        if(item->type() == Edge::Type)
        {
            Edge* edge = qgraphicsitem_cast< Edge* >(item);
            edge->destNode()->addInEdge(edge);
            edge->sourceNode()->addOutEdge(edge);
        }
    scene->addItems( list );
    foreach (Node *node, scene->getNodes())
        node->calculateForces();
}
