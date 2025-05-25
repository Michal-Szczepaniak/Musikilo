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
import "../dialogs"

Dialog {
    id: dialog

    property var pluginData
    property var pluginCode

    onAccepted: {
        var settings = settingsLoader.item.collectSettings();
        settings.name = pluginData.name;

        settingsManager.setPluginSettings(pluginCode, settings)
    }

    canAccept: settingsLoader.active

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: parent.width

            DialogHeader { }

            Loader {
                id: settingsLoader
                anchors {
                    left: parent.left
                    right: parent.right
                }

                active: true
                asynchronous: true
                sourceComponent:
                    switch(pluginData.plugin) {
                        case "nextcloud":
                            return nextcloudSettingsDialog;
                        case "spotify":
                            break;
                        case "squeezebox":
                            return squeezeBoxSettingsDialog;
                        case "mpd":
                            return mpdSettingsDialog;
                    }
                onLoaded: item.setData(pluginData)
            }
        }
    }

    Component {
        id: nextcloudSettingsDialog

        NextcloudSettingsDialog {

        }
    }

    Component {
        id: mpdSettingsDialog

        MPDSettingsDialog {

        }
    }

    Component {
        id: squeezeBoxSettingsDialog

        SqueezeBoxSettingsDialog {

        }
    }
}
