import QtQuick 2.5

ListView {
    model: ListModel {
        id: listModel
        ListElement {
            modelText: ""
        }
    }

    delegate: Text {
        property int rowId: index

        objectName: "delegateItem"
        width: parent.width
        text: modelText
        color: "white"
        font.family: "Monospace"
        font.pixelSize: 20
    }

    function rowAt(id) {
        forceLayout();
        for (var i = 0; i < contentItem.children.length; i++) {
            var item = contentItem.children[i];
            if (item.objectName === "delegateItem" &&
                    item.rowId === id)
                return item;
        }
        return undefined;
    }

    function updateLine(r, str) {
        listModel.get(r).modelText = str;
    }

    function newLine(r, c) {
        var prevRow = listModel.get(r).modelText;
        listModel.insert(r + 1, { modelText: prevRow.slice(c) });
        listModel.get(r).modelText = prevRow.slice(0, c);
    }

    function joinLine(r) {
        listModel.get(r - 1).modelText += listModel.get(r).modelText;
        listModel.remove(r, 1);
    }

    function removeLine(r, ct) {
        listModel.remove(r, ct);
    }
}

