import QtQuick
import QtQuick.Controls
import QtQml

import McBackupGUI

import QtQuick.Controls.Basic.impl

Window {
    title: "Minecraft Backup tool"

    width: 640
    height: 480
    visible: true

    TextInputField {
        id: worldInput

        initialText: GuiHandler.world

        defaultText: "world name"

        onValueUpdated: {
            console.log(text);
            GuiHandler.setWorld(text)
        }
    }

//    Connections {
//        target: GuiHandler

//        onPing: {
//            console.log(GuiHandler.filesProcessed, "/", GuiHandler.filesTotal)
//            console.log(GuiHandler.currentDirectory)
//            console.log(GuiHandler.currentFile)
//        }
//    }

    Button {
        x: 200
        y: 8
        text: "start backup"

        onClicked: GuiHandler.startBackup()

        down: GuiHandler.running ? true : undefined
    }

    Button {
        x: 200
        y: 32
        text: "start restore"

        onClicked: GuiHandler.startRestore()

        down: GuiHandler.running ? true : undefined
    }

    Rectangle {
        id: progress

        width: 150
        height: 16

        radius: 4

        x: 8
        y: 24

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
}
