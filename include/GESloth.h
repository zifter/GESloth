#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QMainWindow>

class Node;
class Edge;
class GraphRedactorScene;
class Object;
class QButtonGroup;
class QToolBar;
class QMenuBar;
class QDomElement;
class QGraphicsItem;
class QComboBox;
class QApplication;
class QTranslator;
class GraphRedactorView;

class GraphWidget : public QMainWindow
{
    Q_OBJECT

public:
    GraphWidget(QApplication*);

    //! Объекты нуждаються в передвижении
    void itemMoved();

    //! Удалить объект
    void delItem(Object* obj);

    //! Создание менюшек
    void createMenus();

    //! Сохранить в файл
    void saveToFile();

    //! Загрузить из файла
    void openFromFile();

protected:
    //! Событие нажатия каких либо клавиш
    void keyPressEvent(QKeyEvent *event);

    //! Событие таймера
    void timerEvent(QTimerEvent *event);

    //! Фактор увелечения
    void scaleView(qreal scaleFactor);

    //! Создание панели инструментов
    void createToolBar();

public slots:

    //! Слот для изменения состояния
    void pointerGroupClicked( int );

    //! Экспортировка в изображение
    void exportToImage();

    //! Об этой лабе
    void about();

    //! О Кьют
    void aboutQt();

    //! Изменение увеличения сцены
    void sceneScaleChanged(const QString &scale);

    //! Отключить псевдо физику
    void switchOff(bool);

    //! Открыть
    void Open();

    //! Сохранить
    void Save();

    //! Сохранить как
    void SaveAs();

    //! Изменить язык
    void switchLang(QAction*);

    //! Вызов контекстной помощи
    void help();

private:

    //! Индификатор таймера
    int timerId;

    //! Для установки масштаба
    QComboBox* sceneScaleCombo;

    //! Сцена
    GraphRedactorScene* scene;

    //! Просмоторщик
    GraphRedactorView* view;

    //! Группа кнопок
    QButtonGroup *pointerTypeGroup;

    //! Панель инструментов
    QToolBar *pointerToolbar;

    //! Имя файла
    QString NameOfFile;
};

#endif
