import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2

import QtQuick.Controls 2.0

Item {
    id: trackTab

    property double baseFontPointSize: width * 0.04

    GridLayout {
        anchors.fill: parent
        columns: 2

        Text {
            text: qsTr("TX (KHz)")
            font.bold: true
            font.pixelSize: trackTab.baseFontPointSize
        }

        Text {
            text: (backend.txFrequency / 1000).toLocaleString(Qt.locale("en_US"), "f", 3)
            color: backend.txFrequency === 0 || (backend.downlinkIsSource && backend.txFrequency != backend.txFrequencySetpoint) ? "red" : "green"
            font.pixelSize: trackTab.baseFontPointSize * 2
            Layout.alignment: Qt.AlignRight
        }

        Text {
            text: qsTr("RX (KHz)")
            font.bold: true
            font.pixelSize: trackTab.baseFontPointSize
        }

        Text {
            text: (backend.rxFrequency / 1000).toLocaleString(Qt.locale("en_US"), "f", 3)
            color: backend.rxFrequency === 0 || (!backend.downlinkIsSource && backend.rxFrequency !== backend.rxFrequencySetpoint) ? "red" : "green"
            font.pixelSize: trackTab.baseFontPointSize * 2
            Layout.alignment: Qt.AlignRight
        }

        Text {
            text: qsTr("Offset (Hz)")
            font.bold: true
            font.pixelSize: trackTab.baseFontPointSize
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Button {
                id: largeDecrement
                text: "--"
                font.pixelSize: trackTab.baseFontPointSize * 0.8
                Layout.fillWidth: true
                Layout.preferredHeight: width
                onClicked: backend.incrementOffset(-100)
            }

            Button {
                text: "-"
                font.pixelSize: trackTab.baseFontPointSize * 0.8
                Layout.fillWidth: true
                Layout.preferredHeight: width
                onClicked: backend.incrementOffset(-1)
            }

            TextField {
                Layout.fillWidth: true
                Layout.preferredHeight: largeDecrement.height
                text: backend.offset
                font.pointSize: trackTab.baseFontPointSize
                validator: IntValidator {
                    bottom: -50000
                    top: 50000
                }

                inputMethodHints: Qt.ImhDigitsOnly

                onAccepted: backend.offset = text
            }

            Button {
                text: "+"
                font.pixelSize: trackTab.baseFontPointSize * 0.8
                Layout.fillWidth: true
                Layout.preferredHeight: width
                onClicked: backend.incrementOffset(1)
            }

            Button {
                text: "++"
                font.pixelSize: trackTab.baseFontPointSize * 0.8
                Layout.fillWidth: true
                Layout.preferredHeight: width
                onClicked: backend.incrementOffset(100)
            }

            Button {
                text: "0"
                font.pixelSize: trackTab.baseFontPointSize * 0.8
                Layout.fillWidth: true
                Layout.preferredHeight: width
                onClicked: backend.offset = 0
            }
        }

        CheckBox {
            text: qsTr("Track")
            font.bold: true
            font.pixelSize: 20
            checked: backend.trackingEnabled
            onCheckedChanged: backend.trackingEnabled = checked
        }

        CheckBox {
            text: qsTr("Downlink is source")
            font.bold: true
            font.pixelSize: 20
            checked: backend.downlinkIsSource
            onCheckedChanged: backend.downlinkIsSource = checked
        }

        CheckBox {
            text: qsTr("PTT Mute")
            font.bold: true
            font.pixelSize: 20
            checked: backend.rxAudioMuteEnabled
            onCheckedChanged: backend.rxAudioMuteEnabled = checked
        }
    }
}
