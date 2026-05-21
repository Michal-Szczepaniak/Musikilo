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
            title: qsTr("Kodi controls")
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
                label: qsTr("Audio streams")
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
                label: qsTr("Video streams")
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
                label: qsTr("Subtitles")
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

            Grid {
                id: remoteGrid
                columns: 3
                spacing: Theme.paddingLarge
                anchors.horizontalCenter: parent.horizontalCenter

                Item {
                    width: col.width/3 - Theme.paddingLarge
                    height: col.width/3 - Theme.paddingLarge

                    Button {
                        anchors.centerIn: parent
                        text: qsTr("Menu")
                        width: Theme.itemSizeExtraLarge
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var controls = player.pluginControls
                            controls.menu = true
                            player.pluginControls = controls
                        }
                    }
                }

                Item {
                    width: col.width/3 - Theme.paddingLarge
                    height: col.width/3 - Theme.paddingLarge

                    Button {
                        anchors.centerIn: parent
                        text: qsTr("Up")
                        width: Theme.itemSizeExtraLarge
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var controls = player.pluginControls
                            controls.up = true
                            player.pluginControls = controls
                        }
                    }
                }

                Item {
                    width: col.width/3 - Theme.paddingLarge
                    height: col.width/3 - Theme.paddingLarge

                    Button {
                        anchors.centerIn: parent
                        text: qsTr("Home")
                        width: Theme.itemSizeExtraLarge
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var controls = player.pluginControls
                            controls.home = true
                            player.pluginControls = controls
                        }
                    }
                }

                Item {
                    width: col.width/3 - Theme.paddingLarge
                    height: col.width/3 - Theme.paddingLarge

                    Button {
                        anchors.centerIn: parent
                        text: qsTr("Left")
                        width: Theme.itemSizeExtraLarge
                    }


                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var controls = player.pluginControls
                            controls.left = true
                            player.pluginControls = controls
                        }
                    }
                }

                Item {
                    width: col.width/3 - Theme.paddingLarge
                    height: col.width/3 - Theme.paddingLarge

                    Button {
                        anchors.centerIn: parent
                        text: qsTr("OK")
                        width: Theme.itemSizeExtraLarge
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var controls = player.pluginControls
                            controls.select = true
                            player.pluginControls = controls
                        }
                    }
                }

                Item {
                    width: col.width/3 - Theme.paddingLarge
                    height: col.width/3 - Theme.paddingLarge

                    Button {
                        anchors.centerIn: parent
                        text: qsTr("Right")
                        width: Theme.itemSizeExtraLarge
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var controls = player.pluginControls
                            controls.right = true
                            player.pluginControls = controls
                        }
                    }
                }

                Item {
                    width: col.width/3 - Theme.paddingLarge
                    height: col.width/3 - Theme.paddingLarge

                    Button {
                        anchors.centerIn: parent
                        text: qsTr("Back")
                        width: Theme.itemSizeExtraLarge
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var controls = player.pluginControls
                            controls.back = true
                            player.pluginControls = controls
                        }
                    }
                }

                Item {
                    width: col.width/3 - Theme.paddingLarge
                    height: col.width/3 - Theme.paddingLarge

                    Button {
                        anchors.centerIn: parent
                        text: qsTr("Down")
                        width: Theme.itemSizeExtraLarge
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var controls = player.pluginControls
                            controls.down = true
                            player.pluginControls = controls
                        }
                    }
                }

                Item {
                    width: col.width/3 - Theme.paddingLarge
                    height: col.width/3 - Theme.paddingLarge

                    Button {
                        anchors.centerIn: parent
                        text: qsTr("Stop")
                        width: Theme.itemSizeExtraLarge
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var controls = player.pluginControls
                            controls.stop = true
                            player.pluginControls = controls
                        }
                    }
                }

                Item {
                    width: col.width/3 - Theme.paddingLarge
                    height: Theme.itemSizeExtraLarge

                    Button {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Previous chapter")
                        width: col.width/2 - Theme.paddingLarge
                        onClicked: {
                            var controls = player.pluginControls
                            controls.prevChapter = true
                            player.pluginControls = controls
                        }
                    }
                }

                Item {
                    width: col.width/3 - Theme.paddingLarge
                    height: Theme.itemSizeExtraLarge
                }

                Item {
                    width: col.width/3 - Theme.paddingLarge
                    height: Theme.itemSizeExtraLarge

                    Button {
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Next chapter")
                        width: col.width/2 - Theme.paddingLarge
                        onClicked: {
                            var controls = player.pluginControls
                            controls.nextChapter = true
                            player.pluginControls = controls
                        }
                    }
                }
            }
        }

    }
}
