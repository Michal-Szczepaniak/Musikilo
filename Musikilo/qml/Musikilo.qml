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
import "pages"

ApplicationWindow
{
    id: appWindow
    initialPage: Component { Main { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    Component.onCompleted: {
        playlistmodel.setMediaPlayer(mediaPlayer);
    }

    Audio {
        id: mediaPlayer
        onPlaybackStateChanged: {
            if (playbackState === MediaPlayer.PlayingState) {
                mediaPlayer.operationsPending = false
            } else if (playbackState === MediaPlayer.StoppedState && !mediaPlayer.operationsPending && playlistmodel.activeItem + 1 < playlistmodel.rowCount() && mediaPlayer.operationsPending === false) {
                playlistmodel.activeItem++
            }
        }

        property bool operationsPending: false
    }
}
