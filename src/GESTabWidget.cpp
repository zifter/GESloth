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
	addTab(new GESPage(), tr("New page"));
}

void GESTabWidget::addPage( QString& name, Graph* graph ){
	GESPage* page = new GESPage( );
	page->getScene()->setGraph( graph );
	addTab( page, name );
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

GESPage* GESTabWidget::getCurrentPage(){
	return qobject_cast< GESPage* >( currentWidget() );
}
