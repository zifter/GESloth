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
 * GESPage.h
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#ifndef GESPAGE_H_
#define GESPAGE_H_

class GESScene;
class GESView;
class GESTabWidget;

#include <QWidget>
#include "PageSettings.h"

class GESPage : public QWidget {
	Q_OBJECT
public:
	explicit GESPage( GESTabWidget* prnt );
	~GESPage();

    //! Сцена
    GESScene* getScene(){
    	return mScene;
    }

    //! Просмоторщик
    GESView* getView() const {
    	return mView;
    }

    //! Сцена
    void setScene( GESScene* scn ){
    	mScene = scn;
    }

    //! Просмоторщик
    GESView* setView( GESView* view ){
    	return view;
    }

    void setFileName( const QString& name ){
    	mFileName = name;
    }

    QString getFileName() const {
    	return mFileName;
    }

    void setSettings( PageSettings* set ){
		mSettings = set;
	}
    PageSettings* getSettings(){
    	return mSettings;
    }


public slots:

    //! Изменение увеличения сцены
    void sceneScaleChanged(const QString &scale);

private:

    //! Событие таймера
    void timerEvent(QTimerEvent *event);

    //! Сцена
    GESScene* mScene;

    //! Просмоторщик
    GESView* mView;

    //! Name of file
    QString mFileName;

    //! Settings of this page
    PageSettings* mSettings;
};

#endif /* GESPAGE_H_ */
