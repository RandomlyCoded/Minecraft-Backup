import QtQuick
import QtQml

import McBackupGUI

Window {
    title: "Minecraft Backup tool"

    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480
    visible: true

    InputSection {
        anchors.top: parent.top
        anchors.topMargin: 8
    }

    ActionSection {
        anchors.verticalCenter: parent.verticalCenter
    }

    StatusSection {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
    }
}
