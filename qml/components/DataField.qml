import QtQuick 2.0
import Sailfish.Silica 1.0

// One label/value row. Long values fold away and expand on tap.
Column {
    id: field
    property string label
    property string value
    property bool mono: false
    // 0 normal · 1 reachable but locked / needs more (orange) · 2 error (red)
    property int level: 0
    // non-empty -> the row is tappable and emits activated() (e.g. "pin")
    property string action: ""
    signal activated()

    readonly property color valueColor: level === 2 ? "#ff4d4d"
                                       : level === 1 ? "#ff8a00"
                                       : Theme.primaryColor

    readonly property int foldAt: 128
    readonly property bool foldable: value.length > foldAt
    property bool expanded: false

    width: parent ? parent.width : 0
    spacing: 0

    Label {
        width: parent.width
        text: field.label + (field.level > 0 ? "   ·  dig deeper" : "")
        color: field.level > 0 ? field.valueColor : Theme.secondaryHighlightColor
        font.pixelSize: Theme.fontSizeExtraSmall
        truncationMode: TruncationMode.Fade
    }
    Label {
        width: parent.width
        text: (field.foldable && !field.expanded)
              ? field.value.substring(0, field.foldAt) + "…" : field.value
        wrapMode: mono ? Text.WrapAnywhere : Text.Wrap
        color: field.valueColor
        font.pixelSize: mono ? Theme.fontSizeExtraSmall : Theme.fontSizeSmall
        font.family: mono ? "monospace" : Theme.fontFamily
        MouseArea { anchors.fill: parent; enabled: field.action !== "" || field.foldable
            onClicked: field.action !== "" ? field.activated() : (field.expanded = !field.expanded) }
    }
    Label {
        visible: field.foldable
        text: field.expanded ? qsTr("tap to fold")
              : qsTr("%1 characters · tap to expand").arg(field.value.length)
        color: Theme.secondaryColor
        font.pixelSize: Theme.fontSizeExtraSmall
        font.italic: true
    }
}
