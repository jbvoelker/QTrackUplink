import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2

import QtQuick.Controls 2.0

// For compatibility with Qt 5.7 on Raspbian
import QtQuick.Controls 1.4 as ControlsLegacy

Item {
    ControlsLegacy.ScrollView {
        anchors.fill: parent
        id: scrollView

        clip: true

        onVisibleChanged: {
            if (!visible) {
                txStartFrequency.text = backend.txStartFrequency
                rxStartFrequency.text = backend.rxStartFrequency
                txModel.text = backend.txModel
                txPort.text = backend.txPort
                rxModel.text = backend.rxModel
                rxPort.text = backend.rxPort
                onPttCommand.text = backend.onPttCommand
                onReturnToRxCommand.text = backend.onReturnToRxCommand
            }
        }

        ColumnLayout {

            GridLayout {
                columns: 4

                Text {
                    text: qsTr("TX (Hz)")
                    font.bold: true
                }

                TextField {
                    id: txStartFrequency
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.columnSpan: 3
                    text: backend.txStartFrequency
                    color: text === backend.txStartFrequency.toString() ? "black" : "darkgrey"
                    validator: DoubleValidator {
                        bottom: 0
                    }

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Text {
                    text: qsTr("RX (Hz)")
                    font.bold: true
                }

                TextField {
                    id: rxStartFrequency
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.columnSpan: 3
                    text: backend.rxStartFrequency
                    color: text === backend.rxStartFrequency.toString() ? "black" : "darkgrey"
                    validator: DoubleValidator {
                        bottom: 0
                    }

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Text {
                    text: qsTr("TX Model")
                    font.bold: true
                }

                TextField {
                    id: txModel
                    Layout.fillHeight: true
                    Layout.preferredWidth: font.pixelSize * 3
                    text: backend.txModel
                    color: text === backend.txModel.toString() ? "black" : "darkgrey"
                    validator: IntValidator {
                        bottom: 0
                    }

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Text {
                    text: qsTr("Port")
                    font.bold: true
                }

                TextField {
                    id: txPort
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: backend.txPort
                    color: text === backend.txPort ? "black" : "darkgrey"
                }

                Text {
                    text: qsTr("RX Model")
                    font.bold: true
                }

                TextField {
                    id: rxModel
                    Layout.fillHeight: true
                    Layout.preferredWidth: font.pixelSize * 3
                    text: backend.rxModel
                    color: text === backend.rxModel.toString() ? "black" : "darkgrey"

                    validator: IntValidator {
                        bottom: 0
                    }
                }

                Text {
                    text: qsTr("Port")
                    font.bold: true
                }

                TextField {
                    id: rxPort
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: backend.rxPort
                    color: text === backend.rxPort ? "black" : "darkgrey"
                }

                Text {
                    text: qsTr("On PTT")
                    font.bold: true
                }

                TextField {
                    id: onPttCommand
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.columnSpan: 3
                    text: backend.onPttCommand
                    color: text === backend.onPttCommand ? "black" : "darkgrey"
                }

                Text {
                    text: qsTr("On back\nto RX")
                    font.bold: true
                }

                TextField {
                    id: onReturnToRxCommand
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.columnSpan: 3
                    text: backend.onReturnToRxCommand
                    color: text === backend.onReturnToRxCommand ? "black" : "darkgrey"
                }
            }

            Button {
                text: qsTr("Apply")
                font.bold: true

                Layout.fillHeight: true
                Layout.fillWidth: true

                onClicked: {
                    backend.txStartFrequency = txStartFrequency.text
                    backend.rxStartFrequency = rxStartFrequency.text
                    backend.txModel = txModel.text
                    backend.txPort = txPort.text
                    backend.rxModel = rxModel.text
                    backend.rxPort = rxPort.text
                    backend.onReturnToRxCommand = onReturnToRxCommand.text
                    backend.onPttCommand = onPttCommand.text
                    backend.init()
                }
            }
        }
    }
}
