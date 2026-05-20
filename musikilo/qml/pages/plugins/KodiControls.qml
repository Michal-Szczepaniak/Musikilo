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

        PageHeader {
            id: header
            title: qsTr("Kodi controls")
        }

        Column {
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


            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Stop")
                onClicked: {
                    var controls = player.pluginControls
                    controls.stop = true
                    player.pluginControls = controls
                }
            }

            ComboBox {
                width: parent.width
                label: "Audio streams"
                Binding on currentIndex {
                    value: player.pluginControls.currentAudioStream
                }

                menu: ContextMenu {
                    Repeater {
                        model: player.pluginControls.audioStreams
                        delegate: MenuItem {
                            text: modelData
                        }
                    }
                }

                onCurrentIndexChanged: {
                    if (currentIndex >= 0 && currentIndex < player.pluginControls.audioStreams.length) {
                        var controls = player.pluginControls
                        controls.currentAudioStream = currentIndex
                        player.pluginControls = controls
                    }
                }
            }

            ComboBox {
                width: parent.width
                label: "Video streams"
                Binding on currentIndex {
                    value: player.pluginControls.currentVideoStream
                }

                menu: ContextMenu {
                    Repeater {
                        model: player.pluginControls.videoStreams
                        delegate: MenuItem {
                            text: modelData
                        }
                    }
                }

                onCurrentIndexChanged: {
                    if (currentIndex >= 0 && currentIndex < player.pluginControls.videoStreams.length) {
                        var controls = player.pluginControls
                        controls.currentVideoStream = currentIndex
                        player.pluginControls = controls
                    }
                }
            }

            ComboBox {
                width: parent.width
                label: "Subtitles"
                Binding on currentIndex {
                    value: player.pluginControls.currentSubtitle
                }

                menu: ContextMenu {
                    Repeater {
                        model: player.pluginControls.subtitles
                        delegate: MenuItem {
                            text: modelData
                        }
                    }
                }

                onCurrentIndexChanged: {
                    if (currentIndex >= 0 && currentIndex < player.pluginControls.subtitles.length) {
                        var controls = player.pluginControls
                        controls.currentSubtitle = currentIndex
                        player.pluginControls = controls
                    }
                }
            }
        }

    }
}
