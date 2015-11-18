import QtQuick 2.0

Item {
    property alias highLightUp: highLightUp

    Rectangle {
        id: lowLightUp
        width: 20; height: 20
        color: "#303040"
    }

    Rectangle {
        id: lowLightDown
        width: 20; height: 20
        color: lowLightUp.color
    }

    Rectangle {
        id: lowLightMiddle
        anchors.top: lowLightUp.bottom
        anchors.bottom: lowLightDown.top
        anchors.left: lowLightDown.left
        anchors.right: lowLightUp.right
        color: lowLightDown.color
    }

    Rectangle {
        id: highLightUp
        width: 20; height: 20
        color: "#443399"
    }

    Rectangle {
        id: highLightDown
        width: 20; height: 20
        color: highLightUp.color
    }

    Rectangle {
        id: highLightMiddle
        anchors.top: highLightUp.bottom
        anchors.bottom: highLightDown.top
        anchors.left: highLightDown.left
        anchors.right: highLightUp.right
        color: highLightDown.color
    }

    Rectangle {
        id: hotLight
        anchors.left: highLightUp.right
        anchors.top: highLightUp.top
        width: 10; height: 20
        color: "#EE3333"
        visible: false
    }

    RawRowsList {
        id: list
        anchors.fill: parent
    }

    function setHotLight(b) {
        hotLight.visible = b;
    }

    function light(high, br, bc, er, ec) {
        var up = high ? highLightUp : lowLightUp;
        var down = high ? highLightDown : lowLightDown;

        var begin = list.tokenAt(br, bc);
        var end = list.tokenAt(er, ec);
        var beginPos = mapFromItem(begin, 0, 0);
        var endPos = mapFromItem(end, end.width, 0);
        var leftMost = beginPos.x;
        var rightMost = endPos.x;
        var i, j;

        // determine leftmost and rightmost non-space position
        for (i = br; i <= er; i++) {
            var columns = list.model.get(i).modelColumns;
            var start = i === br ? bc : 0;
            var limit = i === er ? ec + 1 : columns.count;
            for (j = start; j < limit; j++) {
                var text = columns.get(j).modelText;
                if (!(/^\s*$/.test(text))) {
                    var token = list.tokenAt(i, j);
                    var pos = mapFromItem(token, 0, 0);
                    leftMost = pos.x < leftMost ? pos.x : leftMost;
                    rightMost = pos.x + token.width > rightMost ?
                                pos.x + token.width : rightMost;
                }
            }
        }

        up.x = beginPos.x;
        up.width = Math.max(0, rightMost - up.x);
        up.y = beginPos.y;
        down.x = leftMost;
        down.width = Math.max(0, endPos.x - down.x);
        down.y = endPos.y;
    }

    function newLine(r, c) {
        list.newLine(r, c);
    }

    function insert(token, r, c) {
        list.insert(token, r, c);
    }

    function update(r, c, token) {
        list.update(r, c, token);
    }

    function erase(br, bc, er, ec) {
        list.erase(br, bc, er, ec);
    }
}

