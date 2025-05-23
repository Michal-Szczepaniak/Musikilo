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
import org.nemomobile.mpris 1.0
import "../components"
import "plugins"

Page {
    id: mainPage

    allowedOrientations: Orientation.All

    SlideshowView {
        id: swipeView
        itemWidth: width
        itemHeight: height
        clip: true
        interactive: false

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height - mainFlickable.height - mainFlickable.contentY

        model: VisualItemModel {
            FilesList { }
            SongsList { }
            Loader {
                active: true
                sourceComponent: {
                    console.log(settingsManager.currentPluginType)
                    switch (settingsManager.currentPluginType) {
                    case "mpd":
                        return mpdControls;
                    default:
                        return noControls;
                    }
                }
            }
            SongDetails { }
            Settings { }
        }
    }

    SilicaFlickable {
        id: mainFlickable
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: mainPageHeader.height
        contentHeight: mainPageHeader.height
        flickableDirection: Flickable.VerticalFlick

        TabHeader {
            id: mainPageHeader
            listView: swipeView
            iconArray: [ "image://theme/icon-m-events", "qrc:///images/icon-m-playlists.svg", "image://theme/icon-m-menu", "image://theme/icon-m-media-songs", "image://theme/icon-m-developer-mode", ]
        }

        PushUpMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("About.qml"))
            }
        }
    }


    MprisPlayer {
        id: mprisPlayer

        serviceName: "musikilo"
        property string artist: player.artist
        property string song: player.title

        onArtistChanged: {
            var metadata = mprisPlayer.metadata

            metadata[Mpris.metadataToString(Mpris.Artist)] = [artist] // List of strings

            mprisPlayer.metadata = metadata
        }

        onSongChanged: {
            var metadata = mprisPlayer.metadata

            metadata[Mpris.metadataToString(Mpris.Title)] = song // String

            mprisPlayer.metadata = metadata
        }

        identity: "Musikilo"

        canControl: true

        canGoNext: true
        canGoPrevious: true
        canPause: true
        canPlay: true
        canSeek: false

        playbackStatus: player.state === MediaPlayer.PlayingState ?
                            Mpris.Playing : player.state === MediaPlayer.PausedState ?
                                Mpris.Paused : Mpris.Stopped

        loopStatus: Mpris.None
        shuffle: false
        volume: 1

        onPauseRequested: {
            player.pause()
        }

        onPlayRequested: {
            player.play()
        }

        onPlayPauseRequested: {
            player.state === MediaPlayer.PlayingState ? player.pause() : player.play()
        }

        onStopRequested: {
            playlistModel.pause()
        }

        onNextRequested: {
            if (playlistModel.currentIndex + 1 < playlistModel.rowCount()) playlistModel.currentIndex++
        }

        onPreviousRequested: {
            if (playlistModel.currentIndex > 0) playlistModel.currentIndex--
        }

        onSeekRequested: {
        }
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: player.state === MediaPlayer.PlayingState ? "image://theme/icon-cover-pause" : "image://theme/icon-cover-play"
            onTriggered: player.state === MediaPlayer.PlayingState ? player.pause() : player.play()
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-next-song"
            onTriggered: if (playlistModel.currentIndex + 1 < playlistModel.rowCount()) playlistModel.currentIndex++
        }
    }

    Component {
        id: mpdControls

        MPDControls {}
    }

    Component {
        id: noControls

        Item {
            implicitHeight: swipeView.height; implicitWidth: swipeView.width

            SilicaFlickable {
                anchors.fill: parent

                PageHeader {
                    title: qsTr("Plugin controls")
                }

                Label {
                    text: 'No controls available for this plugin'
                    anchors.centerIn: parent
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    color: Theme.secondaryHighlightColor
                }
            }
        }
    }
}
