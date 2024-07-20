import QtQuick
import QtQuick.Controls

import McBackupGUI

Item {
    width: parent.width
    height: 64

    Rectangle {
        anchors.fill: parent
        color: "green"
        opacity: 0.2
    }

    Button {
        id: backupButton

        x: 16

        text: "start backup"

        onClicked: GuiHandler.startBackup()
        down: GuiHandler.running ? true : undefined
    }

    Button {
        id: restoreButton

        anchors.right: parent.right
        anchors.rightMargin: backupButton.x

        text: "start restore"

        onClicked: GuiHandler.startRestore()

        down: GuiHandler.running ? true : undefined
    }

    Button {
        id: openBackupDirButton

        anchors.left: backupButton.left
        anchors.bottom: parent.bottom

        text: "open backup directory"

        onClicked: GuiHandler.openBackupDirectory()
    }

    Button {
        id: openWorldDirButton

        anchors.right: restoreButton.right
        anchors.bottom: parent.bottom

        text: "open world directory"

        onClicked: GuiHandler.openSavesDirectory()
    }
}
