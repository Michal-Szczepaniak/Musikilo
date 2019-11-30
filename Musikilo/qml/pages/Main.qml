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

Page {
    id: mainPage

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: parent.height

        SlideshowView {
            id: swipeView
            itemWidth: width
            itemHeight: height
            clip: true
            interactive: false

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: mainPageHeader.top

            model: VisualItemModel {
                FilesList { }
                SongsList { }
                SongDetails { }
                Settings { }
            }
        }

        TabHeader {
            id: mainPageHeader
            listView: swipeView
            iconArray: [ "image://theme/icon-m-events", "image://theme/icon-m-events", "image://theme/icon-m-media-songs", "image://theme/icon-m-developer-mode", ]
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
        property string artist: mediaPlayer.metaData.author!== undefined ?
                                    mediaPlayer.metaData.author :
                                    mediaPlayer.metaData.albumArtist !== undefined ?
                                        mediaPlayer.metaData.albumArtist :
                                        mediaPlayer.metaData.contributingArtist !== undefined ?
                                            mediaPlayer.metaData.contributingArtist : ""
        property string song: mediaPlayer.metaData.title !== undefined ?
                                  mediaPlayer.metaData.title : ""

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

        identity: "microtube"

        canControl: true

        canGoNext: true
        canGoPrevious: true
        canPause: true
        canPlay: true
        canSeek: false

        playbackStatus: mediaPlayer.playbackState === MediaPlayer.PlayingState ?
                            Mpris.Playing : mediaPlayer.playbackState === MediaPlayer.PausedState ?
                                Mpris.Paused : Mpris.Stopped

        loopStatus: Mpris.None
        shuffle: false
        volume: 1

        onPauseRequested: {
            playlistmodel.pause()
        }

        onPlayRequested: {
            playlistmodel.resume()
        }

        onPlayPauseRequested: {
            mediaPlayer.playbackState == MediaPlayer.PlayingState ? playlistmodel.pause() : playlistmodel.resume()
        }

        onStopRequested: {
            playlistmodel.pause()
        }

        onNextRequested: {
            if (playlistmodel.activeItem + 1 < playlistmodel.rowCount()) playlistmodel.activeItem++
        }

        onPreviousRequested: {
            if (playlistmodel.activeItem > 0) playlistmodel.activeItem--
        }

        onSeekRequested: {
        }
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: if (playlistmodel.activeItem + 1 < playlistmodel.rowCount()) playlistmodel.activeItem++
        }

        CoverAction {
            iconSource: mediaPlayer.playbackState == MediaPlayer.PlayingState ? "image://theme/icon-cover-pause" : "image://theme/icon-cover-play"
            onTriggered: mediaPlayer.playbackState == MediaPlayer.PlayingState ? playlistmodel.pause() : playlistmodel.resume()
        }
    }
}
