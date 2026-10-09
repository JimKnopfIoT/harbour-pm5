import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: page
    allowedOrientations: Orientation.All
    property color accent: "#ff8a00"

    property bool _attached: false
    onStatusChanged: {
        if (status === PageStatus.Active && !_attached) {
            _attached = true
            pageStack.pushAttached(Qt.resolvedUrl("MifaInfoPage.qml"))
        }
    }

    SpaceBackground {}

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: col.height

        Column {
            id: col
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader { title: "MIFARE Classic" }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Theme.paddingMedium
                Button {
                    text: "Check keys"
                    enabled: !Proxmark.busy
                    onClicked: Proxmark.checkMifareKeys()
                }
                Button {
                    text: "Dump"
                    enabled: !Proxmark.busy && Proxmark.mfFound > 0
                    onClicked: Proxmark.dumpMifare()
                }
            }
            BusyIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
                running: Proxmark.busy; size: BusyIndicatorSize.Medium
            }

            DetailItem {
                visible: Proxmark.mfResult.length > 0
                label: "Keys"; value: Proxmark.mfResult
            }

            SectionHeader { visible: Proxmark.mfDump.length > 0; text: "Dump · " + Proxmark.mfDumpBlocks + " blocks" }
            Rectangle {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                height: dump.height + 2 * Theme.paddingMedium
                radius: Theme.paddingMedium
                visible: Proxmark.mfDump.length > 0
                color: Qt.rgba(1,1,1,0.04)
                border.width: 1; border.color: Qt.rgba(0.15,0.78,0.43,0.5)
                Label {
                    id: dump
                    anchors { left: parent.left; right: parent.right; top: parent.top
                              margins: Theme.paddingMedium }
                    text: Proxmark.mfDump
                    font.family: "monospace"; font.pixelSize: Theme.fontSizeTiny
                    color: Theme.primaryColor
                    wrapMode: Text.WrapAnywhere
                }
            }

            LogView {}

            Item { width: 1; height: Theme.paddingLarge }
        }
        VerticalScrollDecorator {}
    }
}
