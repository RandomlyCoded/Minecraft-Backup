import QtQuick
import QtQuick.Controls
import QtQml

import McBackupGUI

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

    StatusSection {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
    }
}
