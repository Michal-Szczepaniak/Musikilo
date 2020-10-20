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
import org.nemomobile.configuration 1.0
import com.verdanditeam.ofono 1.0

Item {
    id: settingsPage

    implicitHeight: swipeView.height; implicitWidth: swipeView.width

    OfonoModemManager { id: modemManager }

    function tryLogIn() {
        if(settings.hostname !== "" && settings.rootPath !== "" && settings.username !== "" && settings.password !== "" && settings.port !== "") {
            webdavmodel.setConnectionSettings(settings.connectionType, settings.hostname, settings.rootPath, settings.username, simpleCrypt.decryptToString(settings.password), settings.port)
            webdavmodel.getFilesList("/");
        }
    }

    Connections {
        target: modemManager
        onReadyChanged: {
            var key = modemManager.imeiCodes.join("");
            simpleCrypt.key = key;
            tryLogIn();
        }
    }

    Connections {
        target: webdavmodel
        onGotFilesList: {
            connectedMessage.visible = true
            hideConnected.start()
        }
    }

    Timer {
        id: hideConnected
        interval: 3000;
        repeat: false
        onTriggered: connectedMessage.visible = false
    }

    ConfigurationGroup {
        id: settings
        path: "/apps/musikilo"

        property int connectionType: 1
        property string hostname: ""
        property string rootPath: "/"
        property string username: ""
        property string password: ""
        property int port: 443
    }

    SilicaFlickable {
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        contentHeight: column.height + header.height - mainPageHeader.height

        PageHeader {
            id: header
            title: qsTr("Settings")
        }

        Column {
            id: column
            spacing: Theme.paddingLarge
            anchors.topMargin: header.height
            anchors.fill: parent
            ComboBox {
               id: themes
               width: parent.width
               label: "Connection Type"
               currentIndex: settings.connectionType

               menu: ContextMenu {
                   MenuItem { text: "HTTP" }
                   MenuItem { text: "HTTPS" }
               }

               onCurrentItemChanged: {
                   settings.connectionType = themes.currentIndex
                   hostnameValue.focus = true
               }
            }

            TextField {
                id: hostnameValue
                text: settings.hostname
                label: qsTr("Hostname")
                labelVisible: true
                placeholderText: label
                width: parent.width
                EnterKey.onClicked: {
                    settings.hostname = hostnameValue.text
                    rootPathValue.focus = true
                }
                onFocusChanged: settings.hostname = hostnameValue.text
            }

            TextField {
                id: rootPathValue
                text: settings.rootPath
                label: qsTr("Root path")
                labelVisible: true
                placeholderText: label
                width: parent.width
                EnterKey.onClicked: {
                    settings.rootPath = rootPathValue.text
                    usernameValue.focus = true
                }
                onFocusChanged: settings.rootPath = rootPathValue.text
            }

            TextField {
                id: usernameValue
                text: settings.username
                label: qsTr("Username")
                labelVisible: true
                placeholderText: label
                width: parent.width
                EnterKey.onClicked: {
                    settings.username = usernameValue.text
                    passwordValue.focus = true
                }
                onFocusChanged: settings.username = usernameValue.text
            }

            PasswordField {
                id: passwordValue
                width: parent.width
                label: qsTr("Password")
                labelVisible: true
                placeholderText: label
                EnterKey.onClicked: {
                    settings.password = simpleCrypt.encryptToString(passwordValue.text)
                    portValue.focus = true
                }
                onFocusChanged: settings.password = simpleCrypt.encryptToString(passwordValue.text)
            }

            TextField {
                id: portValue
                text: settings.port
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                label: qsTr("Port")
                labelVisible: true
                placeholderText: label
                width: parent.width
                EnterKey.onClicked: settings.port = parseInt(portValue.text)
                onFocusChanged: settings.port = parseInt(portValue.text)
            }

            Button {
                text: qsTr("Connect")
                onClicked: tryLogIn();
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                id: connectedMessage
                visible: false
                text: qsTr("Connected!")
                anchors.horizontalCenter: parent.horizontalCenter
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeSmall
            }

        }
        VerticalScrollDecorator {}
    }
}
