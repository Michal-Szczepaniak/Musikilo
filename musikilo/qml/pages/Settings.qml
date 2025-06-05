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

        Label {
            anchors.centerIn: parent
            visible: settingsManager.plugins.length === 0
            text: qsTr("Add plugin from pull-down menu")
            color: Theme.secondaryHighlightColor
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
                    spacing: Theme.paddingMedium

                    Rectangle {
                        color: "transparent"
                        height: 1
                        width: Theme.paddingMedium
                    }

                    Icon {
                        width: Theme.iconSizeMedium
                        height: Theme.iconSizeMedium
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:///images/icon-m-" + listItem.data.plugin + ".svg"
                        color: settingsManager.currentPlugin === modelData ? Theme.highlightColor : Theme.primaryColor
                    }

                    Text {
                        id: valueText
                        color: settingsManager.currentPlugin === modelData ? Theme.highlightColor : Theme.primaryColor
                        font.pixelSize: Theme.fontSizeSmall
                        textFormat: Text.PlainText
                        wrapMode: Text.Wrap
                        anchors.verticalCenter: parent.verticalCenter
                        text: listItem.data.name
                    }

                    Text {
                        id: labelText
                        color: settingsManager.currentPlugin === modelData ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeSmall
                        textFormat: Text.PlainText
                        wrapMode: Text.Wrap
                        anchors.verticalCenter: parent.verticalCenter
                        text: getPrettyName(listItem.data.plugin)

                        function getPrettyName(name) {
                            switch (name) {
                            case "nextcloud":
                                return qsTr("Nextcloud");
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
