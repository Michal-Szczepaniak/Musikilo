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
    id: filesListPage

    implicitHeight: swipeView.height; implicitWidth: swipeView.width

    Connections {
        target: settingsManager

        onPluginReady: {
            fileModel.getFilesList('/');
        }
    }

    SilicaFlickable {
        id: filesFlickable
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick

        PageHeader {
            id: header
            title: qsTr("Files list")
        }

        SilicaListView {
            id: filesList
            model: fileModel
            anchors.top: header.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            delegate: ListItem {
                id: delegate

                width: filesList.width

                menu: contextMenuComponent

                Component {
                    id: contextMenuComponent
                    ContextMenu {
                        MenuItem {
                            text: "Add file"
                            onClicked: playlistModel.addSong(path)
                        }

                        MenuItem {
                            text: "Play file"
                            onClicked: {
                                playlistModel.playSong(path)
                            }
                        }
                    }
                }

                Icon {
                    id: icon
                    width: Theme.iconSizeMedium
                    height: width
                    anchors.leftMargin: Theme.paddingLarge
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    source: isDir ? "image://theme/icon-m-file-folder" : "image://theme/icon-m-media-songs"
                }

                Label {
                    id: buddyName
                    text: name
                    anchors.leftMargin: Theme.paddingMedium
                    anchors.rightMargin: Theme.paddingLarge
                    anchors.left: icon.right
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                    elide: Text.ElideRight
                    truncationMode: TruncationMode.Fade
                }

                onClicked: {
                    if(isDir) {
                        fileModel.getFilesList(path);
                    } else {
                    }
                }
            }
            VerticalScrollDecorator {}
        }
    }
}
