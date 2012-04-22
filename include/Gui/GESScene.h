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
#include <QUndoCommand>
#include <QVector>

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
class AbstractSceneEventHandler;

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

	EditMode getEditMode() const{
		return mMode;
	}

	EditMode getPreviousEditMode() const{
		return mPreviousMode;
	}

	void add( Edge* );
	void add( Node* );

	void addCommand( QUndoCommand* cmd ){
		mStackCommand.push( cmd );
	}
	void renderToImage(QPainter *painter, const QRectF &target, const QRectF &source);

public:

	/**
	 * @brief Create context menu which depend from items which situated under mouse
	 * @param scenePoint Point on scene
	 * @param screenPoint Point on screen where we create context menu
	 */
	void createContextMenu( const QPointF& scenePoint, const QPoint& screenPoint );

public slots:
	//! Установка состояния
	void setEditMode(EditMode state);

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

	//! Graph which contains on scene
	Graph* mGraph;

	EditMode mMode;
	EditMode mPreviousMode;

	QVector< AbstractSceneEventHandler* > mHandler;

	//! Меню
	QMenu* menu;

	//! Текущий элемент
	QGraphicsItem* currentItem;

	//! Стек команд
	QUndoStack mStackCommand;

};

#endif // GRAPHREDACTORSCENE_H
