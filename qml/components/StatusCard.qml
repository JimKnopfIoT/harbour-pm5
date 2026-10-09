import QtQuick 2.0
import Sailfish.Silica 1.0
import QtGraphicalEffects 1.0

// Connection state: pulsing signal ring + version + connect/disconnect.
Item {
    id: root
    width: parent.width
    height: Theme.itemSizeHuge * 1.4
    property color accent: Proxmark.connected ? "#28c76f" : (Proxmark.present ? "#ff8a00" : "#6b6f7a")

    Rectangle {
        anchors { fill: parent; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
        radius: Theme.paddingLarge
        color: Qt.rgba(1, 1, 1, 0.04)
        border.width: 1
        border.color: Qt.rgba(accent.r, accent.g, accent.b, 0.5)

        Row {
            anchors { fill: parent; margins: Theme.paddingLarge }
            spacing: Theme.paddingLarge

            // signal orb
            Item {
                width: Theme.itemSizeMedium
                height: Theme.itemSizeMedium
                anchors.verticalCenter: parent.verticalCenter

                Rectangle {
                    id: orb
                    anchors.centerIn: parent
                    width: Theme.paddingLarge * 1.4; height: width; radius: width / 2
                    color: accent
                }
                Glow { anchors.fill: orb; source: orb; radius: 18; samples: 19; color: accent }

                Repeater {
                    model: Proxmark.connected ? 2 : 0
                    delegate: Rectangle {
                        anchors.centerIn: parent
                        width: orb.width; height: width; radius: width / 2
                        color: "transparent"
                        border.width: 2; border.color: accent
                        SequentialAnimation on scale {
                            running: true; loops: Animation.Infinite
                            PauseAnimation { duration: index * 700 }
                            NumberAnimation { from: 1; to: 4.2; duration: 1400; easing.type: Easing.OutCubic }
                        }
                        SequentialAnimation on opacity {
                            running: true; loops: Animation.Infinite
                            PauseAnimation { duration: index * 700 }
                            NumberAnimation { from: 0.7; to: 0; duration: 1400; easing.type: Easing.OutCubic }
                        }
                    }
                }
            }

            Column {
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width - Theme.itemSizeMedium - Theme.paddingLarge
                spacing: 2
                Label {
                    text: Proxmark.connected ? "CONNECTED"
                        : Proxmark.present ? "READY" : "NO DEVICE"
                    font.pixelSize: Theme.fontSizeSmall
                    font.bold: true
                    color: accent
                }
                Label {
                    width: parent.width
                    wrapMode: Text.Wrap
                    text: Proxmark.status
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                }
                Label {
                    width: parent.width
                    visible: Proxmark.model.length > 0
                    wrapMode: Text.Wrap
                    text: Proxmark.model
                    font.pixelSize: Theme.fontSizeTiny
                    color: Theme.secondaryHighlightColor
                }
                Label {
                    width: parent.width
                    visible: Proxmark.fwVer.length > 0
                    wrapMode: Text.Wrap
                    text: "FW " + Proxmark.fwVer
                          + (Proxmark.romVer.length > 0 ? "   ·   ROM " + Proxmark.romVer : "")
                    font.pixelSize: Theme.fontSizeTiny
                    color: Theme.secondaryHighlightColor
                }
            }
        }
    }
}
