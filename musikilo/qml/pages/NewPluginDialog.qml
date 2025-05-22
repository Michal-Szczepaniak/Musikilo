import QtQuick 2.0
import Sailfish.Silica 1.0
import "../dialogs"

Dialog {
    id: dialog

    onAccepted: {
        var settings = settingsLoader.item.collectSettings();
        settings.name = pluginName.text;

        settingsManager.setPluginSettings(codify(pluginName.text), settings)
    }

    canAccept: settingsLoader.active && pluginName.acceptableInput && !settingsManager.hasPlugin(codify(pluginName.text))

    function codify(value) {
        value = value.toLowerCase();
        value = value.replace(" ", "-");
        return value;
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: parent.width

            DialogHeader { }

            ComboBox {
                id: pluginTypeBox
                anchors.horizontalCenter: parent.horizontalCenter
                property var pluginCodes: ["nextcloud", "spotify", "squeezebox", "mpd"]
                menu: ContextMenu {
                    MenuItem { text: qsTr("Nextcloud") }
                    MenuItem { text: qsTr("Spotify") }
                    MenuItem { text: qsTr("SqueezeBox") }
                    MenuItem { text: qsTr("MPD") }
                }
                on_MenuOpenChanged: if (!_menuOpen) {
                                        settingsLoader.active = false
                                        switch(pluginCodes[currentIndex]) {
                                            case "nextcloud":
                                                settingsLoader.sourceComponent = nextcloudSettingsDialog
                                                break;
                                            case "spotify":
                                                break;
                                            case "squeezebox":
                                                break;
                                            case "mpd":
                                                break;
                                        }
                                        settingsLoader.active = true
                                    }
            }

            TextField {
               id: pluginName
               label: qsTr("Name")
               labelVisible: true
               placeholderText: label
               width: parent.width
               description: {
                   if (settingsManager.hasPlugin(codify(text))) {
                       return "Code already in use: " + codify(text);
                   } else {
                       return codify(text);
                   }
               }
               validator: RegExpValidator { regExp: /[0-9A-Za-z ]+/ }
            }

            Loader {
                id: settingsLoader
                anchors {
                    left: parent.left
                    right: parent.right
                }

                active: false
                asynchronous: true
            }
        }
    }

    Component {
        id: nextcloudSettingsDialog

        NextcloudSettingsDialog {

        }
    }
}
