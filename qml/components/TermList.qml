import QtQuick 2.0
import Sailfish.Silica 1.0

Column {
    property string intro
    property string outro
    property var terms: []
    width: parent ? parent.width : 0
    spacing: Theme.paddingSmall

    Label {
        visible: intro.length > 0
        width: parent.width
        text: intro; wrapMode: Text.Wrap
        color: Theme.secondaryColor; font.pixelSize: Theme.fontSizeExtraSmall
    }
    Repeater {
        model: terms
        Column {
            width: parent.width
            Label { text: modelData.term; color: "#28c76f"
                    font.pixelSize: Theme.fontSizeExtraSmall; font.bold: true }
            Label { width: parent.width; text: modelData.text; wrapMode: Text.Wrap
                    color: Theme.primaryColor; font.pixelSize: Theme.fontSizeExtraSmall }
        }
    }
    Label {
        visible: outro.length > 0
        width: parent.width
        text: outro; wrapMode: Text.Wrap
        color: Theme.secondaryColor; font.pixelSize: Theme.fontSizeExtraSmall; font.italic: true
    }
}
