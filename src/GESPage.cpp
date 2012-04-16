/*
 * GESPage.cpp
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#include "GESPage.h"
#include "GESScene.h"
#include "GESView.h"
#include <QHBoxLayout>

GESPage::GESPage() {
	mScene = new GESScene();
	mScene->setItemIndexMethod(QGraphicsScene::NoIndex);

	QHBoxLayout *layout = new QHBoxLayout;
	mView = new GESView();
	mView->setScene(mScene);
	mView->setSceneRect(0, 0, 500, 500);
	mView->setDragMode(QGraphicsView::RubberBandDrag);
	layout->addWidget(mView);

	setLayout(layout);
}

GESPage::~GESPage() {

}

void GESPage::delItem(Object* obj) {
	mScene->removeItem(obj);
}

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
