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
            title: qsTr("MPD controls")
        }

        Column {
            anchors.top: header.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            spacing: Theme.paddingMedium

            Slider {
                width: parent.width
                value: typeof player.pluginControls === "undefined" ? 0 : player.pluginControls.volume
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

            SectionHeader {
                text: qsTr("Outputs")
            }

            Repeater {
                model: player.pluginControls.outputs
                delegate: TextSwitch {
                    id: outputSwitch
                    width: parent.width
                    text: modelData
                    onCheckedChanged: {
                        if (checked === Boolean(player.pluginControls.enabledOutputs & (1 << index))) return;
                        var controls = player.pluginControls
                        controls.enabledOutputs ^= (1 << index)
                        player.pluginControls = controls
                    }

                    Connections {
                        target: player
                        onPluginControlsChanged: outputSwitch.checked = player.pluginControls.enabledOutputs & (1 << index)
                    }
                }
            }
        }

    }
}
