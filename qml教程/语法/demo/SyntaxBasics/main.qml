import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 1.4

Window {
    id: root
    visible: true
    width: 640
    height: 480
    color: "black"
    title: qsTr("QML Syntax Basics")
    //左边的功能列表
    TreeView{
        width: parent.width/4
        height: parent.height
        headerDelegate:Rectangle {
            color: "red"
            width: 30
            height: 30
        }
        TableViewColumn {
               title: qsTr("Name")
               role: qsTr("fileName")
           }
    }
    //右边的显示窗口

}
