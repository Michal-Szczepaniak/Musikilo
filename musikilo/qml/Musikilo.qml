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
import Nemo.Notifications 1.0
import "pages"

ApplicationWindow
{
    id: appWindow
    initialPage: Component { Main { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    Notification {
         id: testNotification

         summary: qsTr("Musikilo")
         replacesId: 1
    }

    Connections {
        target: fileModel
        onErrorOccured: {
            testNotification.body = qsTr("Error occured: %1").arg(error);
            testNotification.publish();
            pageStack.push(errorDialog,  { error: error })
        }
    }

    Connections {
        target: playlistModel
        onErrorOccured: {
            testNotification.body = qsTr("Error occured: %1").arg(error);
            testNotification.publish();
            pageStack.push(errorDialog,  { error: error })
        }
    }

    Connections {
        target: settingsManager
        onTestSucceeded: {
            testNotification.body = qsTr("Test successful");
            testNotification.publish();
        }

        onTestFailed: {
            testNotification.body = qsTr("Test failed: %1").arg(message);
            testNotification.publish();
        }
    }

    Connections {
        target: player
        onErrorOccured: {
            testNotification.body = qsTr("Error occured: %1").arg(error);
            testNotification.publish();
            pageStack.push(errorDialog,  { error: error })
        }

        onStateChanged: {
            if (state === MediaPlayer.StoppedState && playlistModel.currentIndex + 1 < playlistModel.rowCount()) {
                playlistmodel.currentIndex++
            }
        }
    }

    Component {
        id: errorDialog

        Dialog {
            property string error
            canAccept: false

            Column {
                width: parent.width

                DialogHeader {
                    title: qsTr("Error")
                    cancelText: qsTr("Okay")
                }

                Label {
                    id: nameField
                    text: errorDialog.error
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: Theme.pad
                    anchors.rightMargin: Theme.paddingLargedingLarge
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }
            }
        }
    }
}
