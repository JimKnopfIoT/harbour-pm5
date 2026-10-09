import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

// Raw terminal: send a custom APDU to the card, or an arbitrary wire command
// (NG/MIX), and watch the reply in the shared log (timestamped, colored,
// exportable). This is NOT the proxmark3 client CLI — it is the wire level.
Page {
    id: page
    allowedOrientations: Orientation.All
    property string exportMsg: ""

    // Ready-made commands (order matches the ComboBox menu below).
    property var presets: [
        { hex: "",           apdu: true,  hold: false },
        { hex: "9060000000", apdu: true,  hold: true  },
        { hex: "90AF000000", apdu: true,  hold: true  },
        { hex: "906A000000", apdu: true,  hold: true  },
        { hex: "905A00000300000000", apdu: true, hold: true },
        { hex: "906F000000", apdu: true,  hold: true  },
        { hex: "90F50000010000", apdu: true, hold: true },
        { hex: "906E000000", apdu: true,  hold: true  },
        { hex: "9045000000", apdu: true,  hold: true  },
        { hex: "90640000010000", apdu: true, hold: true },
        { hex: "00A404000E325041592E5359532E444446303100", apdu: true, hold: false },
        { hex: "00A40000023F0000", apdu: true, hold: false },
        { hex: "0107", apdu: false, hold: false },
        { hex: "0112", apdu: false, hold: false }
    ]

    function applyPreset(i) {
        var p = presets[i]
        if (!p || p.hex.length === 0) return
        mode.currentIndex = p.apdu ? 0 : 1
        hold.checked = p.hold
        input.text = p.hex
    }

    function logLineColor(t) {
        if (t.indexOf("! ") >= 0 || t.indexOf("failed") >= 0 || t.indexOf("no reply") >= 0
            || t.indexOf("EINVARG") >= 0)
            return "#ff4d4d"
        if (t.indexOf(">> ") >= 0) return "#4db8ff"
        if (t.indexOf("sw=90") >= 0 || t.indexOf("sw=91  ") >= 0 || t.indexOf("(OK)") >= 0) return "#28c76f"
        if (t.indexOf("sw=69") >= 0 || t.indexOf("sw=6A") >= 0 || t.indexOf("sw=6a") >= 0
            || t.indexOf("sw=91A") >= 0 || t.indexOf("App-AID") >= 0)
            return "#ff8a00"
        return "#e0e0e0"
    }

    function send() {
        var s = input.text.replace(/\s+/g, " ").trim()
        if (s.length === 0) return
        if (mode.currentIndex === 0) {
            Proxmark.rawApdu(s, hold.checked)
        } else {
            var parts = s.split(" ")
            var cmd = parts.shift()
            Proxmark.rawCmd(cmd, parts.join(""), mode.currentIndex === 1)
        }
    }

    SpaceBackground {}

    Column {
        id: controls
        anchors { top: parent.top; left: parent.left; right: parent.right }
        spacing: Theme.paddingSmall

        PageHeader { title: "Terminal" }

        ComboBox {
            id: preset
            width: parent.width
            label: qsTr("Quick command")
            currentIndex: 0
            onCurrentIndexChanged: page.applyPreset(currentIndex)
            menu: ContextMenu {
                MenuItem { text: "—  (frei eingeben)" }
                MenuItem { text: "DESFire · GetVersion" }
                MenuItem { text: "DESFire · weiter (AF)" }
                MenuItem { text: "DESFire · Apps auflisten" }
                MenuItem { text: "DESFire · App wählen (AID!)" }
                MenuItem { text: "DESFire · Dateien auflisten" }
                MenuItem { text: "DESFire · FileSettings (File!)" }
                MenuItem { text: "DESFire · freier Speicher" }
                MenuItem { text: "DESFire · GetKeySettings" }
                MenuItem { text: "DESFire · GetKeyVersion (Key!)" }
                MenuItem { text: "EMV · SELECT PPSE" }
                MenuItem { text: "ISO · SELECT MF (3F00)" }
                MenuItem { text: "PM5 · Version (CMD)" }
                MenuItem { text: "PM5 · Capabilities (CMD)" }
            }
        }
        ComboBox {
            id: mode
            width: parent.width
            label: qsTr("Mode")
            currentIndex: 0
            menu: ContextMenu {
                MenuItem { text: "APDU" }
                MenuItem { text: "CMD (NG)" }
                MenuItem { text: "CMD (MIX)" }
            }
        }
        TextSwitch {
            id: hold
            visible: mode.currentIndex === 0
            text: qsTr("Hold session")
            description: qsTr("Keep one activation across APDUs (SELECT → GPO → …)")
        }
        TextField {
            id: input
            width: parent.width
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            font.family: "monospace"
            label: mode.currentIndex === 0 ? qsTr("APDU (hex)") : qsTr("cmd-id [payload] (hex)")
            placeholderText: mode.currentIndex === 0
                ? "00A4040007A0000000031010" : "0385  <payload>"
            EnterKey.iconSource: "image://theme/icon-m-enter-accept"
            EnterKey.onClicked: page.send()
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Send")
            enabled: Proxmark.connected && !Proxmark.busy && input.text.length > 0
            onClicked: page.send()
        }
        Label {
            visible: page.exportMsg.length > 0
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: page.exportMsg
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }
        SectionHeader { text: qsTr("Output") }
    }

    SilicaListView {
        anchors { top: controls.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        clip: true
        model: Proxmark.logLines
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
        delegate: Label {
            x: Theme.horizontalPageMargin
            width: page.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.WrapAnywhere
            text: modelData
            font.pixelSize: Theme.fontSizeExtraSmall
            font.family: "monospace"
            color: page.logLineColor(modelData)
        }
        VerticalScrollDecorator {}
    }
}
