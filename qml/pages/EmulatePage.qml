import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

// UID emulation: present a chosen UID as a 14443-A tag. Only fools readers that
// trust the UID; DESFire crypto-auth readers are not fooled (needs the key +
// full crypto emulation, which this app does not do).
Page {
    id: page
    allowedOrientations: Orientation.All
    property color accent: "#28c76f"

    SpaceBackground {}

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: col.height

        Column {
            id: col
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader { title: "Emulate · UID" }

            TextField {
                id: uidField
                width: parent.width
                label: qsTr("UID (hex, 4 / 7 / 10 bytes)")
                placeholderText: "04861EB2EC4B80"
                text: Proxmark.hfUid.replace(/:/g, "")
                font.family: "monospace"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
                validator: RegExpValidator { regExp: /[0-9a-fA-F]{0,20}/ }
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: focus = false
            }
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Theme.paddingMedium
                Button {
                    text: qsTr("Start")
                    enabled: Proxmark.connected && (uidField.text.length === 8
                             || uidField.text.length === 14 || uidField.text.length === 20)
                    onClicked: Proxmark.simulateUid(uidField.text)
                }
                Button {
                    text: qsTr("Stop")
                    enabled: Proxmark.connected
                    onClicked: Proxmark.stopEmulation()
                }
            }

            InfoBlock {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                title: qsTr("What UID emulation does")
                works: [
                    qsTr("Presents the chosen UID as a 14443-A tag (DESFire for a 7-byte UID)"),
                    qsTr("Enough for readers that only check the UID")
                ]
                blocked: [
                    qsTr("A reader with real DESFire crypto auth (e.g. your time system) is NOT fooled by the UID — that needs the card's key and full crypto emulation, which this app does not do.")
                ]
                footnote: qsTr("Start, then hold the PM5 to the reader; press Stop to end.")
            }

            LogView {}

            Item { width: 1; height: Theme.paddingLarge }
        }
        VerticalScrollDecorator {}
    }
}
