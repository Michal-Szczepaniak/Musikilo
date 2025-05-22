import QtQuick 2.0
import Sailfish.Silica 1.0
import "../dialogs"

Dialog {
    id: dialog

    property var pluginData
    property var pluginCode

    onAccepted: {
        var settings = settingsLoader.item.collectSettings();
        settings.name = pluginData.name;

        settingsManager.setPluginSettings(pluginCode, settings)
    }

    canAccept: settingsLoader.active

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: parent.width

            DialogHeader { }

            Loader {
                id: settingsLoader
                anchors {
                    left: parent.left
                    right: parent.right
                }

                active: true
                asynchronous: true
                sourceComponent:
                    switch(pluginData.plugin) {
                        case "nextcloud":
                            return nextcloudSettingsDialog;
                        case "spotify":
                            break;
                        case "squeezebox":
                            break;
                        case "mpd":
                            break;
                    }
                onLoaded: item.setData(pluginData)
            }
        }
    }

    Component {
        id: nextcloudSettingsDialog

        NextcloudSettingsDialog {

        }
    }
}
