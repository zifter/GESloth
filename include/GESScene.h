#ifndef GESSCENE_H
#define GESSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QGraphicsView>

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

class GESScene: public QGraphicsScene {
Q_OBJECT

public:
	GESScene( GESloth* prnt = 0 );
	virtual ~GESScene() {
	}
	;

	//! Возможные состояния системы
	enum State {
		InsertNode, InsertEdge
	};

	//! Добавить объекты списка
	void addItems(QList<QGraphicsItem*> list);

	void setGraph( Graph* gr );

public slots:
	//! Установка состояния
	void setState(State state);

	//! Получить состояние
	State getState() {
		return myState;
	}

	//! Удалить
	void deleteSelectedObj();

	//! Удалить
	void deleteUnderMouseObj();

	//! Удалить объекты списка
	void deleteObj(QList<QGraphicsItem*> itemList);

	//! Удалить объекты текущего списка
	void deleteObj();

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

	//! Сохраняет лист объектов в массив байтов в XML-формате
	void saveToByte(QList<QGraphicsItem*>& itemList, QByteArray&);

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

	Graph* toGraph( QList<QGraphicsItem*>& itemList );

	//! Состояние
	State myState;

	//! Graph which contains on scene
	Graph* mGraph;

	//! Родитель
	GESloth *Parent;

	//! Линия
	QGraphicsLineItem *line;

	//! Контрольная точка
	QPointF *point;

	//! Меню
	QMenu *menu;

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
