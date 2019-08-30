import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2

import QtQuick.Controls 2.0

// For compatibility with Qt 5.7 on Raspbian
import QtQuick.Layouts 1.3 as LayoutsLegacy

Window {
    visible: true
    minimumWidth: 500
    minimumHeight: minimumWidth * 0.47

    title: qsTr("QTrackUplink")

    color: "#f0f0f0"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        TabBar {
            Layout.minimumWidth: parent.width * 0.4
            Layout.maximumHeight: parent.width * 0.05
            id: tabBar

            TabButton {
                text: qsTr("Track")
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
            }

            TabButton {
                text: qsTr("Config")
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        LayoutsLegacy.StackLayout {
            currentIndex: tabBar.currentIndex

            Layout.fillWidth: true
            Layout.fillHeight: true

            // Index 0
            TrackItem {
            }

            // Index 1
            SettingsItem {
            }
        }
    }
}
