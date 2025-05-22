TARGET = musikilo

QT += core network xml multimedia dbus

CONFIG += link_pkgconfig sailfishapp

PKGCONFIG += qofonoext

SOURCES += \
    src/filemodel.cpp \
    src/filemodelinterface.cpp \
    src/musikilo.cpp \
    src/player.cpp \
    src/playerinterface.cpp \
    src/playlistmodel.cpp \
    src/playlistmodelinterface.cpp \
    src/plugins/webdav/webdavfilemodel.cpp \
    src/plugins/webdav/webdavplayer.cpp \
    src/plugins/webdav/webdavplaylistmodel.cpp \
    src/plugins/webdav/webdavplugin.cpp \
    src/settingsmanager.cpp \
    src/simplecrypt.cpp

HEADERS += \
    src/filemodel.h \
    src/filemodelinterface.h \
    src/player.h \
    src/playerinterface.h \
    src/playlistmodel.h \
    src/playlistmodelinterface.h \
    src/plugininterface.h \
    src/plugins/webdav/webdavfilemodel.h \
    src/plugins/webdav/webdavplayer.h \
    src/plugins/webdav/webdavplaylistmodel.h \
    src/plugins/webdav/webdavplugin.h \
    src/settingsmanager.h \
    src/simplecrypt.h

DISTFILES += \
    qml/Musikilo.qml \
    musikilo.desktop \
    qml/components/TabHeader.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FilesList.qml \
    qml/pages/Main.qml \
    qml/pages/SongDetails.qml \
    qml/pages/Settings.qml \
    qml/pages/SongsList.qml \
    qml/pages/About.qml \
    qml/pages/NewPluginDialog.qml \
    qml/pages/EditPluginDialog.qml \
    qml/dialogs/NextcloudSettingsDialog.qml \
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
