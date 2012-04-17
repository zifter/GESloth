/*
 * GESView.h
 *
 *  Created on: Apr 15, 2012
 *      Author: zifter
 */
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

#ifndef GESVIEW_H_
#define GESVIEW_H_

#include <QGraphicsView>
#include <QComboBox>
#include <QWheelEvent>
#include "Gui/GESScene.h"

class GESPage;

//! Просмоторщик сцены
class GESView : public QGraphicsView
{
    Q_OBJECT

public:
    //! Пустой конструктоh
    GESView( GESPage* prnt );
    virtual ~GESView();

   void setScene( GESScene* scene );

public slots:
	//! Slot for scale changing. (The string must be smth like this: "123%")
	void setScale(const QString& sc);

signals:
	//! Emitted, when sceneRect, visualized by this view, was changed.
	void sceneRectChanged(const QRectF& newRect);
    //! Emitted, when scale factor is changed.
    void scaleChanged(qreal newScaleFactor);

protected:
    //! Колесико
  //  void wheelEvent(QWheelEvent *event);

    bool isSceneRectControlled;

private slots:
	void updateSceneRect(const QRectF& rect);

private:
	GESPage* mParentPage;
};
#endif /* GESVIEW_H_ */
