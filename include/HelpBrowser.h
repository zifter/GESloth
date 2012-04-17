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
