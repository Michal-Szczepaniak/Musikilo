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

    onLandscapeChanged: {
        record.anchors.horizontalCenter = !landscape ? settingsPage.horizontalCenter : undefined
        record.x = landscape ? Screen.height/8 : 0
    }

    Component.onCompleted: {
        record.x = landscape ? Screen.height/8 : 0
        record.anchors.horizontalCenter = !landscape ? settingsPage.horizontalCenter : undefined
    }

    Connections {
        target: playlistModel
        onRowsInserted: playlistElementsCount = playlistModel.rowCount()
        onRowsRemoved: playlistElementsCount = playlistModel.rowCount()
        onModelReset: playlistElementsCount = playlistModel.rowCount()
    }

    Connections {
        target: player
        onDurationChanged: {
            if (player.duration > 0) {
                stylus.rotation = -8
                stylusAnimation.from = -8
                stylusAnimation.to = 15
                stylusAnimation.duration = player.duration
                stylusAnimation.restart()
            }
        }

        onPositionChanged: {
            progressSlider.maximumValue = player.duration
            progressSlider.value = player.position
        }

        onStateChanged: {
            if (player.state === MediaPlayer.StoppedState) {
                stylus.rotation = -20
            } else if (player.state === MediaPlayer.PausedState) {
                stylusAnimation.pause()
            } else {
                stylusAnimation.resume()
            }
        }
    }

    MouseArea {
        id: mousearea
        anchors.fill: parent
        propagateComposedEvents: true

        drag {
            target: draggable;
            minimumX: -Theme.itemSizeHuge;
            maximumX: Theme.itemSizeHuge;
            minimumY: 0;
            maximumY: 0;
            axis: Drag.XAxis;
        }

        onReleased: {
            draggable.x = 0
            if (draggable.x < -(Theme.itemSizeHuge/2) && playlistModel.currentIndex + 1 < playlistElementsCount) {
                playlistModel.currentIndex++
            }

            if (draggable.x > Theme.itemSizeHuge/2 && playlistModel.currentIndex > 0) {
                playlistModel.currentIndex--
            }
        }

        onClicked: player.state === MediaPlayer.PlayingState ? player.pause() : player.play()

        Item {
            id: draggable

            width: parent.width
            height: parent.height
            x: 0

            Behavior on x { PropertyAnimation { } }

            Item {
                id: draggableContainer
                anchors.fill: parent
            }


            Rectangle {
                id: nextBackground
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: draggableContainer.right
                width: Theme.itemSizeHuge
                visible: draggable.x !== 0
                color: draggable.x < -(Theme.itemSizeHuge / 2) ?
                           Qt.rgba(Theme.highlightBackgroundColor.r, Theme.highlightBackgroundColor.g, Theme.highlightBackgroundColor.b, 0.85) :
                           Qt.rgba(Theme.highlightDimmerColor.r, Theme.highlightDimmerColor.g, Theme.highlightDimmerColor.b, 0.85)

                IconButton {
                    id: nextButton
                    icon.source: "image://theme/icon-m-next"
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
                anchors.right: draggableContainer.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: Theme.itemSizeHuge
                visible: draggable.x !== 0
                color: draggable.x > (Theme.itemSizeHuge / 2) ?
                           Qt.rgba(Theme.highlightBackgroundColor.r, Theme.highlightBackgroundColor.g, Theme.highlightBackgroundColor.b, 0.85) :
                           Qt.rgba(Theme.highlightDimmerColor.r, Theme.highlightDimmerColor.g, Theme.highlightDimmerColor.b, 0.85)

                IconButton {
                    id: prevButton
                    icon.source: "image://theme/icon-m-previous"
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

    Image {
        id: record
        width: Screen.width/2
        height: width

        anchors.top: parent.top
        x: Screen.height/8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: !landscape ? Screen.height/8 : Theme.paddingLarge*2

        source: "qrc:///images/record.png"

        RotationAnimation on rotation {
            loops: Animation.Infinite
            from: record.rotation
            to: record.rotation+360
            running: player.state === MediaPlayer.PlayingState && Qt.application.state === Qt.ApplicationActive
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
            text: player.title
            font.pixelSize: Theme.fontSizeLarge
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Label {
            id: author
            width: parent.width
            wrapMode: "WrapAtWordBoundaryOrAnywhere"
            text: player.artist
            font.pixelSize: Theme.fontSizeLarge
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Item {}

        Label {
            id: album
            width: parent.width
            wrapMode: "WrapAtWordBoundaryOrAnywhere"
            text: qsTr("Album: %1").arg(player.album)
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Label {
            id: bitrate
            text: qsTr("Bit rate: %1 kb/s").arg(player.bitrate)
            visible: player.bitrate !== ""
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Label {
        id: progress
        anchors.left: parent.left
        anchors.bottom: progressSlider.bottom
        anchors.margins: Theme.paddingLarge
        text: Format.formatDuration(Math.round(player.position/1000), ((player.duration/1000) > 3600 ? Formatter.DurationLong : Formatter.DurationShort))
    }

    Label {
        id: duration
        anchors.right: parent.right
        anchors.bottom: progressSlider.bottom
        anchors.margins: Theme.paddingLarge
        text: Format.formatDuration(Math.round(player.duration/1000), ((player.duration/1000) > 3600 ? Formatter.DurationLong : Formatter.DurationShort))
    }

    Slider {
        id: progressSlider
        value: player.position
        valueText: down ? Format.formatDuration(Math.round(value/1000), ((value/1000) > 3600 ? Formatter.DurationLong : Formatter.DurationShort)) : ""
        minimumValue: 0
        maximumValue: player.duration !== 0 ? player.duration : 1
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
            player.position = progressSlider.value
        }
    }

    Label {
        id: playlistTracks
        text: qsTr("Song %1/%2").arg(playlistModel.currentIndex + 1).arg(playlistElementsCount)
        visible: playlistElementsCount > 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Theme.paddingLarge
    }
}
