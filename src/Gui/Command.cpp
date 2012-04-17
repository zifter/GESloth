#include "Gui/Command.h"

#include "Graph/Edge.h"
#include "Graph/Node.h"

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
