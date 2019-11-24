import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.6
import "../components"

Page {
    id: mainPage

    allowedOrientations: Orientation.All

    SlideshowView {
        id: swipeView
        itemWidth: width
        itemHeight: height
        clip: true

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: mainPageHeader.top

        model: VisualItemModel {
            FilesList { }
            SongDetails { }
        }
    }

    MediaPlayer {
        id: mediaPlayer
        autoPlay: true

    }

    TabHeader {
        id: mainPageHeader
        listView: swipeView
        iconArray: [ "image://theme/icon-m-events", "image://theme/icon-s-chat", "image://theme/icon-m-developer-mode", ]
    }
}
