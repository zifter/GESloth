#include "Command.h"
#include <QGraphicsItem>
#include "Edge.h"
#include "Node.h"

void addItemCommand::undo()
{
	mGraph->remove(change);
}

void addItemCommand::redo() {
	mGraph->add(change);
}

void delItemCommand::redo()
{
	mGraph->remove(change);
}

void delItemCommand::undo() {
	mGraph->add(change);
}
