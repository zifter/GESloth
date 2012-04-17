#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>

#include "Gui/GESScene.h"

#include "Graph/Object.h"
#include "Graph/Node.h"
#include "Graph/Graph.h"

class QGraphicsItem;

//! Команда добавления объектов
class addItemCommand: public QUndoCommand {
public:

	//! Конструктор команды
	addItemCommand(Graph* changeGraph, Graph* defGraph) :
			change(changeGraph), mGraph(defGraph) {
	}

	//! Undo command
	virtual void undo();
	//! Redo command
	virtual void redo();

private:

	//! Graph which added
	Graph* change;

	//! Initial graph
	Graph* mGraph;

};

//! Команда удаления объектов
class delItemCommand: public QUndoCommand {
public:

	//! Конструктор команды
	delItemCommand(Graph* changeGraph, Graph* defGraph) :
			change(changeGraph), mGraph(defGraph) {
	}

	//! Undo command
	virtual void undo() ;
	//! Redo command
	virtual void redo() ;

private:

	//! Graph which added
	Graph* change;

	//! Initial graph
	Graph* mGraph;
};

//! Команда установки текста
class setTextCommand: public QUndoCommand {
public:

	//! Коструктор команды
	setTextCommand(Object* obj, QString text) :
			SetableText(text), PastText(obj->getText()) {
		object = obj;
	}

	//! Команда назад
	virtual void undo() {
		object->setText(PastText);
		object->update();
	}

	//! Команда вперед
	virtual void redo() {
		object->setText(SetableText);
		object->update();
	}

private:
	//! Объект которому устанавливаеться текст
	Object* object;

	//! Устанавливаемый текст
	QString SetableText;

	//! Предыдущий текст
	QString PastText;
};

#endif // COMMAND_H
