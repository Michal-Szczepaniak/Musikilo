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
import Nemo.Configuration 1.0
import org.nemomobile.ofono 1.0

Item {
    id: settingsPage

    implicitHeight: swipeView.height; implicitWidth: swipeView.width

    OfonoModemManager { id: modemManager }

    Connections {
        target: modemManager
        onReadyChanged: {
            var key = modemManager.imeiCodes.join("");
            simpleCrypt.key = key;
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick

        PageHeader {
            id: header
            title: qsTr("Settings")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Add new plugin")
                onClicked: pageStack.push(newPluginDialog)
            }
        }

        SilicaListView {
            id: pluginList
            model: settingsManager.plugins
            anchors.top: header.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            delegate: ListItem {
                id: listItem
                width: pluginList.width
                property var data: settingsManager.getPluginSettings(modelData)

                menu: contextMenuComponent

                Component {
                    id: contextMenuComponent

                    ContextMenu {
                        MenuItem {
                            text: "Edit"
                            onClicked: pageStack.push(editPluginDialog, {pluginCode: modelData, pluginData: settingsManager.getPluginSettings(modelData)})
                        }

                        MenuItem {
                            text: "Test"
                            onClicked: settingsManager.testPlugin(modelData)
                        }

                        MenuItem {
                            text: "Remove"
                            onClicked: remorseAction("Deleting", function() { settingsManager.removePlugin(modelData) })
                        }
                    }
                }

                Row {
                    width: parent.width
                    height: parent.height

                    Icon {
                        width: Theme.iconSizeExtraSmall
                        height: Theme.iconSizeExtraSmall
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/icons/" + listItem.data.plugin + ".png"
                    }

                    DetailItem {
                        label: listItem.data.name
                        anchors.verticalCenter: parent.verticalCenter
                        value: getPrettyName(listItem.data.plugin)
                        palette.secondaryHighlightColor: settingsManager.currentPlugin === modelData ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        palette.highlightColor: settingsManager.currentPlugin === modelData ? Theme.highlightColor : Theme.primaryColor

                        function getPrettyName(name) {
                            switch (name) {
                            case "nextcloud":
                                return qsTr("Nextcloud");
                            case "spotify":
                                return qsTr("Spotify");
                            case "squeezebox":
                                return qsTr("SqueezeBox");
                            case "mpd":
                                return qsTr("MPD");
                            }
                        }
                    }
                }

                onClicked: settingsManager.currentPlugin = modelData
            }

            VerticalScrollDecorator {}
        }
    }

    Component {
        id: newPluginDialog

        NewPluginDialog {}
    }

    Component {
        id: editPluginDialog

        EditPluginDialog {}
    }
}
