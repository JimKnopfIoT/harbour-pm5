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
            pageStack.pushAttached(Qt.resolvedUrl("LfInfoPage.qml"))
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

            PageHeader { title: "LF · 125 kHz" }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Search badge"
                enabled: !Proxmark.busy
                onClicked: Proxmark.readLf()
            }
            BusyIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
                running: Proxmark.busy; size: BusyIndicatorSize.Medium
            }

            Rectangle {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                height: res.height + 2 * Theme.paddingLarge
                radius: Theme.paddingLarge
                visible: Proxmark.lastTag.length > 0
                color: Qt.rgba(1,1,1,0.04)
                border.width: 1; border.color: Qt.rgba(1,0.54,0,0.5)
                Column {
                    id: res
                    anchors.centerIn: parent
                    width: parent.width - 2 * Theme.paddingLarge
                    spacing: Theme.paddingSmall
                    Label { text: Proxmark.lastTagType; color: page.accent; font.bold: true }
                    Label {
                        width: parent.width; wrapMode: Text.WrapAnywhere
                        text: Proxmark.lastTag
                        font.family: "monospace"; font.pixelSize: Theme.fontSizeLarge
                        color: Theme.primaryColor
                    }
                }
            }

            SectionHeader { text: "Clone to T5577" }
            TextField {
                id: idField
                width: parent.width
                placeholderText: "EM4100 id — 10 hex digits"
                label: "EM4100 id"
                inputMethodHints: Qt.ImhNoPredictiveText
                validator: RegExpValidator { regExp: /[0-9a-fA-F]{0,10}/ }
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: focus = false
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Write T5577"
                enabled: !Proxmark.busy && idField.text.length === 10
                onClicked: writeRemorse.execute("Writing T5577",
                    function() { Proxmark.writeEm4100(idField.text) })
            }

            LogView {}

            Item { width: 1; height: Theme.paddingLarge }
        }
        VerticalScrollDecorator {}
    }
    RemorsePopup { id: writeRemorse }
}
