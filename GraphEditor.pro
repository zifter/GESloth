QT_VERSION = Qt \
    System \
    (4.7.2)
QT += core \
    gui \
    xml
CONFIG(debug, debug|release) { 
    # Set destination path, where we put our executable file
    DESTDIR = build/debug/bin/
    
    # Set folder for object file
    OBJECTS_DIR = build/debug/obj/
    
    # Set folder for msoc_*.cpp file
    MOC_DIR = build/debug/tmp/moc/
    DEFINES = DEBUG
    message("Creating Debug Makefile.")
}
else { 
    # Set destination path, where we put our executable file
    DESTDIR = 
    
    # Set folder for object file
    OBJECTS_DIR = build/release/obj/
    
    # Set folder for moc_*.cpp file
    MOC_DIR = build/release/tmp/moc/
    DEFINES = RELEASE
    message("Creating Release Makefile.")
}

# Set include path
INCLUDEPATH = include/

# Set source path
SUBDIRS = src/

# Set folder for media and resource
RCC_DIR = media/

# Set header
HEADERS += include/PageSettings.h \
    include/GESFileWriter.h \
    include/GESTag.h \
    include/Graph.h \
    include/GESFileLoader.h \
    include/GESTabWidget.h \
    include/GESPage.h \
    include/Macros.h \
    include/GESView.h \
    include/Object.h \
    include/Node.h \
    include/HelpBrowser.h \
    include/GESloth.h \
    include/Command.h \
    include/GESScene.h \
    include/Edge.h

# Set source
SOURCES += src/PageSettings.cpp \
    src/GESTag.cpp \
    include/GESFileWriter.cpp \
    src/Graph.cpp \
    src/GESFileLoader.cpp \
    src/GESTabWidget.cpp \
    src/GESPage.cpp \
    src/GESView.cpp \
    src/Object.cpp \
    src/Node.cpp \
    src/HelpBrowser.cpp \
    src/GESloth.cpp \
    src/Command.cpp \
    src/GESScene.cpp \
    src/Edge.cpp \
    src/main.cpp

# Set translations
TRANSLATIONS += media/translations/graphicsRedactor_ru.ts \
    media/translations/graphicsRedactor_en.ts

# Resource
RESOURCES += media/GESResource.qrc
TEMPLATE = app
TARGET = GESloth
