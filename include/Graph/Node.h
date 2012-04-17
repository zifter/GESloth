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

#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>
#include "Object.h"

class Edge;
class GESloth;
class QGraphicsSceneMouseEvent;

class Node : public Object
{
public:
    Node(GESloth *graphWidget);

    //! Добавить ребро
    void addEdge( Edge *edge);

    //! Добавить входящие ребро
    void addInEdge( Edge *edge);

    //! Входящие ребра
    QList<Edge *> InEdges() const;

    //! Добавить исходящие ребро
    void addOutEdge(Edge *edge);

    //! Исходящие ребра
    QList<Edge *> OutEdges() const;

    //! Удалить исходящие дуги
    void delOutEdge(Edge *edge);

    //! Удалить входящие дуги
    void delInEdge(Edge *edge);

    //! Удалить
    void del();

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    //! вычисление сил
    void calculateForces();

    //! Нахождение на положенном месте
    bool advance();

    //! Область прорисовки
    QRectF boundingRect() const;

    //! Перегруз формы
    QPainterPath shape() const;

    //! Перегруз прорисовки
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    //! Перегруз для изменения позиции
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    //! Событие нажтия
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    //! Событие отпускания кнопки
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    
private:
    //! Список входящих ребер
    QList<Edge *> InEdgeList;

    //! Список выходящих ребер
    QList<Edge *> OutEdgeList;

    //! Новая позиция
    QPointF newPos;

    //! Родитель
    GESloth *graph;

};

#endif
