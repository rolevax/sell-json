import QtQuick 2.0
import sell 1.0

Rectangle {
    id: frame
    width: 800; height: 500
    color: "#222233"
    focus: true

    PDoc {
        id: pDoc

        onTension: {
            street.setTension(b);
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
            street.insert(str, r, c);
        }

        onErased: {
            street.erase(br, bc, er, ec);
        }

        onLineUpdated: {
            street.update(r, str);
        }

        onNewLine: {
            street.newLine(r, c);
        }

        onJoinLine: {
            street.joinLine(r);
        }

        onHotLightSet: {
            street.setHotLight(b);
        }

        onLighted: {
            street.light(false, lbr, lbc, ler, lec);
            street.light(true, hbr, hbc, her, hec);
        }
    }

    Street {
        id: street
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
        x: street.highLightUp.x
        y: street.highLightUp.y
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

