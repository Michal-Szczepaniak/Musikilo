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

Column {
    width: parent.width

    function setData(data) {
        hostnameValue.text = data.hostname;
        passwordValue.text = data.password;
        portValue.text = data.port;
    }

    function collectSettings() {
        return {
            plugin: "mpd",
            hostname: hostnameValue.text,
            password: passwordValue.text,
            port: portValue.text
        }
    }

    TextField {
       id: hostnameValue
       label: qsTr("Hostname")
       labelVisible: true
       placeholderText: label
       width: parent.width
    }

    TextField {
       id: portValue
       inputMethodHints: Qt.ImhFormattedNumbersOnly
       label: qsTr("Port")
       labelVisible: true
       placeholderText: "6600"
       width: parent.width
    }

    PasswordField {
       id: passwordValue
       width: parent.width
       label: qsTr("Password")
       labelVisible: true
       placeholderText: label
    }
}
