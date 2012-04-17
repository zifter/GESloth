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

#include "Gui/GESView.h"
#include "Gui/GESPage.h"

#include "Macros.h"

GESView::GESView(GESPage* prnt ) :
		isSceneRectControlled(false), mParentPage( prnt ){
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
/*
void GESView::wheelEvent(QWheelEvent *event) {
	if (event->delta() > 0) {
		if (sceneScaleCombo->currentIndex() == 9)
			return;
		sceneScaleCombo->setCurrentIndex(sceneScaleCombo->currentIndex() + 1);
	} else {
		if (sceneScaleCombo->currentIndex() == 0)
			return;
		sceneScaleCombo->setCurrentIndex(sceneScaleCombo->currentIndex() - 1);
	}

	emit wheelEvent( event );

}*/

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

void GESView::setScale(const QString& sc)
{
    QTransform t = transform();
    //Default transform
    t.reset();

    //Getting percent value
    QString str = sc;
    str.remove("%");
    double d = str.toDouble();
    mParentPage->getSettings()->setZoom( d );
    d /= 100;

    //Setting transformation
    setTransform(t.scale(d,d),false);

    emit(scaleChanged(transform().mapRect(QRectF(0, 0, 1, 1)).width()));
}
