/*
 * GESTabWidget.cpp
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#include "GESTabWidget.h"
#include "GESPage.h"
#include "GESScene.h"
#include "GESView.h"

GESTabWidget::GESTabWidget() {

}

GESTabWidget::~GESTabWidget() {
}

void GESTabWidget::addPage(){
	addTab(new GESPage(), "New page");
}

void GESTabWidget::setSceneState( int st ){
	GESPage* page = getCurrentPage();
	if( getCurrentPage() != 0 )
		page->getScene()->setState( (GESScene::State) st );
}

void GESTabWidget::setScale(const QString& sc ){
	GESPage* page = getCurrentPage();
	if( getCurrentPage() != 0 )
		page->getView()->setScale( sc );
}

QPixmap* GESTabWidget::getViewportPixmap(){
	GESPage* page = getCurrentPage();

	if( getCurrentPage() == 0 )
		return 0;
	QPixmap* pix = new QPixmap( QPixmap::grabWidget(page->getView()->viewport()));
	return pix;
}

void GESTabWidget::selectAll(){
	GESPage* page = getCurrentPage();
	if( getCurrentPage() != 0 )
		page->getScene()->selectAll();
}

void GESTabWidget::copyObj(){
	GESPage* page = getCurrentPage();
	if( getCurrentPage() != 0 )
		page->getScene()->copyObj();
}

void GESTabWidget::pasteObj(){
	GESPage* page = getCurrentPage();
	if( getCurrentPage() != 0 )
		page->getScene()->pasteObj();
}

void GESTabWidget::redoCommand(){
	GESPage* page = getCurrentPage();
	if( getCurrentPage() != 0 )
		page->getScene()->redoCommand();
}

void GESTabWidget::undoCommand(){
	GESPage* page = getCurrentPage();
	if( getCurrentPage() != 0 )
		page->getScene()->undoCommand();
}

void GESTabWidget::deleteSelectedObj(){
	GESPage* page = getCurrentPage();
	if( getCurrentPage() != 0 )
		page->getScene()->deleteSelectedObj();
}

void GESTabWidget::cutObj(){
	GESPage* page = getCurrentPage();
	if( getCurrentPage() != 0 )
		page->getScene()->cutObj();
}

GESPage* GESTabWidget::getCurrentPage(){
	return qobject_cast< GESPage* >( currentWidget() );
}
