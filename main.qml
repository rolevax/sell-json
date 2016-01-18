import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

Window {
    visible: true
    title: "SELL-JSON"
    width: 800; height: 530
    color: "#444444"

    Button {
        id: openButton
        text: "Open"
        onClicked: {
            fileDialog.open();
        }
    }

    Text {
        anchors.left: openButton.right
        anchors.leftMargin: 10
        anchors.verticalCenter: openButton.verticalCenter
        text: editor.filename
        color: "white"
    }

    Button {
        id: saveButton
        text: "Save"
        anchors.right: parent.right
        onClicked: {
            editor.save();
        }
    }


    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: [ "JSON files (*.json)", "All files (*)" ]
        onAccepted: {
            // slice() to get rid of "file://" prefix
            editor.filename = fileUrl.toString().slice(7);
        }
    }


    Editor {
        id: editor
        anchors.bottom: parent.bottom
    }
}
