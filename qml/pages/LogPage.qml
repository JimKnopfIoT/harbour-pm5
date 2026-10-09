import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: page
    allowedOrientations: Orientation.All
    property string exportMsg: ""
    SpaceBackground {}

    function logLineColor(t, index) {
        if (t.indexOf("failed") >= 0 || t.indexOf("EINVARG") >= 0 || t.indexOf("error") >= 0
            || t.indexOf("PIN blocked") >= 0)
            return "#ff4d4d"
        if (t.indexOf("sw=69") >= 0 || t.indexOf("sw=6A") >= 0 || t.indexOf("sw=6a") >= 0
            || t.indexOf("protected") >= 0 || t.indexOf("not decodable") >= 0
            || t.indexOf("wrong PIN") >= 0 || t.indexOf("No Type") >= 0 || t.indexOf("No 13") >= 0)
            return "#ff8a00"
        return "#e0e0e0"
    }

    SilicaListView {
        anchors.fill: parent
        PullDownMenu {
            MenuItem {
                text: qsTr("Export log")
                onClicked: {
                    var d = pageStack.push(Qt.resolvedUrl("../components/ExportDialog.qml"))
                    d.accepted.connect(function() {
                        page.exportMsg = d.result && d.result.length > 0
                            ? qsTr("Saved: %1").arg(d.result) : qsTr("Export failed")
                    })
                }
            }
        }
        header: Column {
            width: parent.width
            PageHeader { title: "Log" }
            Label {
                visible: page.exportMsg.length > 0
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                wrapMode: Text.Wrap
                text: page.exportMsg
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }
        }
        model: Proxmark.logLines
        delegate: Label {
            x: Theme.horizontalPageMargin
            width: ListView.view.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: modelData
            font.pixelSize: Theme.fontSizeExtraSmall
            font.family: "monospace"
            color: logLineColor(modelData, index)
        }
        ViewPlaceholder {
            enabled: Proxmark.logLines.length === 0
            text: "No activity yet"
        }
        VerticalScrollDecorator {}
    }
}
