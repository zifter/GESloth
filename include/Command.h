#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>
#include "GESScene.h"
#include "Object.h"
#include "Node.h"

class QGraphicsItem;

//! Команда добавления объектов
class addItemCommand : public QUndoCommand
{
public:

    //! Конструктор команды
    addItemCommand( QList<QGraphicsItem*> lst,  GESScene* scn) : list(lst), scene(scn)
    {}

    //! Команда вперед
    virtual void undo()
    {
        ///scene->deleteObj(list);
        //foreach (Node *node, scene->getNodes())
        //    node->calculateForces();
    }
    //! Команда вперед
    virtual void redo();

private:

    //! Список, который добавляется
    QList<QGraphicsItem*> list;

    //! Сцена, куда добавляеться
    GESScene* scene;

};

//! Команда удаления объектов
class delItemCommand : public QUndoCommand
{
public:

    //! Конструктор команды
    delItemCommand( QList<QGraphicsItem*> lst,  GESScene* scn) : list(lst), scene(scn)
    {}

    //! Команда назад
    virtual void undo();

    //! Команда вперед
    virtual void redo()
    {
        //scene->deleteObj(list);
        //foreach (Node *node, scene->getNodes())
       //     node->calculateForces();
    }

private:

    //! Список удаляемых объектов
    QList<QGraphicsItem*> list;

    //! Сцена, откуда удаляются
    GESScene* scene;
};

//! Команда установки текста
class setTextCommand : public QUndoCommand
{
public:

    //! Коструктор команды
    setTextCommand( Object* obj, QString text) : SetableText(text), PastText(obj->getText())
    {
        object = obj;
    }

    //! Команда назад
    virtual void undo()
    {
        object->setText(PastText);
        object->update();
    }

    //! Команда вперед
    virtual void redo()
    {
        object->setText(SetableText);
        object->update();
    }

private:
    //! Объект которому устанавливаеться текст
    Object* object;

    //! Устанавливаемый текст
    QString SetableText;

    //! Предыдущий текст
    QString PastText;
};


#endif // COMMAND_H
