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
#include <QVector>

#include "Gui/SceneEventHandler/AbstractSceneEventHandler.h"

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
class NodeModeEventHandler;
class InstertModeEventHandler;
class EdgeeModeEventHandler;
class PageSettings;

class GESScene: public QGraphicsScene {
Q_OBJECT

public:
	explicit GESScene(GESPage* prnt);
	virtual ~GESScene();

	enum EditMode{
		NoneMode = 0 ,
		NodeMode,
		EdgeMode,
		InsertMode,

		LAST
	};

	void setGraph(Graph* gr);

	Graph* getGraph() const {
		return mGraph;
	}

	void add( Edge* );
	void add( Node* );

	void renderToImage(QPainter *painter, const QRectF &target, const QRectF &source);

public:
	/**
	 * @brief Create node in point
	 * @param point Center point
	 */

	void createNode( const QPointF& point );
	/**
	 * @brief Draw line (look like point)
	 * @param point point where now situated mouses pointer
	 * @param newEdge if true - draw new line, else change previous line
	 */
	void pre_createEdge( const QPointF& point, bool newEdge );

	/**
	 * @brief Create edge from previous info about point
	 */
	void createEdge();

	/**
	 * @brief Create context menu which depend from items which situated under mouse
	 * @param scenePoint Point on scene
	 * @param screenPoint Point on screen where we create context menu
	 */
	void createContextMenu( const QPointF& scenePoint, const QPoint& screenPoint );

public slots:
	//! Установка состояния
	void setState(EditMode state);

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

	Graph* toGraph(QList<QGraphicsItem*>& itemList);

	//! Graph which contains on scene
	Graph* mGraph;

	EditMode mMode;

	QVector< AbstractSceneEventHandler* > hanlder;

	//! Линия
	QGraphicsLineItem* line;

	//! Меню
	QMenu* menu;

	//! Текущий элемент
	QGraphicsItem* currentItem;

	//! Текущие объекты
	QList<QGraphicsItem*> currentList;

	//! Стек команд
	QUndoStack stackCommand;

};

#endif // GRAPHREDACTORSCENE_H
