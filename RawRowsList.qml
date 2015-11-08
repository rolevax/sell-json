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

    function newLine(r, c) {
        listModel.insert(r + 1, { modelColumns: [] });
        var prevRow = listModel.get(r).modelColumns;
        for (var i = c; i < prevRow.count; i++) {
            var token = prevRow.get(i);
            listModel.get(r + 1).modelColumns.insert(i - c, token);
        }
        prevRow.remove(c, prevRow.count - c);
    }

    function removeRow() {
        listModel.remove(row, 1);
    }

    function removeCol() {
        listModel.get(row).modelColumns.remove(col, 1);
    }
}

