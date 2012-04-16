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
