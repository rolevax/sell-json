import QtQuick 2.0

Item {
    property int leftOff
    property int rightOff
    property string lightColor
    property alias entity: entity
    property alias leftText: leftText

    height: entity.height
    width: rightText.width

    Text {
        id: leftText
        color: lightColor
        font.family: "Monospace"
        font.pixelSize: 20
    }

    Text {
        id: rightText
        color: lightColor
        font.family: "Monospace"
        font.pixelSize: 20
    }

    Rectangle {
        id: entity
        width: 20; height: 20
        color: lightColor
        anchors.left: leftText.right
        anchors.right: rightText.right
        anchors.top: leftText.top
        anchors.bottom: leftText.bottom
    }

    function repeat(pattern, count) {
        if (count < 1) return '';
        var result = '';
        while (count > 1) {
            if (count & 1) result += pattern;
            count >>= 1, pattern += pattern;
        }
        return result + pattern;
    }

    onLeftOffChanged: {
        leftText.text = repeat(' ', leftOff);
    }

    onRightOffChanged: {
        rightText.text = repeat(' ', rightOff);
    }
}
