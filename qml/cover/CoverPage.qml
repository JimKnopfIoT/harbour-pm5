import QtQuick 2.0
import Sailfish.Silica 1.0
import QtGraphicalEffects 1.0

CoverBackground {
    property color accent: Proxmark.connected ? "#28c76f" : (Proxmark.present ? "#ff8a00" : "#6b6f7a")

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#232429" }
            GradientStop { position: 1.0; color: "#0d0e12" }
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: Theme.paddingLarge

        Item {
            width: Theme.itemSizeMedium; height: width
            anchors.horizontalCenter: parent.horizontalCenter
            Rectangle {
                id: orb
                anchors.centerIn: parent
                width: Theme.paddingLarge; height: width; radius: width/2
                color: accent
            }
            Glow { anchors.fill: orb; source: orb; radius: 16; samples: 17; color: accent }
            Repeater {
                model: Proxmark.connected ? 2 : 0
                delegate: Rectangle {
                    anchors.centerIn: parent
                    width: orb.width; height: width; radius: width/2
                    color: "transparent"; border.width: 2; border.color: accent
                    SequentialAnimation on scale {
                        running: true; loops: Animation.Infinite
                        PauseAnimation { duration: index * 700 }
                        NumberAnimation { from: 1; to: 4; duration: 1400; easing.type: Easing.OutCubic }
                    }
                    SequentialAnimation on opacity {
                        running: true; loops: Animation.Infinite
                        PauseAnimation { duration: index * 700 }
                        NumberAnimation { from: 0.7; to: 0; duration: 1400; easing.type: Easing.OutCubic }
                    }
                }
            }
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "PM5"
            font.pixelSize: Theme.fontSizeExtraLarge
            font.bold: true
            color: Theme.primaryColor
        }
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: Proxmark.connected ? "connected" : (Proxmark.present ? "ready" : "no device")
            font.pixelSize: Theme.fontSizeExtraSmall
            color: accent
        }
    }
}
