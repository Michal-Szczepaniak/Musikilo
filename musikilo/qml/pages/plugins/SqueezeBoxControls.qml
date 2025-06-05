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

Item {
    id: mpdControlsPage

    implicitHeight: swipeView.height; implicitWidth: swipeView.width

    SilicaFlickable {
        id: filesFlickable
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick

        PageHeader {
            id: header
            title: qsTr("SqueezeBox controls")
        }

        Column {
            anchors.top: header.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            spacing: Theme.paddingMedium

            ComboBox {
                width: parent.width
                label: "Client"
                Binding on currentIndex {
                    value: player.pluginControls.active_player
                }

                menu: ContextMenu {
                    Repeater {
                        model: player.pluginControls.player_names
                        delegate: MenuItem {
                            text: modelData
                        }
                    }
                }

                onCurrentIndexChanged: {
                    if (currentIndex >= 0 && currentIndex < player.pluginControls.player_names.length) {
                        var controls = player.pluginControls
                        controls.active_player = currentIndex
                        player.pluginControls = controls
                    }
                }
            }

            Slider {
                id: volumeSlider
                width: parent.width
                Binding on value {
                    when: !volumeSlider.down
                    value: player.pluginControls.volume
                }
                stepSize: 1
                minimumValue: 0
                maximumValue: 100
                onReleased: {
                    var controls = player.pluginControls
                    controls.volume = parseInt(value)
                    player.pluginControls = controls
                    value = player.pluginControls.volume
                }
                label: qsTr("Volume: %1%").arg(value);
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Refresh database")
                onClicked: {
                    var controls = player.pluginControls
                    controls.refresh = true
                    player.pluginControls = controls
                }
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Sync")
                onClicked: {
                    var controls = player.pluginControls
                    controls.fetch_sync = true
                    player.pluginControls = controls

                    pageStack.push(syncDialog)
                }
            }
        }
    }

    Component {
        id: syncDialog

        Dialog {
            onAccepted: {
                var result = [];
                for (var i = 0; i < player.pluginControls.player_names.length; i++) {
                    if (repeater.itemAt(i).checked)
                        result.push(i)
                }
                var controls = player.pluginControls
                controls.sync_players = result
                player.pluginControls = controls
            }

            Column {
                width: parent.width

                DialogHeader {
                    title: qsTr("Sync settings")
                }

                Repeater {
                    id: repeater
                    model: player.pluginControls.player_names
                    delegate: TextSwitch {
                        id: outputSwitch
                        width: parent.width
                        text: modelData
                        visible: player.pluginControls.active_player !== index
                        checked: player.pluginControls.sync_players.indexOf(index) >= 0
                    }
                }
            }
        }
    }
}
