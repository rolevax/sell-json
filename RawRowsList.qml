import QtQuick 2.5

ListView {
    property int row: 0
    property int col: 0

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

    function move(nextRow, nextCol) {
        var maxRow = listModel.count;
        row = nextRow > maxRow ? maxRow : (nextRow < 0 ? 0 : nextRow);

        var maxCol = row === maxRow ? 0
                                    : listModel.get(row).modelColumns.count;
        col = nextCol > maxCol ? maxCol : (nextCol < 0 ? 0 : nextCol);
    }

    function insert(token) {
        listModel.get(row).modelColumns.insert(col, token);
    }

    function newRow() {
        listModel.insert(row, { modelColumns: [] });
    }

    function removeRow() {
        listModel.remove(row, 1);
    }

    function removeCol() {
        listModel.get(row).modelColumns.remove(col, 1);
    }
}

