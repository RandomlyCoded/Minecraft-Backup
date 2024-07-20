import QtQuick
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

    ActionSection {
        anchors.verticalCenter: parent.verticalCenter
    }

    StatusSection {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
    }
}
