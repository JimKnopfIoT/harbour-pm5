import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Pickers 1.0

// Choose format, file name and folder before exporting the scan + log.
Dialog {
    id: dlg
    property string folder: StandardPaths.documents
    property string result: ""
    canAccept: fname.text.trim().length > 0

    Column {
        width: parent.width
        spacing: Theme.paddingMedium

        DialogHeader { acceptText: qsTr("Export"); title: qsTr("Export log") }

        ComboBox {
            id: fmt
            width: parent.width
            label: qsTr("Format")
            currentIndex: 0
            property var ext: ["txt", "csv", "json"]
            menu: ContextMenu {
                MenuItem { text: "TXT  (lesbar)" }
                MenuItem { text: "CSV  (Tabelle)" }
                MenuItem { text: "JSON  (weiterverarbeitbar)" }
            }
        }
        TextField {
            id: fname
            width: parent.width
            label: qsTr("File name")
            text: "pm5-" + Qt.formatDateTime(new Date(), "yyyyMMdd-HHmmss")
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            EnterKey.iconSource: "image://theme/icon-m-enter-accept"
            EnterKey.onClicked: if (dlg.canAccept) dlg.accept()
        }
        ValueButton {
            label: qsTr("Folder")
            value: dlg.folder
            onClicked: {
                var p = pageStack.push(folderPicker)
                p.selectedPathChanged.connect(function() { dlg.folder = p.selectedPath })
            }
        }
        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.WrapAnywhere
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
            text: qsTr("→ %1/%2.%3").arg(dlg.folder).arg(fname.text.trim())
                  .arg(fmt.ext[fmt.currentIndex])
        }
    }

    Component {
        id: folderPicker
        FolderPickerPage { dialogTitle: qsTr("Pick a folder") }
    }

    onAccepted: {
        var f = fmt.ext[fmt.currentIndex]
        var path = dlg.folder + "/" + fname.text.trim() + "." + f
        dlg.result = Proxmark.exportLog(f, path)
    }
}
