import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: page
    allowedOrientations: Orientation.All
    property color accent: "#28c76f"

    // Attach the info page to the right once, so Silica shows the forward
    // page indicator (top right) and a left-swipe reaches the explanations.
    property bool _attached: false
    onStatusChanged: {
        if (status === PageStatus.Active && !_attached) {
            _attached = true
            pageStack.pushAttached(Qt.resolvedUrl("HfInfoPage.qml"))
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

            PageHeader { title: "HF · 13.56 MHz" }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Read card"
                enabled: !Proxmark.busy
                onClicked: Proxmark.readHf()
            }
            BusyIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
                running: Proxmark.busy; size: BusyIndicatorSize.Medium
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                visible: Proxmark.hfType.length > 0
                text: Proxmark.hfType
                color: page.accent
                font.pixelSize: Theme.fontSizeLarge
                font.bold: true
                wrapMode: Text.Wrap
            }

            // decoded fields
            Column {
                width: parent.width
                spacing: Theme.paddingMedium
                Repeater {
                    model: Proxmark.hfFields
                    DataField {
                        x: Theme.horizontalPageMargin
                        width: col.width - 2 * Theme.horizontalPageMargin
                        label: modelData.label
                        value: modelData.value
                        mono: modelData.mono
                        level: modelData.level === undefined ? 0 : modelData.level
                        action: modelData.action === undefined ? "" : modelData.action
                        onActivated: if (action === "pin")
                            pageStack.push(Qt.resolvedUrl("../components/PinDialog.qml"))
                    }
                }
            }

            LogView {}

            Item { width: 1; height: Theme.paddingLarge }
        }
        VerticalScrollDecorator {}
    }
}
