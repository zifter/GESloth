/*
 * GrapgSettings.h
 *
 *  Created on: Apr 17, 2012
 *      Author: zifter
 */

#ifndef PAGESETTINGS_H_
#define PAGESETTINGS_H_

class PageSettings {
public:
	PageSettings();
	~PageSettings();
	enum DrawState{
		Node, Edge
	};

	void setState(DrawState st ){
		mState = st;
	}
	DrawState getState() const{
		return mState;
	}

	void setZoom( int _z ){
		mZoom = _z;
	}
	int getZoom(){
		return mZoom;
	}

	DrawState mState;
	int mZoom;
};

#endif /* PAGESETTINGS_H_ */
