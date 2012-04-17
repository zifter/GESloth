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

#ifndef GESLOTH_H
#define GESLOTH_H

#include <QMainWindow>
#include <QIcon>
#include <QTabWidget>

#include "PageSettings.h"

class Node;
class Edge;
class Object;
class QButtonGroup;
class QToolBar;
class QMenuBar;
class QDomElement;
class QGraphicsItem;
class QComboBox;
class QApplication;
class QTranslator;
class GESTabWidget;
class QLineEdit;
class GESScene;
class GESView;

class GESloth : public QMainWindow
{
    Q_OBJECT

public:
    GESloth(QApplication*);
    virtual ~GESloth(){};

    //! Объекты нуждаються в передвижении
    void itemMoved();

    //! Создание менюшек
    void createMenus();

protected:
    //! Событие нажатия каких либо клавиш
    void keyPressEvent(QKeyEvent *event);

    //! Создание панели инструментов
    void createToolBar();

    //! Create action in window
    void createAction();

    //! return icon path
    QIcon getIcon(const QString& name );

private slots:

    //! Экспортировка в изображение
    void exportToImage();

    //! Об этой лабе
    void about();

    /**
     * @brief Create new page
     */
    void newPage();

    //! О Кьют
    void aboutQt();

    //! Отключить псевдо физику
    void switchOff(bool);

    //! Открыть
    void Open();

    //! Сохранить
    void Save();

    //! Сохранить как
    void SaveAs();

    //! Изменить язык
    void switchLang(QAction*);

    //! Вызов контекстной помощи
    void help();

	/**
	 *	@brief Method for change view scale
	 */
    void viewScaleChanged(qreal);

	/**
	 * @breif Method for change scale factor
	 */
	void zoomIn();

	/**
	 * @breif Method for change scale factor
	 */
	void zoomOut();

	void closeTab( int );

	void changeTab( int );


private:

	void loadSettings( PageSettings* set );

	//! Tab widget
    GESTabWidget* tabWidget;

    //! Contents current zoom factor.
    QLineEdit* mZoomFactorLine;

    //! Default scale factor
    qint32 mScaleFactorChange;

    //! Индификатор таймера
    int timerId;

    //! Группа кнопок
    QButtonGroup *pointerTypeGroup;

    //! Панель инструментов
    QToolBar *mToolBar;

    //! Имя файла
    QString NameOfFile;

    QAction* mActionCut;
    QAction* mActionPaste;
    QAction* mActionCopy;
    QAction* mActionDelete;
    QAction* mActionRedo;
	QAction* mActionUndo;
	QAction* mActionClear;
    QAction* mActionSelectAll;

    // ! Minimun scale factor
    qint32 minScaleFactor;

    // ! Maximum scale factor
    qint32 maxScaleFactor;
};

#endif
