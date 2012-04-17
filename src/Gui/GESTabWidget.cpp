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

#include "Macros.h"

GESTabWidget::GESTabWidget() {
	setTabsClosable( true );
}

GESTabWidget::~GESTabWidget() {
}

void GESTabWidget::addPage(){
	addTab(new GESPage(), tr("New page"));
}

void GESTabWidget::addPage( QString& name, Graph* graph ){
	GESPage* page = new GESPage( );
	page->setFileName( name );
	page->getScene()->setGraph( graph );
	addTab( page, name );
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

QPixmap* GESTabWidget::getViewportPixmap(){



//    SCgScene *scene = qobject_cast<SCgScene*>(input);
//
//    QSize sz = scene->itemsBoundingRect().size().toSize();
//    QRect rect = scene->itemsBoundingRect().toRect();
//
//    QString isSVG = file_name.mid(file_name.length()-3);
//   /* if(isSVG=="svg"){
//         QSvgGenerator generator;
//         generator.setFileName(file_name);
//         generator.setSize(sz);
//         generator.setViewBox(rect);
//         //generator.setTitle(tr("SVG image for GWF"));
//         generator.setDescription(tr("An SVG drawing created by Knowledge base Editor."));
//         QPainter painter(&generator);
//         painter.setRenderHint(QPainter::Antialiasing,true);
//         scene->render(&painter,QRect(QPoint(0,0), sz),scene->itemsBoundingRect());
//         return true;
//     }
//    else*/{
//        QImage img(sz,QImage::Format_ARGB32_Premultiplied);
//        if (!img.isNull())
//        {
//            QPainter painter(&img);
//            painter.setRenderHint(QPainter::Antialiasing, true);
//            painter.eraseRect(QRect(QPoint(0,0),sz));
//            scene->renderToImage(&painter, QRect(QPoint(0,0), sz), scene->itemsBoundingRect());
//            return img.save(file_name);
//        }
//
//

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

void GESTabWidget::clear(){
	GESPage* page = getCurrentPage();
	if( getCurrentPage() != 0 )
		page->getScene()->clear();
}

GESPage* GESTabWidget::getCurrentPage(){
	return qobject_cast< GESPage* >( currentWidget() );
}
