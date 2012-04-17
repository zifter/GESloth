#include "GESloth.h"
#include "Edge.h"
#include "Node.h"
#include "Object.h"
#include "GESScene.h"
#include "GESView.h"
#include "Macros.h"
#include "HelpBrowser.h"
#include "GESTabWidget.h"
#include "GESFileLoader.h"
#include "GESFileWriter.h"

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


GESloth::GESloth(QApplication* app) :
		timerId(0), mScaleFactorChange(25), minScaleFactor(25), maxScaleFactor(
				999) {
	tabWidget = new GESTabWidget();

	setMinimumSize(450, 450);
	setWindowTitle(tr("Graph Editor Sloth"));
	setCentralWidget(tabWidget);

	createToolBar();
	createAction();
	createMenus();

	connect(tabWidget, SIGNAL( currentChanged ( int ) ), this, SLOT( changeTab(int) ) );
	connect(tabWidget, SIGNAL( tabCloseRequested ( int ) ), this, SLOT( closeTab(int) ) );

}

void GESloth::itemMoved() {
	//if (!timerId)
		//timerId = startTimer(1000 / 25);
}

void GESloth::keyPressEvent(QKeyEvent *event) {
	/*switch (event->key()) {
	case Qt::Key_Space:
		foreach (QGraphicsItem *item, scene->items())
			if (qgraphicsitem_cast<Node *>(item))
				item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
		break;
	}
	QMainWindow::keyPressEvent(event);*/
}




void GESloth::createToolBar() {
	pointerTypeGroup = new QButtonGroup;

	QToolButton *pointerButton = new QToolButton;
	pointerButton->setCheckable(true);
	pointerButton->setChecked(true);
	pointerButton->setIcon(getIcon("pointer"));
	pointerButton->setToolTip(tr("Selection mode"));

	QToolButton *linePointerButton = new QToolButton;
	linePointerButton->setCheckable(true);
	linePointerButton->setIcon(getIcon("linepointer"));
	linePointerButton->setToolTip(tr("Edge mode"));

	pointerTypeGroup->addButton(pointerButton, int(PageSettings::Node));
	pointerTypeGroup->addButton(linePointerButton, int(PageSettings::Edge));
	connect(pointerTypeGroup, SIGNAL(buttonClicked(int)), tabWidget,
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

	// about
	QAction* action = new QAction(QIcon::fromTheme("help-about", getIcon("about")), tr("About... "), this);
	action->setCheckable(false);
	action->setStatusTip(tr("Show some little info about this program"));
	action->setIconVisibleInMenu(true);
	connect(action, SIGNAL(triggered()), this, SLOT(about()));
	menuHelp->addAction(action);

	// about qt
	action = new QAction(getIcon("aboutQt"), tr("About Qt..."), this);
	action->setCheckable(false);
	action->setStatusTip(tr("Show the Qt library's About box"));
	action->setIconVisibleInMenu(true);
	connect(action, SIGNAL(triggered()), this, SLOT(aboutQt()));
	menuHelp->addAction(action);

	// new
	action = new QAction(QIcon::fromTheme("document-new", getIcon("new")), tr("New..."), this);
	action->setShortcut(QKeySequence::New);
	action->setIconVisibleInMenu(true);
	action->setCheckable(false);
	action->setStatusTip(tr("New file"));
	connect(action, SIGNAL(triggered()), this, SLOT(newPage()));
	menuFile->addAction(action);

	// open
	action = new QAction(QIcon::fromTheme("document-open", getIcon("open")), tr("Open..."), this);
	action->setShortcut(QKeySequence::Open);
	action->setIconVisibleInMenu(true);
	action->setCheckable(false);
	action->setStatusTip(tr("Open file"));
	connect(action, SIGNAL(triggered()), this, SLOT(Open()));
	menuFile->addAction(action);

	// save
	action = new QAction(QIcon::fromTheme("document-save", getIcon("save")), tr("Save..."), this);
	action->setShortcut(QKeySequence::Save);
	action->setIconVisibleInMenu(true);
	action->setCheckable(false);
	action->setStatusTip(tr("Save file"));
	connect(action, SIGNAL(triggered()), this, SLOT(Save()));
	menuFile->addAction(action);

	// save as
	action = new QAction(QIcon::fromTheme("document-save-as", getIcon("saveAs")),tr("Save as..."), this);
	action->setShortcut(QKeySequence::SaveAs);
	action->setCheckable(false);
	action->setStatusTip(tr("Save file as"));
	action->setIconVisibleInMenu(true);
	connect(action, SIGNAL(triggered()), this, SLOT(SaveAs()));
	menuFile->addAction(action);

	menuFile->addSeparator();

	// export
	action = new QAction(QIcon::fromTheme("insert-image", getIcon("export")), tr("Export..."), this);
	action->setCheckable(false);
	action->setStatusTip(tr("Export file to image"));
	action->setIconVisibleInMenu(true);
	connect(action, SIGNAL(triggered()), this, SLOT(exportToImage()));
	menuFile->addAction(action);

	QMenu* translateMenu = new QMenu(tr("Translation"));
	translateMenu->setIcon( QIcon::fromTheme("preferences-desktop-locale", getIcon("translate")));
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

	// close
	action = new QAction(QIcon::fromTheme("window-close", getIcon("close")), tr("Close..."), this);
	action->setShortcut(QKeySequence::Close);
	action->setCheckable(false);
	action->setStatusTip(tr("Exit from program"));
	action->setIconVisibleInMenu(true);
	connect(action, SIGNAL(triggered()), this, SLOT(close()));
	menuFile->addAction(action);
}

void GESloth::createAction() {
	QAction* action;

	mToolBar->addSeparator();
    mToolBar->setIconSize(QSize(42, 32));

	//Scale combobox
	QComboBox* b = new QComboBox(mToolBar);
	b->setEditable(true);
	b->setFixedWidth( 62 );
	b->setInsertPolicy(QComboBox::NoInsert);
	QList<QString> scales;
	scales << "25%" << "50%" << "75%" << "100%" << "125%" << "150%" << "175%"
			<< "200%" << "250%" << "300%";

	b->addItems(QStringList(scales));
	b->setCurrentIndex(scales.indexOf("100%"));
	mZoomFactorLine = b->lineEdit();
	mZoomFactorLine->setInputMask("D90%");
	mToolBar->addWidget(b);
	connect(mZoomFactorLine, SIGNAL( textChanged(const QString&)), tabWidget,
			SLOT(setScale(const QString&)));
	connect(tabWidget, SIGNAL(scaleChanged(qreal)), this,
			SLOT(viewScaleChanged(qreal)));

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
	connect(mActionSelectAll, SIGNAL(triggered()), tabWidget, SLOT(selectAll()));

	// copy
	mActionCopy = new QAction(QIcon::fromTheme("edit-copy", getIcon("copy")),
			tr("Copy..."), this);
	mActionCopy->setShortcut(QKeySequence::Copy);
	mActionCopy->setIconVisibleInMenu(true);
	mActionCopy->setStatusTip("Copy selected items");
	connect(mActionCopy, SIGNAL(triggered()), tabWidget, SLOT(copyObj()));

	// paste
	mActionPaste = new QAction(QIcon::fromTheme("edit-paste", getIcon("paste")),
			tr("Paste..."), this);
	mActionPaste->setShortcut(QKeySequence::Paste);
	mActionPaste->setIconVisibleInMenu(true);
	mActionPaste->setStatusTip("Paste items from buffer");
	connect(mActionPaste, SIGNAL(triggered()), tabWidget, SLOT(pasteObj()));

	// undo
	mActionUndo = new QAction(QIcon::fromTheme("edit-undo", getIcon("undo")),
			tr("Undo..."), this);
	mActionUndo->setShortcut(QKeySequence::Undo);
	mActionUndo->setIconVisibleInMenu(true);
	mActionUndo->setStatusTip("Cancel previous command");
	connect(mActionUndo, SIGNAL(triggered()), tabWidget, SLOT(undoCommand()));

	// redo
	mActionRedo = new QAction(QIcon::fromTheme("edit-redo", getIcon("redo")),
			tr("Redo..."), this);
	mActionRedo->setShortcut(QKeySequence::Redo);
	mActionRedo->setIconVisibleInMenu(true);
	mActionRedo->setStatusTip("Redo previous undo-command");
	connect(mActionRedo, SIGNAL(triggered()), tabWidget, SLOT(redoCommand()));

	// delete
	mActionDelete = new QAction(
			QIcon::fromTheme("edit-delete", getIcon("delete")), tr("Delete..."),
			this);
	mActionDelete->setShortcut(QKeySequence::Delete);
	mActionDelete->setIconVisibleInMenu(true);
	mActionDelete->setStatusTip("Delete selected items");
	connect(mActionDelete, SIGNAL(triggered()), tabWidget,
			SLOT(deleteSelectedObj()));

	// clear
	mActionClear = new QAction(
			QIcon::fromTheme("edit-clear", getIcon("clear")), tr("Clear..."),
			this);
	mActionClear->setShortcut(QKeySequence::Refresh);
	mActionClear->setIconVisibleInMenu(true);
	mActionClear->setStatusTip("Clear page");
	connect(mActionClear, SIGNAL(triggered()), tabWidget,
			SLOT(clear()));

	// cut
	mActionCut = new QAction(QIcon::fromTheme("edit-cut", getIcon("cut")),
			tr("Cut..."), this);
	mActionCut->setShortcut(QKeySequence::Cut);
	mActionCut->setIconVisibleInMenu(true);
	mActionSelectAll->setStatusTip("Cut selected items");
	connect(mActionCut, SIGNAL(triggered()), tabWidget, SLOT(cutObj()));

}

void GESloth::exportToImage() {
//	QString filters(tr("*.png\n"
//			"All Files(*.*)"));
//	QString fileName = QCoreApplication::applicationDirPath() + "/"
//			+ tr("Insert name.png");
//	QString selectedFilter;
//	fileName = QFileDialog::getSaveFileName(this, tr("Export file to ..."),
//			fileName, filters);
//
//	if (!fileName.isEmpty())
//		QPixmap::grabWidget(mView->viewport()).save(fileName, "png");
	tabWidget->getViewportPixmap()->save("asd","png");
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
	if (flagSwtch)
		timerId = startTimer(1000 / 25);
	else
		killTimer(timerId);
}

void GESloth::Open() {
	QString FileName = QFileDialog::getOpenFileName(0, tr("Open graph"), "",
			"*.grh");

	if( FileName == "" )
		return;

	GESFileLoader loader;
	Graph* graph = new Graph();
	if( !loader.load( graph, FileName ) )
		loader.showError();
	tabWidget->addPage(FileName, graph );

}

void GESloth::Save() {
	GESPage* page = tabWidget->getCurrentPage();
	if (page->getFileName() == "" ) {
		SaveAs();
		return;
	}
	GESFileWriter writer;
	writer.write( page->getScene()->getGraph(), page->getFileName() );
}

void GESloth::SaveAs() {
	GESPage* page = tabWidget->getCurrentPage();
	QString FileName = QFileDialog::getSaveFileName(0, tr("Save graph"), "",
			"*.grh");
	if( FileName == "" )
		return;
	GESFileWriter writer;
	writer.write( page->getScene()->getGraph(), FileName );
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

void GESloth::newPage(){
	tabWidget->addPage();
}

void GESloth::viewScaleChanged(qreal newScale) {
	qreal oldScale = mZoomFactorLine->text().remove('%').toDouble() / 100;
	if (newScale != oldScale)
		mZoomFactorLine->setText(QString::number(int(newScale * 100)));
}

void GESloth::zoomIn() {
	qint32 oldScale = mZoomFactorLine->text().remove('%').toInt();
	qint32 newScale = oldScale - mScaleFactorChange;
	if (newScale < minScaleFactor)
		newScale = minScaleFactor;

	mZoomFactorLine->setText(QString::number(newScale));
}

void GESloth::zoomOut() {
	qint32 oldScale = mZoomFactorLine->text().remove('%').toInt();
	qint32 newScale = oldScale + mScaleFactorChange;

	if (newScale > maxScaleFactor)
		newScale = maxScaleFactor;

	mZoomFactorLine->setText(QString::number(newScale));
}

void GESloth::closeTab( int index ){
	tabWidget->removeTab( index );
}

void GESloth::changeTab( int ){
	GESPage* page = tabWidget->getCurrentPage();
	if( page != 0 )
		loadSettings(page->getSettings());
}

void GESloth::loadSettings( PageSettings* set ){
	pointerTypeGroup->button((int)set->getState())->setChecked(true);
	mZoomFactorLine->setText(QString::number(set->getZoom()));
}

