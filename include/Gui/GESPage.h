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

#include <QWidget>
#include "PageSettings.h"

class GESPage : public QWidget {
	Q_OBJECT
public:
	GESPage();
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
