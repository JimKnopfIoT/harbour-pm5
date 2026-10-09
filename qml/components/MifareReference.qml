import QtQuick 2.0
import Sailfish.Silica 1.0

Column {
    width: parent ? parent.width : 0
    spacing: 0
    SectionHeader { text: qsTr("Reference") }
    ExpandingSectionGroup {
        width: parent.width
        ExpandingSection {
            title: qsTr("Sectors, blocks, keys")
            content.sourceComponent: Component {
                TermList {
                    intro: qsTr("MIFARE Classic 1K is 16 sectors of 4 blocks (16 byte each). Each sector is locked by its own two keys.")
                    outro: qsTr("A sector opens only with the right key; the check tries a dictionary of well-known defaults.")
                    terms: [
                        { term: "Sector / Block", text: qsTr("1K = 16 sectors × 4 blocks. Block 0 holds the UID; the last block of each sector is its trailer.") },
                        { term: "Sector trailer", text: qsTr("The 4th block: Key A (6 byte) · access bits (4) · Key B (6). Key A never reads back — the dump rebuilds it from the found key.") },
                        { term: "Key A / Key B", text: qsTr("Two keys per sector. Access bits decide what each may do — often A reads, B reads and writes.") },
                        { term: "Crypto1", text: qsTr("MIFARE's proprietary 48-bit cipher. Broken since 2008; the phone's own NFC stack still refuses these cards, the Proxmark does not.") },
                        { term: "Default keys", text: qsTr("Factory/common keys like FFFFFFFFFFFF or A0A1A2A3A4A5. Many cheap cards are never re-keyed and open straight away.") }
                    ]
                }
            }
        }
        ExpandingSection {
            title: qsTr("If default keys fail")
            content.sourceComponent: Component {
                TermList {
                    intro: qsTr("A card with custom keys will not open from the dictionary.")
                    terms: [
                        { term: "nested / hardnested", text: qsTr("Statistical attacks that recover an unknown key from the card's own weak randomness. The firmware collects the nonces; the key search is heavy host work (planned).") },
                        { term: "Magic card", text: qsTr("A special blank whose block 0 / UID is writable — the target a dump is cloned onto (planned).") }
                    ]
                }
            }
        }
    }
}
