#ifndef GRAPHREDACTORSCENE_H
#define GRAPHREDACTORSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QGraphicsView>

class GraphWidget;
class Node;
class Edge;
class QMenu;
class QContextMenuEvent;
class QWidget;
class QLineEdit;
class QByteArray;
class QDomElement;
class QUndoStack;
class QWheelEvent;
class QComboBox;

//! Просмоторщик сцены
class GraphRedactorView : public QGraphicsView
{
    Q_OBJECT

public:
    //! Пустой конструктоh
   GraphRedactorView(){}

    //! Комбо бокс для изменения масштаба
   QComboBox* sceneScaleCombo;

protected:
    //! Колесико
    void wheelEvent(QWheelEvent *event);

};

class GraphRedactorScene : public QGraphicsScene
{
    Q_OBJECT

public:
   GraphRedactorScene(GraphWidget*);

    //! Возможные состояния системы
    enum State { InsertNode, InsertEdge };

    //! Добавить вершину
    void addNode(Node* nd)
    {        nodes << nd;
    }

    //! Добавить ребро
    void addEdge(Edge* dg)
    {        edges << dg;
    }

    //! Получить лист вершин
    QList<Node*> getNodes()
    {       return nodes;
    }

    //! Получить лист ребер
    QList<Edge*> getEdges()
    {       return edges;
    }

    //! Добавить объекты списка
    void addItems(QList<QGraphicsItem*> list);

public slots:
    //! Установка состояния
    void setState(State state)
    {        myState = state;
    }

    //! Получить состояние
    State getState()
    {        return myState;
    }

    //! Удалить
    void deleteSelectedObj();

    //! Удалить
    void deleteUnderMouseObj();

    //! Удалить объекты списка
    void deleteObj(QList<QGraphicsItem*> itemList);

    //! Удалить объекты текущего списка
    void deleteObj();

    //! Установить имя
    void setName();

    //! Копировать элементы
    void copyObj();

    //! Выделить все
    void selectAll();

    //! Вставить
    void pasteObj();

    //! Сохраняет лист объектов в массив байтов в XML-формате
    void saveToByte( QList< QGraphicsItem* >& itemList, QByteArray& );

    //! Загружает из массива байтов в лист объекты
    bool openFromByte( QList< QGraphicsItem* >& , QByteArray& );

    //! Команда назад
    void undoCommand();

    //! Команда вперед
    void redoCommand();

protected:
    //! Событие нажатия кнопки
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

    //! Событие двежение зажатой мышки
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

    //! Событие отпускание кнопки
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    //! Событие двойного щелчка
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent );

    //! Событие вызова контекстного меню
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    //! Состояние
    State myState;

    //! Родитель
    GraphWidget *Parent;

    //! Линия
    QGraphicsLineItem *line;

    //! Лист вершин
    QList< Node* > nodes;

    //! Лист ребер
    QList< Edge* > edges;

    //! Контрольная точка
    QPointF *point;

    //! Меню
    QMenu *menu;

    //! Текущий элемент
    QGraphicsItem* currentItem;

    //! Текущие объекты
    QList<QGraphicsItem*> currentList;

    //! Разбор вершины
    bool parseNode(QDomElement& node, QMap<int, Node*>& list);

    //! Разбор ребра
    bool parseEdge(QDomElement& edge, QMap<int, Node*>&  list, QList< Edge* >& edges );

    //! Стек команд
    QUndoStack* stackCommand;

};
































#endif // GRAPHREDACTORSCENE_H
