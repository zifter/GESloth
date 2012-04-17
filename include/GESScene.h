#ifndef GESSCENE_H
#define GESSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QGraphicsView>

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
class QUndoStack;
class QWheelEvent;
class QComboBox;
class Graph;
class GESPage;

class GESScene: public QGraphicsScene {
Q_OBJECT

public:
	GESScene(GESPage* prnt);
	virtual ~GESScene() {
	}

	void setGraph(Graph* gr);
	Graph* getGraph() const {
		return mGraph;
	}

public slots:
	//! Установка состояния
	void setState(PageSettings::DrawState state);
	/*
	//! Получить состояние
	State getState() {
		return myState;
	}*/

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

	//! Родитель
	GESPage* mParentPage;

	//! Линия
	QGraphicsLineItem *line;

	//! Контрольная точка
	QPointF *point;

	//! Меню
	QMenu *menu;

	PageSettings::DrawState mState;

	//! Текущий элемент
	QGraphicsItem* currentItem;

	//! Текущие объекты
	QList<QGraphicsItem*> currentList;

	//! Разбор вершины
	bool parseNode(QDomElement& node, QMap<int, Node*>& list);

	//! Разбор ребра
	bool parseEdge(QDomElement& edge, QMap<int, Node*>& list
			, QList<Edge*>& edges);

	//! Стек команд
	QUndoStack* stackCommand;

};

#endif // GRAPHREDACTORSCENE_H
