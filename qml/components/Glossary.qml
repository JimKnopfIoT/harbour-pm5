import QtQuick 2.0
import Sailfish.Silica 1.0

// Reference for the raw values on the HF page. Folded away, within reach.
Column {
    width: parent ? parent.width : 0
    spacing: 0

    SectionHeader { text: qsTr("Reference") }

    ExpandingSectionGroup {
        width: parent.width

        ExpandingSection {
            title: qsTr("Anticollision: NFCID1 and SAK")
            content.sourceComponent: Component {
                TermList {
                    intro: qsTr("What a card reveals before any application is selected — the layer the Proxmark reaches on every card.")
                    outro: qsTr("SAK is the fastest tell: bit 0x20 means ISO 14443-4 and an ATS follows, 0x08/0x18 mean MIFARE Classic, 0x00 a Type 2 tag.")
                    terms: [
                        { term: "NFCID1 / UID", text: qsTr("The anticollision identifier, 4/7/10 bytes (single/double/triple). A 4-byte value starting with 08 is not an identity — it is randomly generated per session, which payment cards, passports and identity cards all do on purpose.") },
                        { term: "ATQA / SENS_RES", text: qsTr("The answer to the request command. Its top two bits encode the UID size; the low bits are the bit-frame anticollision setting.") },
                        { term: "SAK / SEL_RES", text: qsTr("One byte, the answer to being singled out. 0x20 = speaks ISO 14443-4, ATS follows; 0x40 = NFC-DEP; 0x04 = UID not complete, another cascade level follows.") },
                        { term: "00", text: qsTr("Type 2 tag — MIFARE Ultralight, NTAG213/215/216.") },
                        { term: "08 / 09 / 18", text: qsTr("MIFARE Classic 1K / Mini / 4K — Crypto1, reachable with the Proxmark (see the MIFARE page).") },
                        { term: "20", text: qsTr("Plain ISO 14443-4: DESFire, bank cards, identity documents.") },
                        { term: "28 / 38", text: qsTr("A Classic part plus an ISO 14443-4 part at once.") }
                    ]
                }
            }
        }

        ExpandingSection {
            title: qsTr("Reading the ATS")
            content.sourceComponent: Component {
                TermList {
                    intro: qsTr("The Answer To Select, with which an ISO 14443-4 card introduces itself after activation. It says how to talk to the card, not what is on it.")
                    outro: qsTr("FSCI and FWI matter most: a small frame with a long waiting time is what makes probing slow.")
                    terms: [
                        { term: "T0", text: qsTr("Format byte. Low nibble = FSCI, the largest frame the card accepts (0→16 … 8→256 byte). Upper bits flag which of TA/TB/TC follow.") },
                        { term: "TA(1)", text: qsTr("Transmission rates: which multiples of 106 kbit/s (212/424/848) the card supports, per direction.") },
                        { term: "TB(1)", text: qsTr("Timing. Upper nibble FWI sets how long the card may take to answer (FWT ≈ 302 µs × 2^FWI); lower nibble SFGI the grace period after activation.") },
                        { term: "TC(1)", text: qsTr("Protocol options: whether the card supports CID (card identifier) and NAD (node addressing).") }
                    ]
                }
            }
        }

        ExpandingSection {
            title: qsTr("EMV / payment applications")
            content.sourceComponent: Component {
                TermList {
                    intro: qsTr("A contactless bank card is an ISO 14443-4 smart card. The reader selects applications by identifier and reads their public directory.")
                    outro: qsTr("Nothing secret is readable: the keys never leave the card's secure element, and every real transaction needs a fresh cryptogram over an unpredictable terminal number.")
                    terms: [
                        { term: "PPSE", text: qsTr("2PAY.SYS.DDF01 — the Proximity Payment System Environment. A directory listing the applications the card offers. German girocards often omit it, so known AIDs are tried directly.") },
                        { term: "AID", text: qsTr("Application Identifier, e.g. A0000000041010 = Mastercard, A0000000031010 = Visa, D276… = girocard.") },
                        { term: "Application Label (50)", text: qsTr("A human-readable name the card gives its application, e.g. \"MASTERCARD\" or \"Debit\".") },
                        { term: "FCI", text: qsTr("File Control Information — the response to selecting an application, a BER-TLV structure holding the label, priorities and processing options.") }
                    ]
                }
            }
        }

        ExpandingSection {
            title: qsTr("Status words")
            content.sourceComponent: Component {
                TermList {
                    intro: qsTr("Every APDU answer ends in two bytes, SW1 SW2. A card that refuses is not a card that is absent.")
                    outro: qsTr("6A82 means the application is genuinely not there. 6982 means it is there and will not talk — what a German eID says to any terminal without an authorisation certificate.")
                    terms: [
                        { term: "9000", text: qsTr("Success.") },
                        { term: "6A82", text: qsTr("File or application not found. Treated as absent.") },
                        { term: "6982", text: qsTr("Security status not satisfied — present, but authentication is missing.") },
                        { term: "6985", text: qsTr("Conditions of use not satisfied — right command, wrong moment.") },
                        { term: "6300", text: qsTr("Verification failed; a PIN retry counter just went down.") },
                        { term: "61xx", text: qsTr("Success, xx more bytes waiting — fetch with GET RESPONSE.") },
                        { term: "6Cxx", text: qsTr("Wrong Le; repeat with xx as the expected length.") }
                    ]
                }
            }
        }
    }
}
