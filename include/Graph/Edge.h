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

#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include "Graph/Object.h"

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
