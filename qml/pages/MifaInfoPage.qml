import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

// Attached page (right of MifarePage): what the MIFARE tools do + reference.
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

            PageHeader { title: "MIFARE · Info" }

            InfoBlock {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                title: qsTr("What the MIFARE tools do")
                works: [
                    qsTr("Check every sector against a dictionary of default keys"),
                    qsTr("Dump all blocks with the found keys, rebuilding the trailers")
                ]
                blocked: [
                    qsTr("A card with custom keys will not open — that needs nested/hardnested (planned)."),
                    qsTr("Writing/cloning to a magic card is planned.")
                ]
            }

            MifareReference {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
            }

            Item { width: 1; height: Theme.paddingLarge }
        }
        VerticalScrollDecorator {}
    }
}
