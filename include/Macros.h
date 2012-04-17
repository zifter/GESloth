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

#ifndef MACROS_H
#define MACROS_H

#include <QDebug>

#ifndef DEBUG
    #define IS_DEBUG false
    #define PRINT(x)
#else
    #define IS_DEBUG true
    #define PRINT(x) 	qDebug() << "\n"<< x; \
                        qDebug() << "     line:" << __LINE__ << ", function:" << __FUNCTION__ << ", file:" << __FILE__;
#endif

#define DELETE(x) delete x;\
                  x = NULL;
// пытаемся объявить массив и из -1 при ошибке или из 1, если все хорошо
#define STATIC_ASSERT(COND,MSG) typedef char static_assertion_##MSG[(!!(COND))*2-1];

#endif // MACROS_H
