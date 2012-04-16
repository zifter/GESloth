#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include "Object.h"

class Node;

class Edge : public  Object
{
public:
    Edge(Node *sourceNode, Node *destNode);
    ~Edge();
    //! Получить источник ребра
    Node* sourceNode() const;

    //! Установить источник ребра
    void setSourceNode(Node *node);

    //! Получить конец ребра
    Node* destNode() const;

    //! Установить конец ребра
    void setDestNode(Node *node);

    //! Регулировка ребра
    void adjust();

    //! Удалить ребро
    void del();

    //! Returns length of edge
    qreal length();

    //! Проверить ребро на то, что src и dst являеються иточником и концом ребра
    bool checkEdge(Node* src, Node* dst);

    //! Перегруз типа для графического объекта
    enum { Type = UserType + 2 };

    //! Вернуть тип
    int type() const { return Type; }
    
protected:
    //! Перегруз области прорисовки
    QRectF boundingRect() const;

    //! Перегрузка прорисовки
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //! Определение формы объекта
    QPainterPath shape() const;

private:
    //! Исходная и концевая вершина
    Node* source,* dest;

    //! Точка исходной вершины
    QPointF sourcePoint;

    //! Точка концевой вершины
    QPointF destPoint;

    //! Размер стрелки
    qreal arrowSize;
};

#endif
