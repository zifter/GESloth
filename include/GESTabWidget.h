/*
 * GESTabWidget.h
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#ifndef GESTABWIDGET_H_
#define GESTABWIDGET_H_

#include <QTabWidget>
#include <QPixmap>

class GESPage;

class GESTabWidget: public QTabWidget {
	Q_OBJECT
public:
	GESTabWidget();
	~GESTabWidget();

	void addPage();

	QPixmap* getViewportPixmap();



public slots:

	void setScale(const QString&);

	void setSceneState( int st );

	void selectAll();

	void copyObj();

	void pasteObj();

	void redoCommand();

	void undoCommand();

	void deleteSelectedObj();

	void cutObj();

signals:
	void scaleChanged(qreal);

private:
	GESPage* getCurrentPage();
};

#endif /* GESTABWIDGET_H_ */
