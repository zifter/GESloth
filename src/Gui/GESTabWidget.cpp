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
 * GESTabWidget.cpp
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#include "Gui/GESTabWidget.h"
#include "Gui/GESScene.h"
#include "Gui/GESView.h"
#include "Gui/GESPage.h"

#include "Macros.h"
GESTabWidget::GESTabWidget( )  {
	setTabsClosable( true );
}

GESTabWidget::~GESTabWidget() {

}

void GESTabWidget::addNewPage(){
	addTab(new GESPage( this ), tr("New page"));
}

void GESTabWidget::addPage( GESPage* page ){
	addTab( page , page->getFileName() );
}

void GESTabWidget::removePage(int index){
	GESPage* page = qobject_cast< GESPage* >( widget( index ) );
	removeTab( index );
	delete page;
}

void GESTabWidget::setSceneState( int st ){
	GESPage* page = getCurrentPage();
	if( getCurrentPage() != 0 )
		page->getScene()->setState( (PageSettings::DrawState) st );
}

void GESTabWidget::setScale(const QString& sc ){
	GESPage* page = getCurrentPage();
	if( getCurrentPage() != 0 )
		page->getView()->setScale( sc );
}

QImage* GESTabWidget::getSceneImage(){
	GESPage* page = getCurrentPage();
    GESScene* scene = page->getScene();

	QSize sz = scene->itemsBoundingRect().size().toSize();
    //QRect rect = scene->itemsBoundingRect().toRect();

	QImage* pix = new QImage( sz,QImage::Format_ARGB32_Premultiplied);
	if (!pix->isNull())
	{
		QPainter painter(pix);
		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.eraseRect(QRect(QPoint(0,0),sz));
		scene->renderToImage(&painter, QRect(QPoint(0,0), sz), scene->itemsBoundingRect());
	}
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

void GESTabWidget::clear(){
	GESPage* page = getCurrentPage();
	if( getCurrentPage() != 0 )
		page->getScene()->clear();
}

GESPage* GESTabWidget::getCurrentPage(){
	return qobject_cast< GESPage* >( currentWidget() );
}
