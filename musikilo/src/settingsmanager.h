#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "plugininterface.h"

#include <QObject>
#include <QSettings>

class SettingsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList plugins READ getPlugins NOTIFY pluginsChanged)
    Q_PROPERTY(QString currentPlugin READ getCurrentPluginName WRITE setCurrentPlugin NOTIFY currentPluginChanged)
public:
    explicit SettingsManager(QObject *parent = nullptr);
    ~SettingsManager();

    QStringList getPlugins() const;

    Q_INVOKABLE QVariantMap getPluginSettings(QString pluginCode);
    Q_INVOKABLE void setPluginSettings(QString pluginCode, QVariantMap settings);
    Q_INVOKABLE bool hasPlugin(QString pluginCode);
    Q_INVOKABLE void testPlugin(QString pluginCode);
    Q_INVOKABLE void removePlugin(QString pluginCode);
    QString getCurrentPluginName();
    void setCurrentPlugin(QString pluginCode);

    PluginInterface* getCurrentPlugin();

signals:
    void pluginsChanged();
    void currentPluginChanged();
    void testSucceeded();
    void testFailed(QString message);

private:
    void createPlugins();
    void createPlugin(QString code);
    PluginInterface* getPlugin(QString code);

private:
    QSettings _settings;
    QStringList _pluginList;
    QMap<QString, PluginInterface*> _plugins;
    QString _currentPlugin = "";
};

#endif // SETTINGSMANAGER_H
