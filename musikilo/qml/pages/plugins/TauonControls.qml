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
    id: tauonControlsPage

    implicitHeight: swipeView.height; implicitWidth: swipeView.width

    SilicaFlickable {
        id: filesFlickable
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        contentHeight: header.height + col.height

        PageHeader {
            id: header
            title: qsTr("Tauon controls")
        }

        Column {
            id: col
            anchors.top: header.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            spacing: Theme.paddingMedium

            Slider {
                width: parent.width
                Binding on value { value: player.pluginControls.volume }
                stepSize: 1
                minimumValue: 0
                maximumValue: 100
                onReleased: {
                    var controls = player.pluginControls
                    controls.volume = parseInt(value)
                    player.pluginControls = controls
                }
                label: qsTr("Volume: %1%").arg(value);
            }

            ComboBox {
                width: parent.width
                label: "Playlist"
                Binding on currentIndex {
                    value: player.pluginControls.active_playlist
                }

                menu: ContextMenu {
                    Repeater {
                        model: player.pluginControls.playlists
                        delegate: MenuItem {
                            text: modelData
                        }
                    }
                }

                onCurrentIndexChanged: {
                    if (currentIndex >= 0 && currentIndex < player.pluginControls.playlists.length) {
                        var controls = player.pluginControls
                        controls.active_playlist = currentIndex
                        player.pluginControls = controls
                    }
                }
            }
        }

    }
}
