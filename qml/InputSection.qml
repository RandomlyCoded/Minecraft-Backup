import QtQuick

import McBackupGUI

Item {
    width: parent.width

    height: 64

    TextInputField {
        id: directoryInput

        x: 8

        initialText: GuiHandler.directory

        defaultText: "saves directory"

        onTextChanged: checkDirName()

        function checkDirName() {
            if(GuiHandler.doesDirectoryExist(text)) {
                GuiHandler.directory = text
                borderColor = "green"
            }
            else
                borderColor = "red"
        }

        width: parent.width - 16
    }

    TextInputField {
        id: worldInput

        anchors.left: directoryInput.left

        anchors.top: directoryInput.bottom
        anchors.topMargin: 8

        initialText: GuiHandler.world

        defaultText: "world name"

        onTextChanged: checkWorldName()

        function checkWorldName() {
            if(GuiHandler.doesDirectoryExist(text)) {
                GuiHandler.world = text
                currentWorldView.update()
                borderColor = "green"
            }
            else
                borderColor = "red"
        }

        // initial update so the initial world name is shown
        Component.onCompleted: checkWorldName()
    }


    Text {
        id: currentWorldView
        anchors.top: worldInput.bottom
        anchors.topMargin: 8

        x: 8

        function update() {
            text = "current world: <b>" + worldInput.text + "</b>"

            // update the world icon preview here to not add another oneliner
            worldIconPreview.source = "image://worldIcon/" + worldInput.text
        }
    }

    Image {
        id: worldIconPreview
        source: "image://worldIcon/" + GuiHandler.world

        anchors.left: worldInput.right
        anchors.leftMargin: 8

        anchors.top: worldInput.top
    }
}
