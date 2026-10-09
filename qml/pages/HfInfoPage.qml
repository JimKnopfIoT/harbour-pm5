import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

// Attached page (right of HfPage): the "what the reader does" notes and the
// glossary. Reached by swiping left; Silica draws the forward page indicator.
Page {
    allowedOrientations: Orientation.All

    SpaceBackground {}

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: col.height

        Column {
            id: col
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader { title: "HF · Info" }

            InfoBlock {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                title: qsTr("What the HF reader does")
                works: [
                    qsTr("Activates any ISO 14443-A card: UID/NFCID1, ATQA and SAK, and the ATS of a 14443-4 card"),
                    qsTr("Names the payment applications (PPSE, then known AIDs) and their labels"),
                    qsTr("On a payment card, reads the on-card transaction log when the card releases it"),
                    qsTr("Tells MIFARE Classic apart from a smart card by its SAK — the MIFARE page then attacks it")
                ]
                blocked: [
                    qsTr("Nothing secret. Keys stay in the card; a payment card cannot be cloned or replayed from a read.")
                ]
                footnote: qsTr("A UID starting with 08 is random and regenerated each session — it is not the card and buys nothing at a terminal.")
            }

            Glossary {
                x: Theme.horizontalPageMargin
                width: col.width - 2 * Theme.horizontalPageMargin
            }

            Item { width: 1; height: Theme.paddingLarge }
        }
        VerticalScrollDecorator {}
    }
}
