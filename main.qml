import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.1

Window {
    visible: true
    width: 400
    height: 500
//    flags: Qt.SplashScreen
    title: qsTr("Hello World")

Column {

    Row {
        Button {
            text: "start connection"
            onClicked: Modbus.startConnection()
        }
        Button {
            text: "end connection"
            onClicked: Modbus.endConnection()
        }
        Button {
            text: "read"
            onClicked: Modbus.startReading()
        }
    }

    Row {
        Label {
            text: "start address"
        }
        TextField {
            onEditingFinished: Modbus.setStartAddress(text)
        }
    }

    Row {
        Label {
            text: "n bytes"
        }
        TextField {
            onEditingFinished: Modbus.setNnbytes(text);
        }
    }

}



}
