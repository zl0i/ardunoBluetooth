import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.2
import ModuleArduino 1.0
import QtQuick.Window 2.2

Window {
    visible: true
    width: 320
    height: 480
    title: qsTr("Hello World")
    ArduinoBl {
          id: _ard
    }
    Item {
        id: _sensrect
        x:0
        y:0
        width: parent.width
        height: 3*parent.height/4
        Image {
            anchors.top: parent.top
            width: parent.width/2
            height: 2*parent.height/3
            id: _img
            source: "qrc:/offled.png"
        }
        ColumnLayout {
            id: _clonoff
            anchors.top: _img.bottom
            width: parent.width/3
            Button {
                text: "On"
                enabled: _ard.isHC05
                onClicked:{                    
                        _img.source = "qrc:/onled.png"
                        _ard.onHC05()
                }
            }
            Button {
                text: "Off"
                enabled: _ard.isHC05
                onClicked: {
                     _img.source = "qrc:/offled.png"
                    _ard.offHC05()
                }
            }
       }
       /*ProgressBar {
           id: _pbtemp
           x: _img.width + 10
           y: 120
           //height: parent.width/10
           anchors.margins: 10
           width: 150
           rotation: -90
           value: 0.5
       }
       ProgressBar {
           id: _pbpress
           x: _img.width + 50
           y: 120
           //height: parent.width/10
           anchors.margins: 10
           width: 150
           rotation: -90
           value: 0.5
       }*/
       ColumnLayout {
           anchors.top: _clonoff.top
           anchors.left: _clonoff.right
           anchors.margins: 20
           Label {

               text: "temperature = " + _ard.temperature
           }
           /*Label {

               text: "preassure = " + _ard.pressure
           }*/
        }
    }

    Item {
        id: _blrect
        anchors.top: _sensrect.bottom
        width: parent.width
        height: parent.height - _sensrect.height
        Label {
            id: _lbl
            anchors.top: parent.top
            width: parent.width
            height: parent.height/5
            //anchors.margins: 2
            text: _ard.msg
        }
        ListView {
            id: _view
            anchors.top: _lbl.bottom
            width: parent.width
            height: parent.height/2
            model: _ard.listDevice_model
            delegate: Text {
                text: display
                //font.pixelSize: 20
            }
        }
        RowLayout {
            id: _rowlb
            anchors.bottom: parent.bottom
            anchors.top: _view.bottom
            width: parent.width
            Button {
                text: "Find"
                onClicked: _ard.startSearh()
            }
            Button {
                enabled: _ard.isHC05
                text: "Connect HC05"
                onClicked: _ard.connectHC05()
            }
            Button {
                anchors.right: parent.right
                text: "Exit"
                onClicked: Qt.quit()
            }
        }
    }
}
