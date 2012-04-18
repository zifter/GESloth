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

#include <QApplication>
#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QToolButton>
#include <QToolBar>
#include <QButtonGroup>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QCoreApplication>
#include <QPixmap>
#include <math.h>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QMap>
#include <QTranslator>
#include <QActionGroup>
#include <QAction>
#include <QLineEdit>
#include <QStringList>
#include <QList>

#include "Gui/GESloth.h"
#include "Gui/GESScene.h"
#include "Gui/GESView.h"
#include "Gui/GESTabWidget.h"

#include "Graph/Edge.h"
#include "Graph/Node.h"
#include "Graph/Object.h"

#include "Macros.h"
#include "HelpBrowser.h"

#include "XML/GESFileLoader.h"
#include "XML/GESFileWriter.h"

const int GESloth::mScaleFactorChange = 25;
const int GESloth::minScaleFactor = 25;
const int GESloth::maxScaleFactor = 999;

GESloth::GESloth(QApplication* app) : mFileExt(".ges"), isActiveToolBar(false){
	mTabWidget = new GESTabWidget( );

	setMinimumSize(450, 450);
	setWindowTitle(tr("Graph Editor Sloth"));
	setCentralWidget(mTabWidget);

	createToolBar();
	createAction();
	createMenus();

	mImageFilter = "*.png *.jpeg";
	mOpenFilter = QString("*" + mFileExt);
	mSaveFilter = QString("*" + mFileExt);

	connect(mTabWidget, SIGNAL( currentChanged ( int ) ), this,
			SLOT( changeTab(int) ));
	connect(mTabWidget, SIGNAL( tabCloseRequested ( int ) ), this,
			SLOT( closeTab(int) ));

}

void GESloth::createToolBar() {
	mToolGroup = new QButtonGroup;

	QToolButton *pointerButton = new QToolButton;
	pointerButton->setCheckable(true);
	pointerButton->setChecked(true);
	pointerButton->setIcon(getIcon("pointer"));
	pointerButton->setToolTip(tr("Selection mode"));

	QToolButton *linePointerButton = new QToolButton;
	linePointerButton->setCheckable(true);
	linePointerButton->setIcon(getIcon("linepointer"));
	linePointerButton->setToolTip(tr("Edge mode"));

	mToolGroup->addButton(pointerButton, int(PageSettings::Node));
	mToolGroup->addButton(linePointerButton, int(PageSettings::Edge));
	connect(mToolGroup, SIGNAL(buttonClicked(int)), mTabWidget,
			SLOT(setSceneState(int)));

	QToolButton *switchOff = new QToolButton;
	switchOff->setCheckable(true);
	switchOff->setChecked(true);
	switchOff->setToolTip(tr("Motion mode"));

	switchOff->setIcon(getIcon("switchOff"));
	connect(switchOff, SIGNAL(clicked(bool)), this, SLOT(switchOff(bool)));

	mToolBar = addToolBar(tr("Modes"));
	mToolBar->addWidget(pointerButton);
	mToolBar->addWidget(linePointerButton);
	mToolBar->addWidget(switchOff);
	mToolBar->setToolTip(tr("Mode"));

}

void GESloth::createMenus() {
	QMenu* menuFile = new QMenu(tr("File"), this);
	QMenu* menuHelp = new QMenu(tr("Help"), this);
	QMenu* menuEdit = new QMenu(tr("Edit"), this);

	menuBar()->addMenu(menuFile);
	menuBar()->addMenu(menuEdit);
	menuBar()->addMenu(menuHelp);

	menuEdit->addAction(mActionCopy);
	menuEdit->addAction(mActionPaste);
	menuEdit->addAction(mActionCut);
	menuEdit->addSeparator();
	menuEdit->addAction(mActionSelectAll);
	menuEdit->addSeparator();
	menuEdit->addAction(mActionClear);
	menuEdit->addAction(mActionDelete);
	menuEdit->addSeparator();
	menuEdit->addAction(mActionRedo);
	menuEdit->addAction(mActionUndo);

	menuHelp->addAction(mActionAbout);
	menuHelp->addAction(mActionAboutQt);

	menuFile->addAction(mActionNew);
	menuFile->addAction(mActionOpen);
	menuFile->addAction(mActionSave);
	menuFile->addAction(mActionSaveAs);

	menuFile->addSeparator();
	menuFile->addAction(mActionExport);

	QMenu* translateMenu = new QMenu(tr("Translation"));
	translateMenu->setIcon(
			QIcon::fromTheme("preferences-desktop-locale",
					getIcon("translate")));
	QActionGroup* languageActionGroup = new QActionGroup(this);
	connect(languageActionGroup, SIGNAL(triggered(QAction *)), this,
			SLOT(switchLang(QAction *)));

	QAction* actEn = languageActionGroup->addAction(tr("English"));
	QAction* actRus = languageActionGroup->addAction(tr("Russian"));

	actEn->setCheckable(true);
	actRus->setCheckable(true);

	QFile fileIni("Graphics_Redactor.ini");
	fileIni.open(QIODevice::ReadOnly);
	QTextStream in(&fileIni);
	QString tranas = in.readAll();
	fileIni.close();

	translateMenu->addAction(actEn);
	translateMenu->addAction(actRus);

	if (tranas.contains("graphicsRedactor_en"))
		actEn->setChecked(true);
	if (tranas.contains("graphicsRedactor_ru"))
		actRus->setChecked(true);

	menuFile->addSeparator();
	menuFile->addMenu(translateMenu);
	menuFile->addSeparator();
	menuFile->addAction(mActionClose);

	deactivateToolBar();

}

void GESloth::activateToolBar(){
	isActiveToolBar = true;
	mActionSaveAs->setDisabled(false);
	mActionSave->setDisabled(false);
	mActionExport->setDisabled(false);

	mActionCut->setDisabled(false);
	mActionPaste->setDisabled(false);
	mActionCopy->setDisabled(false);
	mActionDelete->setDisabled(false);
	mActionRedo->setDisabled(false);
	mActionUndo->setDisabled(false);
	mActionClear->setDisabled(false);
	mActionSelectAll->setDisabled(false);
	mToolBar->show();
}
void GESloth::deactivateToolBar(){
	isActiveToolBar = false;
	mActionSaveAs->setDisabled(true);
	mActionSave->setDisabled(true);
	mActionExport->setDisabled(true);

	mActionCut->setDisabled(true);
	mActionPaste->setDisabled(true);
	mActionCopy->setDisabled(true);
	mActionDelete->setDisabled(true);
	mActionRedo->setDisabled(true);
	mActionUndo->setDisabled(true);
	mActionClear->setDisabled(true);
	mActionSelectAll->setDisabled(true);
	mToolBar->hide();
}

void GESloth::createAction() {
	QAction* action;

	mToolBar->addSeparator();
	mToolBar->setIconSize(QSize(42, 32));

	//Scale combobox
	QComboBox* b = new QComboBox(mToolBar);
	b->setEditable(true);
	b->setFixedWidth(70);
	QFont* font = new QFont();
	font->setPointSize(10);
	b->setFont(*font);
	b->setInsertPolicy(QComboBox::NoInsert);
	QList<QString> scales;
	scales << "25%" << "50%" << "75%" << "100%" << "125%" << "150%" << "175%"
			<< "200%" << "250%" << "300%";

	b->addItems(QStringList(scales));
	b->setCurrentIndex(scales.indexOf("100%"));
	mZoomFactorLine = b->lineEdit();
	mZoomFactorLine->setInputMask("D90%");
	mToolBar->addWidget(b);
	connect(mZoomFactorLine, SIGNAL( textChanged(const QString&)), mTabWidget,
			SLOT(setScale(const QString&)));

	// about
    mActionAbout = new QAction(
			QIcon::fromTheme("help-about", getIcon("about")), tr("About... "),
			this);
    mActionAbout->setCheckable(false);
    mActionAbout->setStatusTip(tr("Show some little info about this program"));
    mActionAbout->setIconVisibleInMenu(true);
	connect(mActionAbout, SIGNAL(triggered()), this, SLOT(about()));

	// about qt
	mActionAboutQt = new QAction(getIcon("aboutQt"), tr("About Qt..."), this);
	mActionAboutQt->setCheckable(false);
	mActionAboutQt->setStatusTip(tr("Show the Qt library's About box"));
	mActionAboutQt->setIconVisibleInMenu(true);
	connect(mActionAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt()));

	// new
	mActionNew = new QAction(QIcon::fromTheme("document-new", getIcon("new")),
			tr("New..."), this);
	mActionNew->setShortcut(QKeySequence::New);
	mActionNew->setIconVisibleInMenu(true);
	mActionNew->setCheckable(false);
	mActionNew->setStatusTip(tr("New file"));
	connect(mActionNew, SIGNAL(triggered()), this, SLOT(newPage()));

	// open
	mActionOpen = new QAction(QIcon::fromTheme("document-open", getIcon("open")),
			tr("Open..."), this);
	mActionOpen->setShortcut(QKeySequence::Open);
	mActionOpen->setIconVisibleInMenu(true);
	mActionOpen->setCheckable(false);
	mActionOpen->setStatusTip(tr("Open file"));
	connect(mActionOpen, SIGNAL(triggered()), this, SLOT(Open()));

	// save
	mActionSave = new QAction(QIcon::fromTheme("document-save", getIcon("save")),
			tr("Save..."), this);
	mActionSave->setShortcut(QKeySequence::Save);
	mActionSave->setIconVisibleInMenu(true);
	mActionSave->setCheckable(false);
	mActionSave->setStatusTip(tr("Save file"));
	connect(mActionSave, SIGNAL(triggered()), this, SLOT(Save()));

	// save as
	mActionSaveAs = new QAction(
			QIcon::fromTheme("document-save-as", getIcon("saveAs")),
			tr("Save as..."), this);
	mActionSaveAs->setShortcut(QKeySequence::SaveAs);
	mActionSaveAs->setCheckable(false);
	mActionSaveAs->setStatusTip(tr("Save file as"));
	mActionSaveAs->setIconVisibleInMenu(true);
	connect(mActionSaveAs, SIGNAL(triggered()), this, SLOT(SaveAs()));

	// export
	mActionExport = new QAction(QIcon::fromTheme("insert-image", getIcon("export")),
			tr("Export..."), this);
	mActionExport->setCheckable(false);
	mActionExport->setStatusTip(tr("Export file to image"));
	mActionExport->setIconVisibleInMenu(true);
	connect(mActionExport, SIGNAL(triggered()), this, SLOT(exportToImage()));

	// close
	mActionClose = new QAction(QIcon::fromTheme("window-close", getIcon("close")),
			tr("Close..."), this);
	mActionClose->setShortcut(QKeySequence::Close);
	mActionClose->setCheckable(false);
	mActionClose->setStatusTip(tr("Exit from program"));
	mActionClose->setIconVisibleInMenu(true);
	connect(mActionClose, SIGNAL(triggered()), this, SLOT(close()));


	//Zoom out
	action = new QAction(QIcon::fromTheme("zoom-out", getIcon("zoomOut")),
			tr("Zoom out (-)"), mToolBar);
	action->setCheckable(false);
	action->setShortcut(QKeySequence::ZoomOut);
	mToolBar->addAction(action);
	connect(action, SIGNAL(triggered()), this, SLOT(zoomOut()));

	//Zoom in
	action = new QAction(QIcon::fromTheme("zoom-in", getIcon("zoomIn")),
			tr("Zoom in (+)"), mToolBar);
	action->setCheckable(false);
	action->setShortcut(QKeySequence::ZoomIn);
	mToolBar->addAction(action);
	connect(action, SIGNAL(triggered()), this, SLOT(zoomIn()));

	// select all
	mActionSelectAll = new QAction(
			QIcon::fromTheme("edit-select-all", getIcon("selectAll")),
			tr("Select All..."), this);
	mActionSelectAll->setShortcut(QKeySequence::SelectAll);
	mActionSelectAll->setIconVisibleInMenu(true);
	mActionSelectAll->setStatusTip("Select all items");
	connect(mActionSelectAll, SIGNAL(triggered()), mTabWidget,
			SLOT(selectAll()));

	// copy
	mActionCopy = new QAction(QIcon::fromTheme("edit-copy", getIcon("copy")),
			tr("Copy..."), this);
	mActionCopy->setShortcut(QKeySequence::Copy);
	mActionCopy->setIconVisibleInMenu(true);
	mActionCopy->setStatusTip("Copy selected items");
	connect(mActionCopy, SIGNAL(triggered()), mTabWidget, SLOT(copyObj()));

	// paste
	mActionPaste = new QAction(QIcon::fromTheme("edit-paste", getIcon("paste")),
			tr("Paste..."), this);
	mActionPaste->setShortcut(QKeySequence::Paste);
	mActionPaste->setIconVisibleInMenu(true);
	mActionPaste->setStatusTip("Paste items from buffer");
	connect(mActionPaste, SIGNAL(triggered()), mTabWidget, SLOT(pasteObj()));

	// undo
	mActionUndo = new QAction(QIcon::fromTheme("edit-undo", getIcon("undo")),
			tr("Undo..."), this);
	mActionUndo->setShortcut(QKeySequence::Undo);
	mActionUndo->setIconVisibleInMenu(true);
	mActionUndo->setStatusTip("Cancel previous command");
	connect(mActionUndo, SIGNAL(triggered()), mTabWidget, SLOT(undoCommand()));

	// redo
	mActionRedo = new QAction(QIcon::fromTheme("edit-redo", getIcon("redo")),
			tr("Redo..."), this);
	mActionRedo->setShortcut(QKeySequence::Redo);
	mActionRedo->setIconVisibleInMenu(true);
	mActionRedo->setStatusTip("Redo previous undo-command");
	connect(mActionRedo, SIGNAL(triggered()), mTabWidget, SLOT(redoCommand()));

	// delete
	mActionDelete = new QAction(
			QIcon::fromTheme("edit-delete", getIcon("delete")), tr("Delete..."),
			this);
	mActionDelete->setShortcut(QKeySequence::Delete);
	mActionDelete->setIconVisibleInMenu(true);
	mActionDelete->setStatusTip("Delete selected items");
	connect(mActionDelete, SIGNAL(triggered()), mTabWidget,
			SLOT(deleteSelectedObj()));

	// clear
	mActionClear = new QAction(QIcon::fromTheme("edit-clear", getIcon("clear")),
			tr("Clear..."), this);
	mActionClear->setShortcut(QKeySequence::Refresh);
	mActionClear->setIconVisibleInMenu(true);
	mActionClear->setStatusTip("Clear page");
	connect(mActionClear, SIGNAL(triggered()), mTabWidget, SLOT(clear()));

	// cut
	mActionCut = new QAction(QIcon::fromTheme("edit-cut", getIcon("cut")),
			tr("Cut..."), this);
	mActionCut->setShortcut(QKeySequence::Cut);
	mActionCut->setIconVisibleInMenu(true);
	mActionSelectAll->setStatusTip("Cut selected items");
	connect(mActionCut, SIGNAL(triggered()), mTabWidget, SLOT(cutObj()));

}

void GESloth::exportToImage() {
	GESPage* page = mTabWidget->getCurrentPage();
	QString FileName = QFileDialog::getSaveFileName(0, tr("Save graph to image"), "",
			mImageFilter);
	if (FileName == "" || page == 0)
		return;
	QImage* tmp = mTabWidget->getSceneImage();
	tmp->save(FileName, "png");
	delete tmp;
}

void GESloth::aboutQt() {
	QMessageBox::aboutQt(this, tr("About FUCKING QT"));
}

void GESloth::about() {
	QMessageBox::about(
			this,
			QString(tr("About this Lab")),
			QString(
					tr(
							"The <b>Graph Redactor</b> is laba for Oleg Strokachuk. <br/>Intended to build a directed graph with the possibility of pseudo-physics interaction ")));
}

void GESloth::switchOff(bool flagSwtch) {
	//if (flagSwtch)
	//	timerId = startTimer(1000 / 25);
	//else
	//	killTimer(timerId);
}

void GESloth::Open() {
	QString FileName = QFileDialog::getOpenFileName(0, tr("Open graph"), "",
			mOpenFilter);

	if (FileName == "")
		return;

	GESFileLoader loader;
	GESPage* page = new GESPage( mTabWidget );
	page->setFileName( FileName );
	if (!loader.load(page) )
		loader.showError();
	if( !isActiveToolBar )
		activateToolBar();
	mTabWidget->addPage(page);

}

void GESloth::Save() {
	GESPage* page = mTabWidget->getCurrentPage();
	if (page->getFileName() == "") {
		SaveAs();
		return;
	}
	GESFileWriter writer;
	writer.write( page );
}

void GESloth::SaveAs() {
	GESPage* page = mTabWidget->getCurrentPage();
	QString FileName = QFileDialog::getSaveFileName(0, tr("Save graph"), "",
			mSaveFilter);
	if (FileName == "")
		return;
	FileName += mFileExt;
	GESFileWriter writer;
	page->setFileName( FileName );
	writer.write(page);
}

void GESloth::switchLang(QAction* act) {
	QFile fileIni("Graphics_Redactor.ini");
	fileIni.open(QIODevice::WriteOnly);
	QTextStream in(&fileIni);
	if (act->text() == tr("English"))
		in << "graphicsRedactor_en";
	if (act->text() == tr("Russian"))
		in << "graphicsRedactor_ru";
	fileIni.close();
}

void GESloth::help() {
	HelpBrowser::showPage("media/doc/index.html");
}

QIcon GESloth::getIcon(const QString& name) {
	return QIcon(":/image/" + name);
}

void GESloth::newPage() {
	if( !isActiveToolBar )
		activateToolBar();
	mTabWidget->addNewPage();
}

void GESloth::viewScaleChanged(int newScale) {
	qreal oldScale = mZoomFactorLine->text().remove('%').toInt();
	if (newScale != oldScale)
		mZoomFactorLine->setText(QString::number( newScale));
}

void GESloth::zoomIn() {
	qint32 oldScale = mZoomFactorLine->text().remove('%').toInt();
	qint32 newScale = oldScale + mScaleFactorChange;
	if (newScale < minScaleFactor)
		newScale = minScaleFactor;
	mZoomFactorLine->setText(QString::number(newScale));
}

void GESloth::zoomOut() {
	qint32 oldScale = mZoomFactorLine->text().remove('%').toInt();
	qint32 newScale = oldScale - mScaleFactorChange;
	if (newScale > maxScaleFactor)
		newScale = maxScaleFactor;
	mZoomFactorLine->setText(QString::number(newScale));
}

void GESloth::closeTab(int index) {
	mTabWidget->removePage(index);
	if( mTabWidget->currentIndex() == -1 )
		deactivateToolBar();
}

void GESloth::changeTab(int) {
	GESPage* page = mTabWidget->getCurrentPage();
	if (page != 0)
		loadSettings(page->getSettings());
}

void GESloth::loadSettings(PageSettings* set) {
	mToolGroup->button((int) set->getState())->setChecked(true);
	mZoomFactorLine->setText(QString::number(set->getZoom()));
}

void GESloth::wheelEvent( QWheelEvent* event ){
	if (event->modifiers() == Qt::ControlModifier) {
		mZoomFactorLine->setText(QString::number(mTabWidget->getCurrentPage()->getSettings()->getZoom()));
	} else
		emit QWidget::wheelEvent(event);
}
