import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: filesListPage

    implicitHeight: swipeView.height; implicitWidth: swipeView.width

    Component.onCompleted: {
        playlistmodel.setMediaPlayer(mediaPlayer);
        webdavmodel.getFilesList("/");
    }


    SilicaFlickable {
        id: filesFlickable
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
//        contentHeight: userColumn.height


        PullDownMenu {
            MenuItem {
                text: qsTr("Quit")
                onClicked: Qt.quit()
            }

            MenuItem {
                text: qsTr("New…")
                onClicked: pageStack.push(Qt.resolvedUrl("NewChat.qml"))
            }
        }

        PageHeader {
            id: header
            title: qsTr("Buddy list")
        }

        SilicaListView {
            id: filesList
            model: webdavmodel
            currentIndex: -1
            anchors.top: header.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            header: SearchField {
                id: searchField
                width: parent.width
                placeholderText: "Search"
            }
            delegate: ListItem {
                id: delegate

                width: filesList.width

                menu: contextMenuComponent

                Component {
                    id: contextMenuComponent
                    ContextMenu {
                        MenuItem {
                            text: "Delete"
                        }
                    }
                }

                Label {
                    id: buddyName
                    x: Theme.horizontalPageMargin
                    text: name
                    anchors.leftMargin: Theme.paddingLarge
                    anchors.rightMargin: Theme.paddingLarge
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                    elide: Text.ElideRight
                }

                onClicked: {
                    if(isDir) {
                        webdavmodel.getFilesList(path);
                    } else {
                        webdavmodel.play(path)
                    }
                }
            }
            VerticalScrollDecorator {}
        }
    }
}
