import QtQuick 2.0
import Sailfish.Silica 1.0

// Offline-PIN entry. Half-blind (echo hidden), no predictive text, not kept in
// any clipboard/cache, cleared the moment the dialog closes. The PIN is handed
// straight to the backend and never logged.
Dialog {
    id: dlg
    canAccept: pin.text.length >= 4

    Column {
        width: parent.width
        spacing: Theme.paddingMedium

        DialogHeader { acceptText: qsTr("Try PIN"); title: qsTr("Offline PIN") }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            color: "#ff8a00"
            font.pixelSize: Theme.fontSizeExtraSmall
            text: qsTr("A wrong PIN spends one of the card's few tries. The app sends it only while at least 2 tries remain, so it can never block the card — but repeated wrong entries count the card down. Nothing is shown, logged or kept.")
        }

        TextField {
            id: pin
            width: parent.width
            echoMode: TextInput.Password
            inputMethodHints: Qt.ImhHiddenText | Qt.ImhNoPredictiveText
                              | Qt.ImhNoAutoUppercase | Qt.ImhSensitiveData | Qt.ImhDigitsOnly
            label: qsTr("PIN (half-blind)")
            placeholderText: qsTr("PIN")
            EnterKey.iconSource: "image://theme/icon-m-enter-accept"
            EnterKey.onClicked: if (dlg.canAccept) dlg.accept()
        }
    }

    onDone: {
        if (result === DialogResult.Accepted)
            Proxmark.readLogWithPin(pin.text)
        pin.text = ""   // scrub the entered digits
    }
}
