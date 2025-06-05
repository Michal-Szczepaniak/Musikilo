TARGET = musikilo

QT += core network xml multimedia dbus

CONFIG += link_pkgconfig sailfishapp

SOURCES += \
    src/filemodel.cpp \
    src/filemodelinterface.cpp \
    src/musikilo.cpp \
    src/player.cpp \
    src/playerinterface.cpp \
    src/playlistmodel.cpp \
    src/playlistmodelinterface.cpp \
    src/plugins/mpd/mpdfilemodel.cpp \
    src/plugins/mpd/mpdplayer.cpp \
    src/plugins/mpd/mpdplaylistmodel.cpp \
    src/plugins/mpd/mpdplugin.cpp \
    src/plugins/squeezebox/squeezeboxfilemodel.cpp \
    src/plugins/squeezebox/squeezeboxmanager.cpp \
    src/plugins/squeezebox/squeezeboxplayer.cpp \
    src/plugins/squeezebox/squeezeboxplaylistmodel.cpp \
    src/plugins/squeezebox/squeezeboxplugin.cpp \
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
    src/plugins/mpd/mpdfilemodel.h \
    src/plugins/mpd/mpdplayer.h \
    src/plugins/mpd/mpdplaylistmodel.h \
    src/plugins/mpd/mpdplugin.h \
    src/plugins/squeezebox/squeezeboxfilemodel.h \
    src/plugins/squeezebox/squeezeboxmanager.h \
    src/plugins/squeezebox/squeezeboxplayer.h \
    src/plugins/squeezebox/squeezeboxplaylistmodel.h \
    src/plugins/squeezebox/squeezeboxplugin.h \
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
    qml/pages/plugins/MPDControls.qml \
    qml/pages/plugins/SqueezeBoxControls.qml \
    qml/dialogs/NextcloudSettingsDialog.qml \
    qml/dialogs/MPDSettingsDialog.qml \
    qml/dialogs/SqueezeBoxSettingsDialog.qml \
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

# smpc
PRE_TARGETDEPS += $$OUT_PWD/../smpc/libsmpc.a
INCLUDEPATH += ../smpc/src
LIBS += -L$$OUT_PWD/../smpc/ -lsmpc
