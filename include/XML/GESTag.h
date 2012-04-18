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
 * GESTag.h
 *
 *  Created on: Apr 16, 2012
 *      Author: zifter
 */

#ifndef GESTAG_H_
#define GESTAG_H_

#include <QString>

namespace tag {
	const QString XML_POINTF("pointf");

	const QString XML_GRAPH("graph");
		const QString XML_EDGE("edge");
		const QString XML_NODE("node");

	const QString XML_SETTINGS("settings");
		const QString XML_SCENE_RECT("sc_rect");
			const QString XML_SIZE("size");
			const QString XML_CENTER("center");
}

namespace attr {
	const QString XML_TEXT("text");
	const QString XML_ID("id");
	const QString XML_ID_SOURCE("idsource");
	const QString XML_ID_DEST("iddest");
	const QString XML_X("x");
	const QString XML_Y("y");
}

#endif /* GESTAG_H_ */
