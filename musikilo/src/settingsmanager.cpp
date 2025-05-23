#include "settingsmanager.h"
#include <QDebug>
#include <src/plugins/mpd/mpdplugin.h>
#include <src/plugins/webdav/webdavplugin.h>

SettingsManager::SettingsManager(QObject *parent) : QObject(parent)
{
    _pluginList = _settings.value("plugins").toStringList();

    createPlugins();
}

SettingsManager::~SettingsManager()
{
    for (PluginInterface* plugin : _plugins) {
        delete plugin;
    }

    _plugins.clear();
}

QStringList SettingsManager::getPlugins() const
{
    return _pluginList;
}

QVariantMap SettingsManager::getPluginSettings(QString pluginCode)
{
    return _settings.value("plugin-" + pluginCode).toMap();
}

void SettingsManager::setPluginSettings(QString pluginCode, QVariantMap settings)
{
    _settings.setValue("plugin-" + pluginCode, settings);

    bool newPlugin = false;
    if (!_pluginList.contains(pluginCode)) {
        _pluginList.append(pluginCode);
        newPlugin = true;
    }

    _settings.setValue("plugins", _pluginList);

    if (newPlugin) {
        createPlugin(pluginCode);
    } else {
        getPlugin(pluginCode)->updateConig(settings);
    }

    if (pluginCode == _currentPlugin) {
        emit currentPluginChanged();
    }

    emit pluginsChanged();
}

bool SettingsManager::hasPlugin(QString pluginCode)
{
    return _settings.contains("plugin-" + pluginCode);
}

void SettingsManager::testPlugin(QString pluginCode)
{
    auto plugin = getPlugin(pluginCode);
    if (plugin == nullptr) return;

    plugin->testConfig();
}

void SettingsManager::removePlugin(QString pluginCode)
{
    _pluginList.removeAll(pluginCode);
    _settings.remove("plugin-" + pluginCode);
    delete _plugins[pluginCode];
    _plugins.remove(pluginCode);

    if (_currentPlugin == pluginCode) {
        _currentPlugin = nullptr;
        emit currentPluginChanged();
    }
}

QString SettingsManager::getCurrentPluginName()
{
    return _currentPlugin;
}

QString SettingsManager::getCurrentPluginType()
{
    if (!hasPlugin(_currentPlugin)) return QString();

    return getPluginSettings(_currentPlugin)["plugin"].toString();
}

void SettingsManager::setCurrentPlugin(QString pluginCode)
{
    if (!_plugins.contains(pluginCode)) return;
    if (_currentPlugin != "") {
        getPlugin(_currentPlugin)->deactivate();
    }

    _currentPlugin = pluginCode;

    getPlugin(_currentPlugin)->activate();

    emit currentPluginChanged();
}

PluginInterface *SettingsManager::getCurrentPlugin()
{
    return getPlugin(_currentPlugin);
}

void SettingsManager::createPlugins()
{
    for (QString pluginCode : _pluginList) {
        createPlugin(pluginCode);
    }

    emit currentPluginChanged();
}

void SettingsManager::createPlugin(QString code)
{
    QVariantMap settings = _settings.value("plugin-" + code).toMap();
    if (settings.empty()) {
        qWarning() << "Empty pluin settings for plugin: " << code;
        return;
    }

    auto type = settings.value("plugin").toString();
    PluginInterface* plugin = nullptr;
    if (type == "nextcloud") {
        plugin = new WebDavPlugin;
    } else if (type == "spotify") {

    } else if (type == "mpd") {
        plugin = new MPDPlugin;
    }

    if (plugin == nullptr) {
        return;
    }

    connect(plugin, &PluginInterface::testSucceeded, this, &SettingsManager::testSucceeded);
    connect(plugin, &PluginInterface::testFailed, this, &SettingsManager::testFailed);
    connect(plugin, &PluginInterface::pluginReady, this, &SettingsManager::pluginReady);

    plugin->initialize(settings);

    _plugins[code] = plugin;
}

PluginInterface *SettingsManager::getPlugin(QString code)
{
    if (!_plugins.contains(code)) return nullptr;

    return _plugins[code];
}
