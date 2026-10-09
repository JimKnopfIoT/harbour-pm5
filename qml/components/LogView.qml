import QtQuick 2.0
import Sailfish.Silica 1.0

// Inline log (timestamped, colored) + export button. Used on every page so the
// activity transcript and export live right where you work. Explanations live
// on each page's attached info page (swipe left).
Column {
    id: lv
    width: parent ? parent.width : 0
    property string exportMsg: ""

    function logLineColor(t) {
        if (t.indexOf("! ") >= 0 || t.indexOf("failed") >= 0 || t.indexOf("EINVARG") >= 0
            || t.indexOf("PIN blocked") >= 0 || t.indexOf("no reply") >= 0)
            return "#ff4d4d"
        if (t.indexOf(">> ") >= 0) return "#4db8ff"
        if (t.indexOf("(OK)") >= 0) return "#28c76f"
        if (t.indexOf("sw=69") >= 0 || t.indexOf("sw=6A") >= 0 || t.indexOf("sw=6a") >= 0
            || t.indexOf("sw=91A") >= 0 || t.indexOf("protected") >= 0 || t.indexOf("not decodable") >= 0
            || t.indexOf("wrong PIN") >= 0 || t.indexOf("No Type") >= 0 || t.indexOf("No 13") >= 0
            || t.indexOf("App-AID") >= 0)
            return "#ff8a00"
        return "#e0e0e0"
    }

    SectionHeader { text: qsTr("Log") }

    Repeater {
        model: Proxmark.logLines
        Label {
            x: Theme.horizontalPageMargin
            width: lv.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.WrapAnywhere
            text: modelData
            font.pixelSize: Theme.fontSizeExtraSmall
            font.family: "monospace"
            color: lv.logLineColor(modelData)
        }
    }
    Label {
        visible: Proxmark.logLines.length === 0
        x: Theme.horizontalPageMargin
        text: qsTr("No activity yet")
        color: Theme.secondaryColor
        font.pixelSize: Theme.fontSizeExtraSmall
    }

    Button {
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Export log")
        enabled: Proxmark.logLines.length > 0 || Proxmark.hfType.length > 0
        onClicked: {
            var d = pageStack.push(Qt.resolvedUrl("ExportDialog.qml"))
            d.accepted.connect(function() {
                lv.exportMsg = d.result && d.result.length > 0
                    ? qsTr("Saved: %1").arg(d.result) : qsTr("Export failed")
            })
        }
    }
    Label {
        visible: lv.exportMsg.length > 0
        x: Theme.horizontalPageMargin
        width: lv.width - 2 * Theme.horizontalPageMargin
        wrapMode: Text.Wrap
        text: lv.exportMsg
        color: Theme.secondaryHighlightColor
        font.pixelSize: Theme.fontSizeExtraSmall
    }
}
