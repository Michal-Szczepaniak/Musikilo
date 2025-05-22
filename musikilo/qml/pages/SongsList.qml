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
import QtMultimedia 5.6

Item {
    id: songsListPage

    implicitHeight: swipeView.height; implicitWidth: swipeView.width

    Connections {
        target: player

        onStateChanged: {
            if (player.position === player.duration-1 && playlistModel.currentIndex+1 < playlistModel.rowCount()) {
                playlistModel.currentIndex++
            }
        }
    }

    SilicaFlickable {
        id: songsFlickable
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick

        PageHeader {
            id: header
            title: qsTr("Playlist")
        }

        SilicaListView {
            id: songsList
            model: playlistModel
            anchors.top: header.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            delegate: ListItem {
                id: playlistDelegate

                width: songsList.width

                menu: playlistMenuComponent

                Component {
                    id: playlistMenuComponent
                    ContextMenu {
                        MenuItem {
                            text: qsTr("Remove")
                            onClicked: playlistModel.remove(index)
                        }
                    }
                }

                Label {
                    id: playlistSongName
                    x: Theme.horizontalPageMargin
                    text: name
                    anchors.leftMargin: Theme.paddingLarge
                    anchors.rightMargin: Theme.paddingLarge
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    color: playlistModel.currentIndex === index ? Theme.highlightColor : Theme.primaryColor
                    elide: Text.ElideRight
                }

                onClicked: {
                    playlistModel.currentIndex = index
                }
            }
            VerticalScrollDecorator {}
        }
    }
}
