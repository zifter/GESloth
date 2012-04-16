/*
 * GESFileWriter.h
 *
 *  Created on: Apr 17, 2012
 *      Author: zifter
 */

#ifndef GESFILEWRITER_H_
#define GESFILEWRITER_H_

#include <QByteArray>
#include "Graph.h"

class GESFileWriter {
public:
	GESFileWriter();
	~GESFileWriter();

	void write( Graph* graph, QString name );
	QByteArray& writeToByte( Graph* graph );
};

#endif /* GESFILEWRITER_H_ */
