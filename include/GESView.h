/*
 * GESView.h
 *
 *  Created on: Apr 15, 2012
 *      Author: zifter
 */

#ifndef GESVIEW_H_
#define GESVIEW_H_
#include <QGraphicsView>
#include <QComboBox>
#include <QWheelEvent>
#include <GESScene.h>

//! Просмоторщик сцены
class GESView : public QGraphicsView
{
    Q_OBJECT

public:
    //! Пустой конструктоh
    GESView();
    virtual ~GESView();

   void setScene( GESScene* scene );

public slots:
	//! Slot for scale changing. (The string must be smth like this: "123%")
	void setScale(const QString& sc);

signals:
	//! Emitted, when sceneRect, visualized by this view, was changed.
	void sceneRectChanged(const QRectF& newRect);
    //! Emitted, when scale factor is changed.
    void scaleChanged(qreal newScaleFactor);

protected:
    //! Колесико
  //  void wheelEvent(QWheelEvent *event);

    bool isSceneRectControlled;

private slots:
	void updateSceneRect(const QRectF& rect);

};
#endif /* GESVIEW_H_ */
