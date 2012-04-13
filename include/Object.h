#ifndef OBJECT_H
#define OBJECT_H

#include <QGraphicsItem>

#ifndef NULL
#define NULL   ((void *) 0)
#endif

class Object : public QGraphicsItem
{
public:
    Object();

    //! Виртуальная функция удаления
    virtual void del(){};

    //! Установить объект
    void setText(QString _txt)
    {
        if(_txt.length()>15)
            text = _txt.replace(15, _txt.length()-15, QString("~"));
        else
            text = _txt;
    };

    //! Получить текст объекта
    QString getText() const
    {
        return text;
    };

private:
    //! Текст
    QString text;
};

#endif // OBJECT_H
