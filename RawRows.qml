import QtQuick 2.0

Item {
    property alias highLightUp: highUp

    LightBar {
        id: lowUp
        lightColor: "#303040"
    }

    LightBar {
        id: lowDown
        lightColor: lowUp.lightColor
    }

    Rectangle {
        id: lowMiddle
        anchors.top: lowUp.bottom
        anchors.bottom: lowDown.top
        anchors.left: lowDown.left
        anchors.leftMargin: lowDown.leftText.width
        anchors.right: lowUp.right
        color: lowDown.lightColor
    }

    LightBar {
        id: highUp
        lightColor: "#443399"
    }

    LightBar {
        id: highDown
        lightColor: highUp.lightColor
    }

    Rectangle {
        id: highMiddle
        anchors.top: highUp.bottom
        anchors.bottom: highDown.top
        anchors.left: highDown.left
        anchors.leftMargin: highDown.leftText.width
        anchors.right: highUp.right
        color: highDown.lightColor
    }

    Rectangle {
        id: hotLight
        anchors.left: highUp.right
        anchors.top: highUp.top
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
        var up = high ? highUp : lowUp;
        var down = high ? highDown : lowDown;

        // determine leftmost and rightmost non-space position
        var leftMost = bc;
        var rightMost = ec;
        for (var i = br; i <= er; i++) {
            var row = list.model.get(i).modelText;
            leftMost = Math.min(leftMost, row.search(/\S/));
            rightMost = Math.max(rightMost, row.length);
        }

        up.leftOff = bc;
        up.rightOff = rightMost;
        up.y = mapFromItem(list.rowAt(br), 0, 0).y;

        down.leftOff = leftMost;
        down.rightOff = ec;
        down.y = mapFromItem(list.rowAt(er), 0, 0).y;
    }

    function newLine(r, c) {
        list.newLine(r, c);
    }

    function joinLine(r) {
        list.joinLine(r);
    }

    function insert(str, r, c) {
        list.insert(str, r, c);
    }

    function update(r, bc, ec, str) {
        list.update(r, bc, ec, str);
    }

    function erase(br, bc, er, ec) {
        list.erase(br, bc, er, ec);
    }
}

