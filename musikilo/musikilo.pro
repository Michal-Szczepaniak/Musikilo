TARGET = musikilo

QT += core network xml multimedia dbus

CONFIG += link_pkgconfig sailfishapp

PKGCONFIG += qofonoext

SOURCES += src/Musikilo.cpp \
    src/playlistmodel.cpp \
    src/simplecrypt.cpp \
    src/webdavmodel.cpp

DISTFILES += qml/Musikilo.qml \
    musikilo.desktop \
    qml/components/TabHeader.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FilesList.qml \
    qml/pages/Main.qml \
    qml/pages/SongDetails.qml \
    qml/pages/Settings.qml \
    qml/pages/SongsList.qml \
    qml/pages/About.qml \
    translations/*.ts

RESOURCES += \
    qml/resources/resources.qrc

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

TRANSLATIONS += translations/musikilo-de.ts \
    translations/musikilo-zh_CN.ts

# qwebdavlib
DEFINES += QWEBDAVITEM_EXTENDED_PROPERTIES
PRE_TARGETDEPS += $$OUT_PWD/../qwebdavlib/qwebdavlib/libqwebdav.a
INCLUDEPATH += ../qwebdavlib/qwebdavlib/
LIBS += -L$$OUT_PWD/../qwebdavlib/qwebdavlib/ -lqwebdav

HEADERS += \
    src/playlistmodel.h \
    src/simplecrypt.h \
    src/webdavmodel.h
