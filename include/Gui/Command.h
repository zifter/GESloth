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
	explicit addItemCommand(Graph* changeGraph, Graph* defGraph) :
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
	explicit delItemCommand(Graph* changeGraph, Graph* defGraph) :
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
	explicit setTextCommand(Object* obj, QString text) :
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
