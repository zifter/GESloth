#ifndef HELPBROWSER_H
#define HELPBROWSER_H
#include <QWidget>

class QPushButton;
class QTextBrowser;
class QString;

//! Виджет контекстной помощи
class HelpBrowser : public QWidget
{
    Q_OBJECT

public:

    //! Конструктор
    HelpBrowser(const QString &path, const QString &page);

    //! Метод, с помощью которого создается виджет
    static void showPage(const QString &page);

public slots:

    //! Установки имени
    void updateWindowTitle();

private:

    //! Просмоторщик текста
    QTextBrowser *textBrowser;

    //! Кнопка вернуться на главную страницу
    QPushButton *homeButton;

    //! Кнопка назад
    QPushButton *backButton;

    //! Кнопка закрытия
    QPushButton *closeButton;
};

#endif // HELPBROWSER_H
