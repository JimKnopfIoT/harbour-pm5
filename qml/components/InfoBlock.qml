import QtQuick 2.0
import Sailfish.Silica 1.0

Column {
    property string title
    property var works: []
    property var blocked: []
    property string footnote
    width: parent ? parent.width : 0
    spacing: Theme.paddingMedium

    SectionHeader { text: title }

    Column {
        width: parent.width; spacing: Theme.paddingSmall; visible: works.length > 0
        Repeater {
            model: works
            Row {
                width: parent.width; spacing: Theme.paddingSmall
                Label { text: "✓"; color: "#28c76f"; font.pixelSize: Theme.fontSizeSmall }
                Label { width: parent.width - Theme.paddingSmall - Theme.horizontalPageMargin
                        text: modelData; wrapMode: Text.Wrap
                        color: Theme.primaryColor; font.pixelSize: Theme.fontSizeSmall }
            }
        }
    }
    Column {
        width: parent.width; spacing: Theme.paddingSmall; visible: blocked.length > 0
        Repeater {
            model: blocked
            Row {
                width: parent.width; spacing: Theme.paddingSmall
                Label { text: "✗"; color: "#ff8a00"; font.pixelSize: Theme.fontSizeSmall }
                Label { width: parent.width - Theme.paddingSmall - Theme.horizontalPageMargin
                        text: modelData; wrapMode: Text.Wrap
                        color: Theme.secondaryColor; font.pixelSize: Theme.fontSizeSmall }
            }
        }
    }
    Label {
        width: parent.width; visible: footnote.length > 0
        text: footnote; wrapMode: Text.Wrap
        color: Theme.secondaryColor; font.pixelSize: Theme.fontSizeExtraSmall; font.italic: true
    }
}
