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

CoverBackground {
    Column {
        anchors.fill: parent
        anchors.margins: Theme.paddingLarge*2
        spacing: Theme.paddingLarge

        Label {
            id: label
            text: "Musikilo"
            font.pixelSize: Theme.fontSizeLarge
            font.bold: Font.Bold
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Label {
            id: title
            width: parent.width
            text: player.title
            wrapMode: "WrapAtWordBoundaryOrAnywhere"
            font.bold: Font.Bold
            horizontalAlignment: Text.AlignHCenter
        }

        Label {
            id: author
            width: parent.width
            text: player.artist
            font.bold: Font.Bold
            horizontalAlignment: Text.AlignHCenter
            wrapMode: "WrapAtWordBoundaryOrAnywhere"
        }
    }
}
