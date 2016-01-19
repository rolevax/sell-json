import QtQuick 2.0

Item {
    property int leftOff
    property int back

    height: entity.height
    width: rightText.width

    Text {
        id: leftText
        color: hotLight.color
        font.family: "Monospace"
        font.pixelSize: 20
    }

    Rectangle {
        id: hotLight
        anchors.left: leftText.right
        anchors.bottom: leftText.bottom
        width: 10;
        height: 20
        color: "#EE3333"
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
        leftText.text = repeat(' ', leftOff - back);
    }

    onBackChanged: {
        leftText.text = repeat(' ', leftOff - back);
    }
}
