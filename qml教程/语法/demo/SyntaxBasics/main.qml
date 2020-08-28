import QtQuick 2.14
import QtQuick.Window 2.14

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("QML Syntax Basics")
    GridView{
      width:parent.width;
      height: parent.height;
      ListModel{
        ListElement {name:"signal"}
      }

      Rectangle{
          color: "red"
          Text {
              id: text
              text: name
          }
      }
    }
}
