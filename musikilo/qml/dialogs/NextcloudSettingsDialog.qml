import QtQuick 2.0
import Sailfish.Silica 1.0

Column {
    width: parent.width

    function setData(data) {
        connectionType.currentIndex = data.connectionType;
        hostnameValue.text = data.hostname;
        rootPathValue.text = data.rootPath;
        usernameValue.text = data.username;
        passwordValue.text = data.password;
        portValue.text = data.port;
    }

    function collectSettings() {
        return {
            plugin: "nextcloud",
            connectionType: connectionType.currentIndex,
            hostname: hostnameValue.text,
            rootPath: rootPathValue.text,
            username: usernameValue.text,
            password: passwordValue.text,
            port: portValue.text
        }
    }

    ComboBox {
        id: connectionType
        width: parent.width
        label: "Connection Type"

        menu: ContextMenu {
            MenuItem { text: "HTTP" }
            MenuItem { text: "HTTPS" }
        }

        onCurrentItemChanged: {
            hostnameValue.focus = true
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
       id: rootPathValue
       label: qsTr("Root path")
       labelVisible: true
       placeholderText: label
       width: parent.width
    }

    TextField {
       id: usernameValue
       label: qsTr("Username")
       labelVisible: true
       placeholderText: label
       width: parent.width
    }

    PasswordField {
       id: passwordValue
       width: parent.width
       label: qsTr("Password")
       labelVisible: true
       placeholderText: label
    }

    TextField {
       id: portValue
       inputMethodHints: Qt.ImhFormattedNumbersOnly
       label: qsTr("Port")
       labelVisible: true
       placeholderText: label
       width: parent.width
    }
}
