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
