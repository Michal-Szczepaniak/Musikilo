/*
    Copyright (C) 2019 Michał Szczepaniak

    This file is part of Musikilo.

    Musikilo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Musikilo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Musikilo. If not, see <http://www.gnu.org/licenses/>.
*/

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
                property var pluginCodes: ["nextcloud", "squeezebox", "mpd"]
                menu: ContextMenu {
                    MenuItem { text: qsTr("Nextcloud") }
                    MenuItem { text: qsTr("SqueezeBox") }
                    MenuItem { text: qsTr("MPD") }
                }
                on_MenuOpenChanged: if (!_menuOpen) {
                                        settingsLoader.active = false
                                        switch(pluginCodes[currentIndex]) {
                                            case "nextcloud":
                                                settingsLoader.sourceComponent = nextcloudSettingsDialog
                                                break;
                                            case "squeezebox":
                                                settingsLoader.sourceComponent = squeezeBoxSettingsDialog
                                                break;
                                            case "mpd":
                                                settingsLoader.sourceComponent = mpdSettingsDialog
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

    Component {
        id: mpdSettingsDialog

        MPDSettingsDialog {

        }
    }

    Component {
        id: squeezeBoxSettingsDialog

        SqueezeBoxSettingsDialog {

        }
    }
}
