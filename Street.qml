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

    HotLight {
        id: hotLight
        leftOff: highUp.rightOff
        anchors.top: highUp.top
        width: 10; height: 20
        visible: false
    }

    Asphalt {
        id: asph
        anchors.fill: parent
    }

    function setHotLight(back) {
        if (back < 0) {
            hotLight.visible = false;
        } else {
            hotLight.back = back;
            hotLight.visible = true;
        }
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
            if (i !== br)
                leftMost = Math.min(leftMost, row.search(/\S/));
            if (i !== er)
                rightMost = Math.max(rightMost, row.length);
        }

        up.leftOff = bc;
        up.rightOff = rightMost;
        up.y = mapFromItem(asph.rowAt(br), 0, 0).y;

        down.leftOff = leftMost;
        down.rightOff = ec;
        down.y = mapFromItem(asph.rowAt(er), 0, 0).y;
    }

    function updateLine(r, str) {
        asph.updateLine(r, str);
    }

    function newLine(r, c) {
        asph.newLine(r, c);
    }

    function joinLine(r) {
        asph.joinLine(r);
    }

    function removeLine(r, ct) {
        asph.removeLine(r, ct);
    }
}

