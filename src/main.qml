import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15 // ColumnLayout
import QtQuick.Controls 2.15 // Buttons
import com.cppeverything.devicemonitor 1.0

Window {
    width: 200
    height: 480
    visible: true
    title: qsTr("Damn")

    ColumnLayout {
        spacing: 5
        anchors.fill: parent

        Switch {
            text: qsTr("Available Only")
            Layout.alignment: Qt.AlignRight

            onCheckedChanged: {
                Controller.filter_model.filter_status = checked ? DeviceData.Online : DeviceData.Undefined
                Controller.filter_model.filter_active = checked
            }
        }

        ListView {
            id: listView
            spacing: 3
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: Controller.filter_model

            delegate: DeviceDelegate {
                deviceName: model.name
                deviceState: model.status
                connectedUser: model.user
                height: 50
                width: listView.width
            }

            Text {
                text: qsTr("No device available")
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignTop
                visible: parent.count == 0
                anchors.fill: parent
            }
        }
    }


}
