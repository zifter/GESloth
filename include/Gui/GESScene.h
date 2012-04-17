#ifndef GESSCENE_H
#define GESSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
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

#include <QMimeData>
#include <QGraphicsView>
#include <QUndoStack>

#include "PageSettings.h"

class GESloth;
class Node;
class Edge;
class QMenu;
class QContextMenuEvent;
class QWidget;
class QLineEdit;
class QByteArray;
class QDomElement;
class QWheelEvent;
class QComboBox;
class Graph;
class GESPage;

class GESScene: public QGraphicsScene {
Q_OBJECT

public:
	GESScene(GESPage* prnt);
	virtual ~GESScene();

	void setGraph(Graph* gr);

	Graph* getGraph() const {
		return mGraph;
	}

	void add( Edge* );
	void add( Node* );

	void renderToImage(QPainter *painter, const QRectF &target, const QRectF &source);

public slots:
	//! Установка состояния
	void setState(PageSettings::DrawState state);

	//! Удалить
	void deleteSelectedObj();

	//! Удалить
	void deleteUnderMouseObj();

	//! Установить имя
	void setName();

	//! Копировать элементы
	void copyObj();

	//! Копировать элементы
	void cutObj();

	//! Выделить все
	void selectAll();

	//! Вставить
	void pasteObj();

	void clear();

	//! Команда назад
	void undoCommand();

	//! Команда вперед
	void redoCommand();

protected:
	//! Событие нажатия кнопки
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

	//! Событие двежение зажатой мышки
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

	//! Событие отпускание кнопки
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

	//! Событие двойного щелчка
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);

	//! Событие вызова контекстного меню
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:

	Graph* toGraph(QList<QGraphicsItem*>& itemList);

	//! Graph which contains on scene
	Graph* mGraph;

	//! Линия
	QGraphicsLineItem* line;

	//! Контрольная точка
	QPointF* point;

	//! Меню
	QMenu* menu;

	PageSettings::DrawState mState;

	//! Текущий элемент
	QGraphicsItem* currentItem;

	//! Текущие объекты
	QList<QGraphicsItem*> currentList;

	//! Стек команд
	QUndoStack stackCommand;

};

#endif // GRAPHREDACTORSCENE_H
