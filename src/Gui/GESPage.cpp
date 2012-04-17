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
 * GESPage.cpp
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#include <QHBoxLayout>

#include "Gui/GESPage.h"
#include "Gui/GESScene.h"
#include "Gui/GESView.h"
#include "Gui/GESTabWidget.h"

#include "Macros.h"

GESPage::GESPage( GESTabWidget* prnt ) : QWidget( prnt ), mFileName(""){
	mSettings = new PageSettings();
	mScene = new GESScene(this);
	mScene->setItemIndexMethod(QGraphicsScene::NoIndex);

	QHBoxLayout *layout = new QHBoxLayout;
	mView = new GESView( this );
	mView->setScene(mScene);
	mView->setSceneRect(0, 0, 500, 500);
	mView->setDragMode(QGraphicsView::RubberBandDrag);
	layout->addWidget(mView);

	setLayout(layout);
}

GESPage::~GESPage() {
	delete mScene;
	delete mSettings;
	delete mView;
}
/*
void GESPage::delItem(Object* obj) {
	mScene->removeItem(obj);
}*/

void GESPage::sceneScaleChanged(const QString &scale) {
	double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
	QMatrix oldMatrix = mView->matrix();
	mView->resetMatrix();
	mView->translate(oldMatrix.dx(), oldMatrix.dy());
	mView->scale(newScale, newScale);
}

void GESPage::timerEvent(QTimerEvent *event) {
//	Q_UNUSED(event);
//
//	foreach (Node *node, scene->getNodes())
//		node->calculateForces();
//
//	foreach (Node *node, scene->getNodes())
//		node->advance();
}
