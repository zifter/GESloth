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
SUBDIRS = src/ \
    src/Graph \
    src/Gui

# Set folder for media and resource
RCC_DIR = media/

# Set header
HEADERS += include/Gui/Command.h \
    include/Gui/PageSettings.h \
    include/XML/GESFileLoader.h \
    include/XML/GESFileWriter.h \
    include/XML/GESTag.h \
    include/Gui/GESPage.h \
    include/Gui/GESScene.h \
    include/Gui/GESTabWidget.h \
    include/Gui/GESView.h \
    include/Gui/GESloth.h \
    include/Macros.h \
    include/HelpBrowser.h \
    include/Graph/Edge.h \
    include/Graph/Node.h \
    include/Graph/Graph.h \
    include/Graph/Object.h

# Set source
SOURCES += src/Gui/Command.cpp \
    src/Gui/PageSettings.cpp \
    src/XML/GESFileLoader.cpp \
    src/XML/GESFileWriter.cpp \
    src/Gui/GESPage.cpp \
    src/Gui/GESScene.cpp \
    src/Gui/GESTabWidget.cpp \
    src/Gui/GESView.cpp \
    src/Gui/GESloth.cpp \
    src/HelpBrowser.cpp \
    src/Graph/Edge.cpp \
    src/Graph/Node.cpp \
    src/Graph/Graph.cpp \
    src/Graph/Object.cpp \
    src/main.cpp

# Set translations
TRANSLATIONS += media/translations/graphicsRedactor_ru.ts \
    media/translations/graphicsRedactor_en.ts

# Resource
RESOURCES += media/GESResource.qrc
TEMPLATE = app
TARGET = GESloth
