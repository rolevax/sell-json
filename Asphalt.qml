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

    function insert(str, r, c) {
        var orig = listModel.get(r).modelText;
        var next = [orig.slice(0, c), str, orig.slice(c)].join("");
        listModel.get(r).modelText = next;
    }

    function update(r, bc, ec, str) {
        var orig = listModel.get(r).modelText;
        var next = [orig.slice(0, bc), str, orig.slice(ec)].join("");
        listModel.get(r).modelText = next;
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

    function erase(br, bc, er, ec) {
        if (br === er) { // erase within a row
            var orig = listModel.get(br).modelText;
            listModel.get(br).modelText = orig.slice(0, bc) + orig.slice(ec + 1);
        } else {
            // remove characters in the last row
            listModel.get(er).modelText = listModel.get(er).modelText.slice(ec + 1);

            if (br + 1 < er) // remove middle rows
                listModel.remove(br + 1, (er - 1) - (br + 1) + 1);

            // remove characters in the first row
            listModel.get(br).modelText = listModel.get(br).modelText.slice(0, bc);
        }
    }
}

