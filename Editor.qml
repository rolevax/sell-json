import QtQuick 2.0
import sell 1.0

Rectangle {
    id: frame
    width: 800; height: 500
    color: "#222233"
    focus: true

    PDoc { id: pDoc }

    PRawRows {
        id: pRawRows

        onWritten: {
            rawRows.insert(token, r, c);
        }

        onErased: {
            rawRows.erase(br, bc, er, ec);
        }

        onFleshUpdated: {
            rawRows.update(r, c, token);
        }

        onNewLine: {
            rawRows.newLine(r, c);
        }

        onHotLightSet: {
            rawRows.setHotLight(b);
        }

        onLighted: {
            rawRows.light(false, lbr, lbc, ler, lec);
            rawRows.light(true, hbr, hbc, her, hec);
        }
    }

    RawRows {
        id: rawRows
        anchors.fill: parent
    }

    property var doc: "global str"

    Keys.onPressed: {
        pDoc.keyboard(event.modifiers + event.key);
            //changeBox.visible = true;
        event.accepted = true;
    }

    Rectangle {
        id: changeBox
        width: 200; height: 100
        anchors.centerIn: parent
        color: "yellow"
        visible: false
        focus: visible
        Keys.onPressed: {
            if (event.key === Qt.Key_S) {
                rawRows.removeRow();
                rawRows.insertRow("add str");
                visible = false;
                frame.focus = true;
            }
            event.accepted = true;
        }
    }

    Component.onCompleted: {
        pDoc.attachPRawRows(pRawRows);
        pDoc.load();
    }
}

