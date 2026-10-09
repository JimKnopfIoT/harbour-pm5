import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

// Attached page (right of LfPage): what the LF reader does + the LF reference.
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

            PageHeader { title: "LF · Info" }

            InfoBlock {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                title: qsTr("What the LF reader does")
                works: [
                    qsTr("Reads EM4100, HID Prox and IO Prox 125 kHz badges"),
                    qsTr("Clones an EM4100 id onto a blank T5577 — a real duplicate")
                ]
                blocked: [
                    qsTr("Encrypted LF (e.g. Hitag with keys) is not cloned; that is the Hitag page (planned).")
                ]
            }

            LfReference {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
            }

            Item { width: 1; height: Theme.paddingLarge }
        }
        VerticalScrollDecorator {}
    }
}
