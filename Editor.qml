import QtQuick 2.0
import sell 1.0

Rectangle {
    id: frame
    width: 800; height: 500
    color: "#222233"
    focus: true

    PDoc {
        id: pDoc

        onMenu: {
            if (text === "") {
                menu.visible = false;
            } else {
                menuText.text = text;
                menu.visible = true;
            }
        }

        onPushed: {
            modesModel.append({ modelText: name });
        }

        onPopped: {
            modesModel.remove(modesModel.count - 1, 1);
        }
    }

    PRawRows {
        id: pRawRows

        onWritten: {
            rawRows.insert(str, r, c);
        }

        onErased: {
            rawRows.erase(br, bc, er, ec);
        }

        onFleshUpdated: {
            rawRows.update(r, bc, ec, str);
        }

        onNewLine: {
            rawRows.newLine(r, c);
        }

        onJoinLine: {
            rawRows.joinLine(r);
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

    ListView {
        id: modes
        model: ListModel {
            id: modesModel
        }

        width: 130
        height: parent.height
        anchors.right: parent.right
        verticalLayoutDirection: ListView.BottomToTop
        delegate: Rectangle {
            width: parent.width
            height: 35
            color: "yellow"
            Text {
                anchors.centerIn: parent
                text: modelText
                font.pixelSize: 20
            }
        }
    }

    Keys.onPressed: {
        pDoc.keyboard(event.text, event.modifiers);
        event.accepted = true;
    }

    Rectangle {
        id: menu
        width: menuText.width + 20;
        height: menuText.height + 20;
        x: rawRows.highLightUp.x
        y: rawRows.highLightUp.y
        color: "#88DDDDCC"
        visible: false
        Text {
            id: menuText
            anchors.centerIn: parent
            font.pixelSize: 20
        }
    }

    Component.onCompleted: {
        pDoc.attachPRawRows(pRawRows);
        timer.start(); // workaround for force repaint
    }

    Timer {
        id: timer
        interval: 17 // skip one frame (16ms)
        onTriggered: {
            pDoc.load();
        }
    }
}

