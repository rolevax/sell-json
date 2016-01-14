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
        property bool tension: false
        id: highUp
        lightColor: tension ? "#22AA22" : "#443399"
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

    Asphalt {
        id: asph
        anchors.fill: parent
    }

    function setHotLight(b) {
        hotLight.visible = b;
    }

    function setTension(b) {
        highUp.tension = b;
    }

    function light(high, br, bc, er, ec) {
        var up = high ? highUp : lowUp;
        var down = high ? highDown : lowDown;

        // determine leftmost and rightmost non-space position
        var leftMost = bc;
        var rightMost = ec;
        for (var i = br; i <= er; i++) {
            var row = asph.model.get(i).modelText;
            leftMost = Math.min(leftMost, row.search(/\S/));
            rightMost = Math.max(rightMost, row.length);
        }

        up.leftOff = bc;
        up.rightOff = rightMost;
        up.y = mapFromItem(asph.rowAt(br), 0, 0).y;

        down.leftOff = leftMost;
        down.rightOff = ec;
        down.y = mapFromItem(asph.rowAt(er), 0, 0).y;
    }

    function newLine(r, c) {
        asph.newLine(r, c);
    }

    function joinLine(r) {
        asph.joinLine(r);
    }

    function insert(str, r, c) {
        asph.insert(str, r, c);
    }

    function update(r, bc, ec, str) {
        asph.update(r, bc, ec, str);
    }

    function erase(br, bc, er, ec) {
        asph.erase(br, bc, er, ec);
    }
}

