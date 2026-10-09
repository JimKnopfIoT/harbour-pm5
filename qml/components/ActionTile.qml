import QtQuick 2.0
import Sailfish.Silica 1.0
import QtGraphicalEffects 1.0

// Neon-bordered touch tile. glyph + title + subtitle, tinted by accent.
BackgroundItem {
    id: tile
    property string glyph
    property string title
    property string subtitle
    property color accent: "#ff8a00"
    property bool busy: false
    property bool soon: false

    height: Theme.itemSizeHuge * 1.15

    Rectangle {
        id: card
        anchors { fill: parent; margins: Theme.paddingSmall }
        radius: Theme.paddingLarge
        opacity: tile.soon ? 0.4 : 1.0
        color: tile.highlighted ? Qt.rgba(accent.r, accent.g, accent.b, 0.14)
                                : Qt.rgba(1, 1, 1, 0.04)
        border.width: 1
        border.color: Qt.rgba(accent.r, accent.g, accent.b, tile.highlighted ? 0.9 : 0.45)

        Label {
            visible: tile.soon
            anchors { top: parent.top; right: parent.right; margins: Theme.paddingSmall }
            text: qsTr("geplant")
            font.pixelSize: Theme.fontSizeTiny
            color: accent
        }

        Column {
            anchors.centerIn: parent
            width: parent.width - 2 * Theme.paddingLarge
            spacing: Theme.paddingSmall

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: tile.glyph
                font.pixelSize: Theme.fontSizeExtraLarge * 1.3
                color: accent
                visible: !tile.busy
            }
            BusyIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
                running: tile.busy
                visible: tile.busy
                size: BusyIndicatorSize.Medium
            }
            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: tile.title
                font.pixelSize: Theme.fontSizeMedium
                font.bold: true
                color: Theme.primaryColor
            }
            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                text: tile.subtitle
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.secondaryColor
                visible: text.length > 0
            }
        }
    }
    // soft outer glow when pressed
    Glow {
        anchors.fill: card
        source: card
        radius: 16
        samples: 17
        color: accent
        opacity: tile.highlighted ? 0.5 : 0.0
        Behavior on opacity { NumberAnimation { duration: 160 } }
    }
}
