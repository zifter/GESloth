#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "Edge.h"
#include "Node.h"
#include "GESloth.h"

Node::Node(GraphWidget *graphWidget)
    : graph(graphWidget)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setToolTip(QObject::tr("Node"));
    setZValue(-4);
}

void Node::addInEdge(Edge *edge)
{
    edge->adjust();
    if(InEdgeList.contains(edge))
        return;
    InEdgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::InEdges() const
{
    return InEdgeList;
}

void Node::addOutEdge(Edge *edge)
{
    if(OutEdgeList.contains(edge))
        return;
    OutEdgeList << edge;
}

QList<Edge *> Node::OutEdges() const
{
    return OutEdgeList;
}

void Node::delOutEdge(Edge *edge)
{
    OutEdgeList.removeOne(edge);
}
void Node::delInEdge(Edge *edge)
{
    InEdgeList.removeOne(edge);
}

void Node::del()
{
    foreach(Edge* edge, InEdges())
    {
        edge->del();
        InEdgeList.removeOne(edge);
        graph->delItem(edge);
    }
    foreach(Edge* edge, OutEdges())
    {
        edge->del();
        OutEdgeList.removeOne(edge);
        graph->delItem(edge);
    }
}

void Node::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this)
    {
        newPos = pos();
        return;
    }
    
    // ����������� ���� ���, ��������� ���������� ���� ����
    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items())
    {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node)
            continue;

        QLineF line(mapFromItem(node, 0, 0), QPointF(0, 0));
        qreal dx = line.dx();
        qreal dy = line.dy();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0)
        {
            xvel += (dx * 500 ) / l;
            yvel += (dy * 500 ) / l;
        }
    }

    // ��������� ������������ ��� �������
    double weight = (InEdgeList.size() + 1) * 10;
    foreach (Edge *edge, InEdgeList)
    {
        QPointF pos;
        if (edge->sourceNode() == this)
            pos = mapFromItem(edge->destNode(), 0, 0);
        else
            pos = mapFromItem(edge->sourceNode(), 0, 0);
        xvel += pos.x() / weight;
        yvel += pos.y() / weight;
    }

    foreach (Edge *edge, OutEdgeList)
    {
        QPointF pos;
        if (edge->sourceNode() == this)
            pos = mapFromItem(edge->destNode(), 0, 0);
        else
            pos = mapFromItem(edge->sourceNode(), 0, 0);
        xvel += pos.x() / weight;
        yvel += pos.y() / weight;
    }
    
    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 20), sceneRect.right() - 20));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 20), sceneRect.bottom() - 20));
}


bool Node::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-20- adjust, -33 - adjust,
                  100 + adjust, 70 + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    QRadialGradient gradient(-10, 0, 20);
    if (option->state & QStyle::State_Sunken)
    {
        gradient.setColorAt(1, QColor(Qt::white).light(120));
        gradient.setColorAt(0, QColor(Qt::gray).light(120));
    }
    else
    {
        gradient.setColorAt(0, Qt::gray);
        gradient.setColorAt(1, Qt::white);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
    if (isSelected())
    {
        painter->setPen(QPen(Qt::blue, 1, Qt::DotLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(-12, -12, 24, 24);
        painter->setPen(QPen(Qt::blue, 1, Qt::SolidLine));
        painter->drawEllipse(-1,-1,1,1);
    }
    qreal adj = 0;
    if(getText().size()<5)
        adj = 10;
    else
        adj = 0;
    painter->setPen(Qt::red);
    painter->setFont(QFont("Arial", 7));
    painter->drawText(QPointF(-20 + adj,20),getText());
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
    case ItemPositionHasChanged:
        foreach (Edge *edge, InEdgeList)
            edge->adjust();
        foreach (Edge *edge, OutEdgeList)
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    }

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
