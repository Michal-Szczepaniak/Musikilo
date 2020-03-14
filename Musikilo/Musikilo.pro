TARGET = harbour-musikilo

QT += core network xml multimedia dbus

INCLUDEPATH += ../qtmpris/src

LIBS += -L../qtmpris/src -lmpris-qt5

CONFIG += sailfishapp

SOURCES += src/Musikilo.cpp \
    src/playlistmodel.cpp \
    src/simplecrypt.cpp \
    src/webdavmodel.cpp

DISTFILES += qml/Musikilo.qml \
    qml/components/TabHeader.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FilesList.qml \
    qml/pages/Main.qml \
    qml/pages/SongDetails.qml \
    qml/pages/Settings.qml \
    qml/pages/SongsList.qml \
    qml/pages/About.qml \
    translations/*.ts \
    harbour-musikilo.desktop

RESOURCES += \
    qml/resources/resources.qrc

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

TRANSLATIONS += translations/harbour-musikilo-de.ts \
    translations/harbour-musikilo-zh_CN.ts

# libqofonoext
PRE_TARGETDEPS += $$OUT_PWD/../libqofonoext/src/libqofonoext.a
INCLUDEPATH += ../libqofonoext/src/
LIBS += -L$$OUT_PWD/../libqofonoext/src/ -lqofonoext

# qwebdavlib
DEFINES += QWEBDAVITEM_EXTENDED_PROPERTIES
PRE_TARGETDEPS += $$OUT_PWD/../qwebdavlib/qwebdavlib/libqwebdav.a
INCLUDEPATH += ../qwebdavlib/qwebdavlib/
LIBS += -L$$OUT_PWD/../qwebdavlib/qwebdavlib/ -lqwebdav

HEADERS += \
    src/playlistmodel.h \
    src/simplecrypt.h \
    src/webdavmodel.h
