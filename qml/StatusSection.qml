import QtQuick

import McBackupGUI

Item {
    Rectangle {
        anchors.fill: parent
        opacity: 0.2
        color: "red"
    }

    width: parent.width

    height: filesProcessed.y + filesProcessed.height

    Rectangle {
        id: progressBar

        width: parent.width - 16
        height: 16

        radius: 4

        x: 8

        anchors.top: fileNameView.bottom
        anchors.topMargin: 8

        color: "#cc0000"
        border.color: "black"
        border.width: 1

        Rectangle {
            id: bar

            radius: parent.radius

            border.color: "#2d2d2d"
            border.width: parent.border.width

            width: parent.width * (GuiHandler.filesProcessed / GuiHandler.filesTotal)
            height: parent.height

            color: "#00cc00"
        }
    }

    // file counting
    Text {
        id: filesProcessed
        anchors.top: progressBar.bottom
        anchors.topMargin: 4

        anchors.left: progressBar.left

        text: GuiHandler.filesProcessed.toString();

        visible: GuiHandler.running

        width: 32

        horizontalAlignment: Text.AlignRight
    }

    Text {
        id: fileSeparator
        anchors.top: filesProcessed.top

        anchors.left: filesProcessed.right

        text: " / ";

        visible: GuiHandler.running
    }

    Text {
        id: filesTotal
        anchors.top: filesProcessed.top

        anchors.left: fileSeparator.right

        text: GuiHandler.filesTotal.toString();

        visible: GuiHandler.running
    }

    // file name display
    Text {
        id: directoryNameView
        anchors.left: progressBar.left

        text: GuiHandler.currentDirectory
    }

    Text {
        id: fileNameView
        anchors.left: progressBar.left

        anchors.top: directoryNameView.bottom
        anchors.topMargin: 8

        text: GuiHandler.currentFile
    }
}
