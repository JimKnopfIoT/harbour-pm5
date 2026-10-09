import QtQuick 2.0
import Sailfish.Silica 1.0

Column {
    width: parent ? parent.width : 0
    spacing: 0
    SectionHeader { text: qsTr("Reference") }
    ExpandingSectionGroup {
        width: parent.width
        ExpandingSection {
            title: qsTr("125 kHz formats")
            content.sourceComponent: Component {
                TermList {
                    intro: qsTr("Low-frequency badges carry a fixed number and no cryptography — which is why they clone onto a blank so easily.")
                    terms: [
                        { term: "EM4100", text: qsTr("40-bit read-only id, modulated RF/64. The classic building/fob transponder. Clones onto a T5577.") },
                        { term: "HID Prox", text: qsTr("HID 125 kHz Wiegand: a facility code plus a card number. Read-only in the tag.") },
                        { term: "IO Prox", text: qsTr("Kantech ioProx, same idea as HID Prox with a different frame.") },
                        { term: "T5577", text: qsTr("A programmable 125 kHz transponder that can emulate EM4100, HID and others. This is the blank the reader writes a cloned id onto.") },
                        { term: "RF/64", text: qsTr("The bit rate: one data bit per 64 carrier cycles. EM4100's standard clock.") }
                    ]
                }
            }
        }
    }
}
