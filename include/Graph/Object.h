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

#ifndef OBJECT_H
#define OBJECT_H

#include <QGraphicsItem>

#ifndef NULL
#define NULL   ((void *) 0)
#endif

class Graph;

class Object: public QGraphicsItem {
public:
	Object();
	~Object();

	//! Виртуальная функция удаления
	virtual void del() {
	}

	void setGraph(Graph* gr){
		mGraph = gr;
	}

	Graph* getGraph() const {
		return mGraph;
	}

	//! Установить объект
	void setText(QString _txt) {
		text = _txt;
		if (_txt.length() > 15)
			short_text = _txt.replace(15, _txt.length() - 15, QString("~"));
		else
			short_text = _txt;
	}

	//! Получить текст объекта
	QString getShortText() const {
		return short_text;
	}

	//! Получить текст объекта
	QString getText() const {
		return text;
	}

protected:
	Graph* mGraph;

private:
	//! Текст
	QString text;

	//!
	QString short_text;

};

#endif // OBJECT_H
