import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: page
    allowedOrientations: Orientation.All

    SpaceBackground {}

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: content.height

        PullDownMenu {
            MenuItem {
                text: "Log"
                onClicked: pageStack.push(Qt.resolvedUrl("LogPage.qml"))
            }
            MenuItem {
                text: Proxmark.connected ? "Disconnect" : "Connect"
                onClicked: Proxmark.connected ? Proxmark.disconnect() : Proxmark.connectDevice()
            }
        }

        Column {
            id: content
            width: page.width
            spacing: Theme.paddingLarge

            PageHeader { title: "PM5" }

            StatusCard {}

            Item { width: 1; height: Theme.paddingMedium }

            Grid {
                x: Theme.horizontalPageMargin
                width: page.width - 2 * Theme.horizontalPageMargin
                columns: 2
                property real cellW: (width - Theme.paddingMedium) / 2

                ActionTile {
                    width: parent.cellW
                    glyph: "◎"; title: "LF 125 kHz"
                    subtitle: "EM4100 · HID · IO · T5577"
                    accent: "#ff8a00"
                    enabled: Proxmark.connected && !Proxmark.busy
                    opacity: enabled ? 1 : 0.4
                    onClicked: pageStack.push(Qt.resolvedUrl("LfPage.qml"))
                }
                ActionTile {
                    width: parent.cellW
                    glyph: "⌽"; title: "HF 13.56 MHz"
                    subtitle: "UID · ATQA · SAK"
                    accent: "#28c76f"
                    enabled: Proxmark.connected && !Proxmark.busy
                    opacity: enabled ? 1 : 0.4
                    onClicked: pageStack.push(Qt.resolvedUrl("HfPage.qml"))
                }
                ActionTile {
                    width: parent.cellW
                    glyph: "⊞"; title: "MIFARE"
                    subtitle: "Keys · Dump"
                    accent: "#ff8a00"
                    enabled: Proxmark.connected && !Proxmark.busy
                    opacity: enabled ? 1 : 0.4
                    onClicked: pageStack.push(Qt.resolvedUrl("MifarePage.qml"))
                }
                ActionTile { width: parent.cellW; soon: true; enabled: false
                    glyph: "≋"; title: "ISO 15693"; subtitle: qsTr("Vicinity · SLIX"); accent: "#28c76f" }
                ActionTile { width: parent.cellW; soon: true; enabled: false
                    glyph: "◈"; title: "iCLASS"; subtitle: qsTr("Picopass"); accent: "#ff8a00" }
                ActionTile { width: parent.cellW; soon: true; enabled: false
                    glyph: "⌗"; title: "ISO 14443-B"; subtitle: qsTr("Typ B"); accent: "#28c76f" }
                ActionTile { width: parent.cellW; soon: true; enabled: false
                    glyph: "❈"; title: "FeliCa"; subtitle: qsTr("NFC-F"); accent: "#ff8a00" }
                ActionTile { width: parent.cellW; soon: true; enabled: false
                    glyph: "◇"; title: "Legic"; subtitle: qsTr("Prime"); accent: "#28c76f" }
                ActionTile { width: parent.cellW; soon: true; enabled: false
                    glyph: "⟟"; title: "Hitag"; subtitle: qsTr("LF · Hitag2/S"); accent: "#ff8a00" }
                ActionTile { width: parent.cellW; soon: true; enabled: false
                    glyph: "▤"; title: "T55xx"; subtitle: qsTr("LF · read/write"); accent: "#28c76f" }
                ActionTile { width: parent.cellW; soon: true; enabled: false
                    glyph: "◧"; title: "EM4x05/50/70"; subtitle: qsTr("LF"); accent: "#ff8a00" }
                ActionTile { width: parent.cellW; soon: true; enabled: false
                    glyph: "≣"; title: "UHF"; subtitle: qsTr("860–960 MHz · Roadmap"); accent: "#28c76f" }
                ActionTile { width: parent.cellW; soon: true; enabled: false
                    glyph: "❂"; title: "Sniff"; subtitle: qsTr("HF/LF Trace"); accent: "#ff8a00" }

                ActionTile {
                    width: parent.cellW
                    glyph: "›_"; title: "Terminal"
                    subtitle: "APDU · raw cmd"
                    accent: "#28c76f"
                    enabled: Proxmark.connected && !Proxmark.busy
                    opacity: enabled ? 1 : 0.4
                    onClicked: pageStack.push(Qt.resolvedUrl("TerminalPage.qml"))
                }
                ActionTile {
                    width: parent.cellW
                    glyph: "◐"; title: "Emulate"
                    subtitle: "UID (14443-A)"
                    accent: "#28c76f"
                    enabled: Proxmark.connected && !Proxmark.busy
                    opacity: enabled ? 1 : 0.4
                    onClicked: pageStack.push(Qt.resolvedUrl("EmulatePage.qml"))
                }
            }

            Label {
                visible: !Proxmark.connected
                x: Theme.horizontalPageMargin
                width: page.width - 2 * Theme.horizontalPageMargin
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
                text: Proxmark.present ? "Pull down to connect the Proxmark."
                                       : "Connect the Proxmark over USB."
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
            }
        }
        VerticalScrollDecorator {}
    }
}
