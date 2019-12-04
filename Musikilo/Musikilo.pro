# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = Musikilo

QT += core network xml multimedia

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
    rpm/Musikilo.spec \
    translations/*.ts \
    Musikilo.desktop

RESOURCES += \
    qml/resources/resources.qrc

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

TRANSLATIONS += translations/Musikilo-de.ts \
    translations/Musikilo-zh_CN.ts

DEFINES += QWEBDAVITEM_EXTENDED_PROPERTIES
PRE_TARGETDEPS += $$OUT_PWD/../qwebdavlib/qwebdavlib/libqwebdav.a
INCLUDEPATH += $$OUT_PWD/../qwebdavlib/qwebdavlib/
LIBS += -L$$OUT_PWD/../qwebdavlib/qwebdavlib/ -lqwebdav

HEADERS += \
    src/playlistmodel.h \
    src/simplecrypt.h \
    src/webdavmodel.h
