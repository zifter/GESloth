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

/*
 * GESView.cpp
 *
 *  Created on: Apr 15, 2012
 *      Author: zifter
 */

#include <QScrollBar>

#include "Gui/GESView.h"
#include "Gui/GESPage.h"
#include "Gui/GESloth.h"
#include "Gui/GESTabWidget.h"

#include "Macros.h"

GESView::GESView(GESPage* prnt) :
		QGraphicsView(prnt), isSceneRectControlled(false) {
	setCacheMode(QGraphicsView::CacheBackground);
	setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::AnchorViewCenter);
	scale(qreal(1.), qreal(1.));
}

GESView::~GESView() {
}

void GESView::setScene(GESScene* scene) {
	QGraphicsView::setScene(scene);
	connect(scene, SIGNAL(sceneRectChanged(const QRectF&)), this,
			SLOT(updateSceneRect(const QRectF&)));
}

void GESView::mouseMoveEvent (QMouseEvent * event)
{
    if(event->buttons() & Qt::MidButton)
    {
        viewport()->setCursor(Qt::ClosedHandCursor);
        QScrollBar *hBar = horizontalScrollBar();
        QScrollBar *vBar = verticalScrollBar();
        QPoint delta = event->pos() - mPrevMousePos;
        hBar->setValue(hBar->value() + (isRightToLeft() ? delta.x() : -delta.x()));
        vBar->setValue(vBar->value() - delta.y());
        mPrevMousePos = event->pos();
    }

    QGraphicsView::mouseMoveEvent(event);
}


void GESView::mousePressEvent (QMouseEvent * event)
{
    if(event->button() == Qt::MidButton)
    {
        viewport()->setCursor(Qt::OpenHandCursor);
        mPrevMousePos = event->pos();
    }
    else
        QGraphicsView::mousePressEvent(event);
}

void GESView::mouseReleaseEvent (QMouseEvent * event)
{
    if(event->button() == Qt::MidButton)
    {
        viewport()->setCursor(Qt::ArrowCursor);
        mPrevMousePos = event->pos();
    }
    else
        QGraphicsView::mouseReleaseEvent(event);
}

void GESView::wheelEvent(QWheelEvent *event) {
	if (event->modifiers() == Qt::ControlModifier) {
		GESPage* page = qobject_cast<GESPage*>(parent());
		int zoom = page->getSettings()->getZoom();
		if( event->delta() > 0 ){
			zoom += GESloth::mScaleFactorChange;
			if( zoom > GESloth::maxScaleFactor )
				zoom = GESloth::maxScaleFactor;
		}
		else{
			zoom -= GESloth::mScaleFactorChange;
			if( zoom < GESloth::minScaleFactor )
				zoom = GESloth::minScaleFactor;
		}
		page->getSettings()->setZoom( zoom );
		emit QWidget::wheelEvent(event);
	}
	else
		emit QGraphicsView::wheelEvent( event );
}

void GESView::updateSceneRect(const QRectF& rect) {
	if (!isSceneRectControlled && !sceneRect().contains(rect))
		isSceneRectControlled = true;

	if (isSceneRectControlled) {
		QPointF topLeft = mapToScene(0, 0);
		QPointF bottomRight = mapToScene(viewport()->width(),
				viewport()->height());
		QRectF vis(topLeft, bottomRight);
		QRectF result = rect.adjusted(-100, -100, 100, 100).unite(vis).unite(
				sceneRect());
		setSceneRect(result);

		emit sceneRectChanged(result);
	}
}

void GESView::setScale(const QString& sc) {
	QTransform t = transform();
	//Default transform
	t.reset();

	//Getting percent value
	QString str = sc;
	str.remove("%");
	double d = str.toDouble();
	qobject_cast<GESPage*>(parent())->getSettings()->setZoom(d);
	d /= 100;

	//Setting transformation
	setTransform(t.scale(d, d), false);
}
