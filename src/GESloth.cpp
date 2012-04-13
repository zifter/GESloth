#include "GESloth.h"
#include "Edge.h"
#include "Node.h"
#include "Object.h"
#include "GESScene.h"

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

#include "HelpBrowser.h"

GraphWidget::GraphWidget(QApplication* app)
    : timerId(0)
{
    scene = new GraphRedactorScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-300, -300, 500, 500);
    resize(400,400);
    QHBoxLayout *layout = new QHBoxLayout;

    view = new GraphRedactorView();
    view->setScene(scene);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);

    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setRenderHint(QPainter::Antialiasing);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    view->setResizeAnchor(QGraphicsView::AnchorViewCenter);

    createToolBar();
    createMenus();
    view->sceneScaleCombo = sceneScaleCombo;
    view->scale(qreal(0.75), qreal(0.75));
    setMinimumSize(450, 450);
    setWindowTitle(tr("Graphic Editor"));
}

void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        break;
    case Qt::Key_Plus:
        if(sceneScaleCombo->currentIndex() == 9)
            break;
        sceneScaleCombo->setCurrentIndex(sceneScaleCombo->currentIndex()+1);
        break;
    case Qt::Key_Minus:
        if(sceneScaleCombo->currentIndex() == 0)
            break;
        sceneScaleCombo->setCurrentIndex(sceneScaleCombo->currentIndex()-1);
        break;
    case Qt::Key_Space:
        foreach (QGraphicsItem *item, scene->items())
            if (qgraphicsitem_cast<Node *>(item))
                item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
        break;
    case Qt::Key_Delete:
        scene->deleteSelectedObj();
        break;
    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
    if(event->matches(QKeySequence::SelectAll))
        scene->selectAll();
    if(event->matches(QKeySequence::Copy))
        scene->copyObj();
    if(event->matches(QKeySequence::Paste))
        scene->pasteObj();
    if(event->matches(QKeySequence::Undo))
        scene->undoCommand();
    if(event->matches(QKeySequence::Redo))
        scene->redoCommand();
}


void GraphWidget::delItem(Object* obj)
{
    scene->removeItem(obj);
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event); // �� ������������ ��� ����������

    foreach (Node *node, scene->getNodes())
        node->calculateForces();

    foreach (Node *node, scene->getNodes())
        node->advance();
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = view->matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    view->scale(scaleFactor, scaleFactor);
}

void GraphWidget::pointerGroupClicked( int )
{
    scene->setState(GraphRedactorScene::State(pointerTypeGroup->checkedId()));
    if( 1 == scene->getState())
        foreach (Node *node, scene->getNodes())
            node->setFlag(QGraphicsItem::ItemIsMovable, false);
    else
        foreach(Node *node, scene->getNodes())
            node->setFlag(QGraphicsItem::ItemIsMovable, true);
}

void GraphWidget::createToolBar()
{
    pointerTypeGroup = new QButtonGroup;
    //������
    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/image/pointer"));
    pointerButton->setToolTip(tr("Selection mode"));

    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/image/linepointer"));
    linePointerButton->setToolTip(tr("Edge mode"));

    pointerTypeGroup->addButton(pointerButton, int(GraphRedactorScene::InsertNode));
    pointerTypeGroup->addButton(linePointerButton,
                                int(GraphRedactorScene::InsertEdge));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << "25%" << "50%" << "75%" << "100%" << "125%" << "150%" << "175%" << "200%" << "250%" << "300%";
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(3);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(sceneScaleChanged(QString)));
    sceneScaleCombo->setToolTip(tr("Scale"));

    QToolButton *switchOff = new QToolButton;
    switchOff->setCheckable(true);
    switchOff->setChecked(true);
    switchOff->setToolTip(tr("Motion mode"));

    switchOff->setIcon(QIcon(":/image/switchOff"));
    connect(switchOff, SIGNAL(clicked(bool)),
            this, SLOT(switchOff(bool)));

    pointerToolbar = addToolBar(tr("Modes"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(switchOff);
    pointerToolbar->addWidget(sceneScaleCombo);
    pointerToolbar->setToolTip(tr("Mode"));

}

void GraphWidget::createMenus()
{
    QMenu* menuFile = new QMenu(tr("File"));
    QMenu* aboutMenu = new QMenu(tr("Help"));
    menuBar()->addMenu(menuFile);
    menuBar()->addMenu(aboutMenu);
    aboutMenu->addAction(tr("About"),this, SLOT(about()),Qt::CTRL + Qt::Key_B);
    aboutMenu->addAction(tr("About Qt"),this, SLOT(aboutQt()));
    aboutMenu->addAction(tr("Help"),this, SLOT(help()));
    menuFile->addAction(tr("Open"),this, SLOT(Open()),Qt::CTRL + Qt::Key_O);
    menuFile->addAction(tr("Save"),this, SLOT(Save()),Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    menuFile->addAction(tr("Save as"),this, SLOT(SaveAs()),Qt::CTRL + Qt::Key_S);
    menuFile->addAction(tr("Export"),this, SLOT(exportToImage()),Qt::CTRL + Qt::Key_X);

    QMenu* translateMenu = new QMenu(tr("Translation"));
    QActionGroup* languageActionGroup = new QActionGroup(this);
    connect(languageActionGroup, SIGNAL(triggered(QAction *)),
    this, SLOT(switchLang(QAction *)));

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

    if(tranas.contains("graphicsRedactor_en") )
        actEn->setChecked(true);
    if(tranas.contains("graphicsRedactor_ru") )
        actRus->setChecked(true);

    menuFile->addMenu(translateMenu);
    menuFile->addAction(tr("Exit"),this, SLOT(close()),Qt::CTRL + Qt::Key_E);
}

void GraphWidget::exportToImage()
{
    QString filters(tr("*.png\n"
                    "All Files(*.*)"));
    QString fileName = QCoreApplication::applicationDirPath() + "/" + tr("Insert name.png");
    QString selectedFilter;
    fileName = QFileDialog::getSaveFileName(this, tr("Export file to ..."), fileName, filters);

    if (!fileName.isEmpty())
        QPixmap::grabWidget(view->viewport()).save(fileName, "png");
}

void GraphWidget::aboutQt()
{
    QMessageBox::aboutQt(this, tr("About FUCKING QT"));
}

void GraphWidget::about()
{
    QMessageBox::about(this, QString(tr("About this Lab")),
        QString(tr("The <b>Graph Redactor</b> is laba for Oleg Strokachuk. <br/>Intended to build a directed graph with the possibility of pseudo-physics interaction ")));
}

void GraphWidget::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}
void GraphWidget::switchOff(bool flagSwtch)
{
    if(flagSwtch)
        timerId = startTimer(1000 / 25);
    else
        killTimer(timerId);
}

void GraphWidget::Open()
{
    QString FileName = QFileDialog::getOpenFileName(0,tr("Open graph"),"","*.grh");
    NameOfFile = FileName;
    openFromFile();
}

void GraphWidget::Save()
{
    if( NameOfFile == "" )
    {
        SaveAs();
        return;
    }
    saveToFile();
}

void GraphWidget::SaveAs()
{
    QString FileName = QFileDialog::getSaveFileName(0,tr("Save graph"),"","*.grh");
    NameOfFile = FileName;
    saveToFile();
}

void GraphWidget::openFromFile()
{
    QFile fileIn(NameOfFile);

    if(!fileIn.open(QFile::ReadOnly | QFile::Text))
         return;
    if( NameOfFile == "" )
    {
         QMessageBox::warning(0, qAppName(),
              tr("File open error.%1")
              .arg(NameOfFile));
         return;
    }

    QList< QGraphicsItem* > items;
    QByteArray bt = fileIn.readAll();
    fileIn.close();
    if( !scene->openFromByte(items, bt ))
    {
        QMessageBox::critical(0,tr("Error!"),tr("Error read XML-file!"),QMessageBox::Close);
    }
    scene->deleteObj(scene->items());
    scene->addItems(items);
}

void GraphWidget::saveToFile()
{
    QFile fileOut(NameOfFile);
    if (!fileOut.open(QFile::WriteOnly | QFile::Text))
        return;
    QByteArray bt;
    QList< QGraphicsItem* > items;
    items = scene->items();
    scene->saveToByte( items , bt );
    fileOut.write(bt);
    fileOut.close();
}


void GraphWidget::switchLang(QAction* act)
{
    QFile fileIni("Graphics_Redactor.ini");
    fileIni.open(QIODevice::WriteOnly);
    QTextStream in(&fileIni);
    if( act->text() == tr("English") )
        in << "graphicsRedactor_en";
    if( act->text() == tr("Russian") )
        in << "graphicsRedactor_ru";
    fileIni.close();
}

void GraphWidget::help()
{
    HelpBrowser::showPage("media/doc/index.html");
}














