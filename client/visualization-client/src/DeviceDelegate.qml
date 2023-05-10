import QtQuick 2.12
//import QtCharts 2.0
import QtQuick.Window 2.12
import com.cppeverything.devicemonitor 1.0

Rectangle {
    id: root

    readonly property double spaceBetweenItems : width / 20

    property string deviceName
    property string deviceState
    property string connectedUser

    radius: 20
    border.color: "grey"
    color: "lightgrey"

    states: [
        State {
            name: "Unknown"
            when: deviceState == DeviceData.Undefined
            PropertyChanges {
                target: statusText
                text: "unreachable"
            }
            PropertyChanges {
                target: statusCircle
                color: "grey"
                border.color: "transparent"
            }
        },
        State {
            name: "Connecting"
            when: deviceState == DeviceData.Connecting
            PropertyChanges {
                target: statusText
                text: "Connecting..."
            }
            PropertyChanges {
                target: statusCircle
                color: "grey"
                border.color: "transparent"
            }
            PropertyChanges {
                target: connectionAnimation
                running: true
            }
        },
        State {
            name: "Busy"
            when: deviceState == DeviceData.Online && connectedUser !== ""
            PropertyChanges {
                target: statusText
                text: "In use by " + root.connectedUser
            }
            PropertyChanges {
                target: statusCircle
                color: "yellow"
                border.color: "grey"
            }
        },
        State {
            name: "Available"
            when: deviceState == DeviceData.Online && connectedUser === ""
            PropertyChanges {
                target: statusText
                text: "Available"
            }
            PropertyChanges {
                target: statusCircle
                color: "green"
                border.color: "transparent"
            }
        }
    ]

    Rectangle {
        id: statusCircle
        radius: width / 2
        width: height
        height: 0.8 * parent.height
        border.width: 1

        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
            leftMargin: root.spaceBetweenItems
        }

        SequentialAnimation{
            id: connectionAnimation
            PropertyAnimation {
                target: statusCircle
                property: "color"
                duration: 1000
                from: "lightgrey"
                to: "green"
            }
            PropertyAnimation{
                target: statusCircle
                property: "color"
                duration: 1000
                from: "green"
                to: "lightgrey"
            }
            running: false
            loops: Animation.Infinite
        }
    }

    Text {
        id: deviceIdText
        text: deviceName

        horizontalAlignment: Text.AlignLeft

        anchors {
            top: parent.top
            bottom: parent.verticalCenter
            left: statusCircle.right
            leftMargin: root.spaceBetweenItems
            right: parent.right
            rightMargin: root.spaceBetweenItems
        }
    }

    Text {
        id: statusText
//        text: connectedUser !== "" ? "In use by " + connectedUser
//                          : deviceState == DeviceData.Online ? "available"
//                                                : "offline"
        horizontalAlignment: Text.AlignLeft

        anchors {
            top: deviceIdText.bottom
            bottom: parent.bottom
            left: deviceIdText.left
            right: deviceIdText.right
            rightMargin: root.spaceBetweenItems
        }
    }
}
