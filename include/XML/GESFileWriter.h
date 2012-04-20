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
 * GESFileWriter.h
 *
 *  Created on: Apr 17, 2012
 *      Author: zifter
 */

#ifndef GESFILEWRITER_H_
#define GESFILEWRITER_H_

#include <QByteArray>
#include <QXmlStreamWriter>
#include "Gui/GESPage.h"
#include "Gui/PageSettings.h"

class GESFileWriter {
public:
	GESFileWriter();
	~GESFileWriter();

	void write( GESPage* page );
	QByteArray& writeGraphToByte(Graph* graph);

private:
	void writeSettings( PageSettings* set );
	void writeGraph( Graph* graph );

	void writePointF( QPointF p );
	QXmlStreamWriter mStream;
	QByteArray mByte;
};

#endif /* GESFILEWRITER_H_ */
