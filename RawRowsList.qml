import QtQuick 2.5

ListView {
    model: ListModel {
        id: listModel
        ListElement {
            modelColumns: []
        }
    }

    delegate: ListView {
        property int rowId: index

        objectName: "delegateItem"
        width: parent.width
        height: 20
        orientation: Qt.Horizontal
        model: modelColumns
        delegate: Item {
            property int colId: index

            objectName: "delegateItem"
            width: token.width; height: token.height

            Text {
                id: token
                text: modelText
                color: "white"
                font.family: "Monospace"
                font.pixelSize: 20
            }
        }

        function delegateAt(id) {
            forceLayout();
            for (var i = 0; i < contentItem.children.length; i++) {
                var item = contentItem.children[i];
                if (item.objectName === "delegateItem" &&
                        item.colId === id)
                    return item;
            }
            console.log("row.delegateAt: illegal id: " + id);
            return undefined;
        }
    }

    function delegateAt(id) {
        forceLayout();
        for (var i = 0; i < contentItem.children.length; i++) {
            var item = contentItem.children[i];
            if (item.objectName === "delegateItem" &&
                    item.rowId === id)
                return item;
        }
        return undefined;
    }

    function tokenAt(rowId, colId) {
        var row = delegateAt(rowId)
        var token = row.delegateAt(colId);
        if (!token)
            console.log("tokenAt: illegal index " + rowId + ", " + colId);
        return token;
    }

    function insert(token, r, c) {
        listModel.get(r).modelColumns.insert(c, token);
    }

    function update(r, c, token) {
        listModel.get(r).modelColumns.set(c, token);
    }

    function newLine(r, c) {
        listModel.insert(r + 1, { modelColumns: [] });
        var prevRow = listModel.get(r).modelColumns;
        for (var i = c; i < prevRow.count; i++) {
            var token = prevRow.get(i);
            listModel.get(r + 1).modelColumns.insert(i - c, token);
        }
        if (c !== prevRow.count)
            prevRow.remove(c, prevRow.count - c);
    }

    function mergeLine(r) {
        var prevLine = listModel.get(r - 1).modelColumns;
        var currLine = listModel.get(r).modelColumns;
        for (var i = 0; i < currLine.count; i++)
            prevLine.append(currLine.get(i));
        listModel.remove(r, 1);
    }

    function erase(br, bc, er, ec) {
        if (br === er) {
            listModel.get(br).modelColumns.remove(bc, ec - bc + 1);
            if (listModel.get(br).modelColumns.count === 0)
                listModel.remove(br, 1);
        } else {
            // remove tokens in the last row
            listModel.get(er).modelColumns.remove(0, ec + 1);

            if (br + 1 < er) // remove tokens in the middle rows
                listModel.remove(br + 1, (er - 1) - (br + 1) + 1);

            // remove tokens in the first row
            var ct = listModel.get(br).modelColumns.count;
            listModel.get(br).modelColumns.remove(bc, ct - bc);

            if (br + 1 < listModel.count)
                mergeLine(br + 1);
        }
    }
}

