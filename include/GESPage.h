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

#include "Object.h"
#include <QWidget>

class GESPage : public QWidget {
	Q_OBJECT
public:
	GESPage();
	~GESPage();

    //! Удалить объект
    void delItem(Object* obj);

    //! Сцена
    GESScene* getScene(){
    	return mScene;
    }

    //! Просмоторщик
    GESView* getView() const {
    	return mView;
    }

    //! Сцена
    GESScene* setScene( GESScene* scn ){
    	return mScene;
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
};

#endif /* GESPAGE_H_ */
