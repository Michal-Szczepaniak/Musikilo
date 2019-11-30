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
    id: settingsPage

    implicitHeight: swipeView.height; implicitWidth: swipeView.width

    property int playlistElementsCount: 0
    property bool landscape: ( mainPage.orientation === Orientation.Landscape || mainPage.orientation === Orientation.LandscapeInverted )

    Connections {
        target: playlistmodel
        onRowsInserted: playlistElementsCount = playlistmodel.rowCount()
        onRowsRemoved: playlistElementsCount = playlistmodel.rowCount()
        onModelReset: playlistElementsCount = playlistmodel.rowCount()
    }

    Connections {
        target: mediaPlayer
        onDurationChanged: {
            if (mediaPlayer.duration > 0) {
                stylus.rotation = -8
                stylusAnimation.from = -8
                stylusAnimation.duration = mediaPlayer.duration
                stylusAnimation.restart()
            }
        }

        onPositionChanged: {
            progressSlider.maximumValue = mediaPlayer.duration
            progressSlider.value = mediaPlayer.position
        }

        onPlaybackStateChanged: if (mediaPlayer.playbackState === MediaPlayer.StoppedState) stylus.rotation = -20
    }

    SilicaFlickable {
        id: playlistFlickable
        anchors.fill: parent
        flickableDirection: Flickable.HorizontalFlick
        boundsBehavior: Flickable.DragOverBounds
        clip: true

        onContentXChanged: {
            if (contentX > Theme.itemSizeExtraLarge/2 && playlistmodel.activeItem + 1 < playlistElementsCount) {
                nextBackground.visible = true
            } else {
                nextBackground.visible = false
            }

            if (contentX < -(Theme.itemSizeExtraLarge/2) && contentX < 0 && playlistmodel.activeItem > 0) {
                prevBackground.visible = true
            } else {
                prevBackground.visible = false
            }
        }

        onDraggingChanged: {
            console.log(contentX)
            if (contentX > Theme.itemSizeExtraLarge/2 && playlistmodel.activeItem + 1 < playlistElementsCount) {
                mediaPlayer.operationsPending = true
                playlistmodel.activeItem++
            }

            if (contentX < -(Theme.itemSizeExtraLarge/2) && contentX < 0 && playlistmodel.activeItem > 0) {
                mediaPlayer.operationsPending = true
                playlistmodel.activeItem--
            }
        }

        MouseArea {
            id: mousearea
            anchors.fill: parent
            propagateComposedEvents: true

            onClicked: mediaPlayer.playbackState === MediaPlayer.PlayingState ? playlistmodel.pause() : playlistmodel.resume()
        }

        Image {
            id: record
            width: Screen.width/2
            height: width

            x: landscape ? Screen.height/8 : 0
            anchors.top: parent.top
            anchors.horizontalCenter: !landscape ? parent.horizontalCenter : undefined
            anchors.topMargin: !landscape ? Screen.height/8 : Theme.paddingLarge*2

            source: "qrc:///images/record.png"

            RotationAnimation on rotation {
                loops: Animation.Infinite
                from: record.rotation
                to: record.rotation+360
                running: mediaPlayer.playbackState === MediaPlayer.PlayingState && Qt.application.state === Qt.ApplicationActive
                duration: 10000
            }
        }

        Image {
            id: stylus
            width: record.width * 1.5
            height: record.width * 1.5
            rotation: 345

            x: record.x + record.width/3
            y: record.y - height/2.4

            RotationAnimation on rotation {
                id: stylusAnimation
                from: -8
                to: 15
                running: mediaPlayer.playbackState === MediaPlayer.PlayingState
            }

            source: "qrc:///images/stylus.png"
        }

        Column {
            anchors.top: landscape ? parent.top : record.bottom
            anchors.topMargin: Theme.paddingLarge*2
            x: !landscape ? 0 : Screen.height / 2
            width: !landscape ? Screen.width : Screen.height / 2
            spacing: Theme.paddingMedium

            Label {
                id: title
                width: parent.width
                wrapMode: "WrapAtWordBoundaryOrAnywhere"
                text: mediaPlayer.metaData.title !== undefined ? mediaPlayer.metaData.title : ""
                font.pixelSize: Theme.fontSizeLarge
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: author
                width: parent.width
                wrapMode: "WrapAtWordBoundaryOrAnywhere"
                text: mediaPlayer.metaData.author!== undefined ?
                          mediaPlayer.metaData.author :
                          mediaPlayer.metaData.albumArtist !== undefined ?
                              mediaPlayer.metaData.albumArtist :
                              mediaPlayer.metaData.contributingArtist !== undefined ?
                                  mediaPlayer.metaData.contributingArtist : ""
                font.pixelSize: Theme.fontSizeLarge
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Item {}

            Label {
                id: album
                width: parent.width
                wrapMode: "WrapAtWordBoundaryOrAnywhere"
                text: mediaPlayer.metaData.albumTitle !== undefined ?
                          qsTr("Album: %1").arg(mediaPlayer.metaData.albumTitle) : ""
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: year
                text: mediaPlayer.metaData.year !== undefined ?
                          qsTr("Year: %1").arg(mediaPlayer.metaData.year) : ""
                visible: mediaPlayer.metaData.year !== undefined
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: trackNumber
                text: mediaPlayer.metaData.trackNumber !== undefined ?
                          qsTr("Track No.: %1").arg(mediaPlayer.metaData.trackNumber) : ""
                visible: mediaPlayer.metaData.trackNumber !== undefined
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: bitrate
                text: mediaPlayer.metaData.audioBitRate !== undefined ?
                          qsTr("Bit rate: %1 kb/s").arg(Math.round(mediaPlayer.metaData.audioBitRate / 1000)) : ""
                visible: mediaPlayer.metaData.audioBitRate !== undefined
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }

        Label {
            id: progress
            anchors.left: parent.left
            anchors.bottom: progressSlider.bottom
            anchors.margins: Theme.paddingLarge
            text: Format.formatDuration(Math.round(mediaPlayer.position/1000), ((mediaPlayer.duration/1000) > 3600 ? Formatter.DurationLong : Formatter.DurationShort))
        }

        Label {
            id: duration
            anchors.right: parent.right
            anchors.bottom: progressSlider.bottom
            anchors.margins: Theme.paddingLarge
            text: Format.formatDuration(Math.round(mediaPlayer.duration/1000), ((mediaPlayer.duration/1000) > 3600 ? Formatter.DurationLong : Formatter.DurationShort))
        }

        Slider {
            id: progressSlider
            value: mediaPlayer.position
            valueText: down ? Format.formatDuration(Math.round(value/1000), ((value/1000) > 3600 ? Formatter.DurationLong : Formatter.DurationShort)) : ""
            minimumValue: 0
            maximumValue: mediaPlayer.duration !== 0 ? mediaPlayer.duration : 1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: playlistTracks.top
            anchors.leftMargin: Theme.paddingLarge
            anchors.rightMargin: Theme.paddingLarge

            onReleased: {
                stylusAnimation.stop()
                stylusAnimation.from = ((progressSlider.value/progressSlider.maximumValue) * 23) - 8
                stylusAnimation.duration = progressSlider.maximumValue - progressSlider.value
                stylusAnimation.to = 15
                stylusAnimation.start()
                mediaPlayer.seek(progressSlider.value)
            }
        }

        Label {
            id: playlistTracks
            text: qsTr("Song %1/%2").arg(playlistmodel.activeItem + 1).arg(playlistElementsCount)
            visible: playlistElementsCount > 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: Theme.paddingLarge
        }

        Rectangle {
            id: nextBackground
            anchors.left: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: Theme.itemSizeLarge
            color: Qt.rgba(Theme.highlightDimmerColor.r, Theme.highlightDimmerColor.g, Theme.highlightDimmerColor.b, 0.85)

            IconButton {
                id: nextButton
                icon.source: "image://theme/icon-m-previous"
                anchors.centerIn: parent
            }
        }

        OpacityRampEffect {
            sourceItem: nextBackground
            direction: OpacityRamp.RightToLeft
            offset: 0.0
            slope: parent.opacity * 0.8
        }

        Rectangle {
            id: prevBackground
            anchors.right: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: Theme.itemSizeLarge
            color: Qt.rgba(Theme.highlightDimmerColor.r, Theme.highlightDimmerColor.g, Theme.highlightDimmerColor.b, 0.85)

            IconButton {
                id: prevButton
                icon.source: "image://theme/icon-m-next"
                anchors.centerIn: parent
            }
        }

        OpacityRampEffect {
            sourceItem: prevBackground
            direction: OpacityRamp.LeftToRight
            offset: 0.0
            slope: parent.opacity * 0.8
        }
    }
}
