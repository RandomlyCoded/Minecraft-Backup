import QtQuick

Rectangle {
    property string defaultText: "insert text here"
    property color backgroundColor: "white"
    property color borderColor: "black"
    property color textColor: "black"
    property int padding: 4
    property int roundness: 2 // I have to name it roundness because otherwise it might conflict with the Rectangle's property

    property int minimumWidth: 256

    property string text: input.text
    property string initialText: ""

    onInitialTextChanged: input.text = initialText

    signal valueUpdated()

    radius: roundness

    width: Math.max(input.width, minimumWidth)
    height: input.height

    color: backgroundColor
    border.color: borderColor

    clip: true

    TextInput {
        id: input

        color: parent.textColor

        width: parent.width

        padding: parent.padding

        onAccepted: valueUpdated()
    }

    MouseArea {
        anchors.fill: parent

        cursorShape: Qt.IBeamCursor

        onClicked: input.focus = true
    }

    Text {
        id: tipText

        x: 4
        y: 4

        // I didn't find any better way to modify colors in QML and didn't want to export it into C++ and back
        function makeColor() {
            const alpha = 128
            let red = borderColor.red

            let hexColor = borderColor.toString().substring(1, 9)

            // assuming the color has an alpha value
            if(hexColor.length !== 6)
                hexColor = hexColor.substring(2, 7)

            return "#aa" + hexColor
        }

        color: makeColor()

        font: input.font

        visible: maybeShow()

        function maybeShow() {
            let text = input.text;
            text = text.replace(/ /g, "");

            return text === ""
        }

        text: defaultText
    }
}
