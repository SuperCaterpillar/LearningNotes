# 通过JavaScript动态创建QML对象

QML支持从JavaScript内部动态创建对象。它还允许根据用户输入或其他事件动态创建可视对象并将其添加到场景中。

## 动态创建对象

有两种方法可以从JavaScript动态创建对象。您可以调用[Qt.createComponent（）](https://doc.qt.io/qt-5/qml-qtqml-qt.html#createComponent-method)动态创建[Component](https://doc.qt.io/qt-5/qml-qtqml-component.html)对象，也可以使用[Qt.createQmlObject（）](https://doc.qt.io/qt-5/qml-qtqml-qt.html#createQmlObject-method)从QML字符串创建对象。

### 动态创建组件

要动态加载QML文件中定义的组件，请在[Qt对象中](https://doc.qt.io/qt-5/qml-qtqml-qt.html)调用[Qt.createComponent（）](https://doc.qt.io/qt-5/qml-qtqml-qt.html#createComponent-method)函数。此函数将QML文件的URL作为其唯一参数，并从该URL 创建[Component](https://doc.qt.io/qt-5/qml-qtqml-component.html)对象

一旦有了[Component](https://doc.qt.io/qt-5/qml-qtqml-component.html)，就可以调用其[createObject（）](https://doc.qt.io/qt-5/qml-qtqml-component.html#createObject-method)方法来创建该组件的实例。此函数可以使用一个或两个参数：

* 第一个是新对象的父对象。父对象可以是图形对象（即[Item](https://doc.qt.io/qt-5/qml-qtquick-item.html)类型）或非图形对象（即[QtObject](https://doc.qt.io/qt-5/qml-qtqml-qtobject.html)或C ++ [QObject](https://doc.qt.io/qt-5/qobject.html)类型）。只有带有图形父对象的图形对象才会呈现到[Qt Quick](https://doc.qt.io/qt-5/qtquick-index.html)可视画布。如果您希望稍后设置父级，则可以安全地传递`null`给该功能。

* 第二个是可选的，是属性-值对的映射，它定义了对象的初始任何属性值。此参数指定的属性值在完成创建对象之前应用到对象，避免了必须初始化特定属性才能启用其他属性绑定时可能发生的绑定错误。此外，与创建对象后定义属性值和绑定相比，性能方面的好处很小。

```js
//Sprite.qml，它定义了一个简单的QML组件：
import QtQuick 2.0

Rectangle { width: 80; height: 50; color: "red" }

//main.qml导入了一个componentCreation.jsJavaScript文件，该文件将创建Sprite对象：
import QtQuick 2.0
import "componentCreation.js" as MyScript

Rectangle {
    id: appWindow
    width: 300; height: 300

    Component.onCompleted: MyScript.createSpriteObjects();
}

//这是componentCreation.js。注意它检查组件是否状态是Component.Ready调用之前的CreateObject（）中的情况下QML文件被加载在网络上且因此立即没有准备好。
var component;
var sprite;

function createSpriteObjects() {
    component = Qt.createComponent("Sprite.qml");
    if (component.status == Component.Ready)
        finishCreation();
    else
        component.statusChanged.connect(finishCreation);
}

function finishCreation() {
    if (component.status == Component.Ready) {
        sprite = component.createObject(appWindow, {x: 100, y: 100});
        if (sprite == null) {
            // Error Handling
            console.log("Error creating object");
        }
    } else if (component.status == Component.Error) {
        // Error Handling
        console.log("Error loading component:", component.errorString());
    }
}
//如果确定要加载的QML文件是本地文件，则可以省略该finishCreation()函数并立即调用createObject（）：
function createSpriteObjects() {
    component = Qt.createComponent("Sprite.qml");
    sprite = component.createObject(appWindow, {x: 100, y: 100});

    if (sprite == null) {
        // Error Handling
        console.log("Error creating object");
    }
}
```

请注意，在这两种情况下，都将[调用createObject（）](https://doc.qt.io/qt-5/qml-qtqml-component.html#createObject-method)并将其`appWindow`作为父参数传递，因为动态创建的对象是可视（Qt Quick）对象。创建的对象将成为中的`appWindow`对象的子代`main.qml`，并出现在场景中。

当使用具有相对路径的文件时，该路径应相对于执行[Qt.createComponent（）](https://doc.qt.io/qt-5/qml-qtqml-qt.html#createComponent-method)的文件。

要将信号连接到动态创建的对象（或从中接收信号），请使用信号`connect()`方法。有关更多信息，请参见[将信号连接到方法和信号](https://doc.qt.io/qt-5/qtqml-syntax-signals.html#connecting-signals-to-methods-and-signals)。

也可以通过[incubateObject（）](https://doc.qt.io/qt-5/qml-qtqml-component.html#incubateObject-method)函数实例化组件而不会阻塞。

### 从QML字符串创建对象

如果直到运行时才定义QML，则可以使用[Qt.createQmlObject（）](https://doc.qt.io/qt-5/qml-qtqml-qt.html#createQmlObject-method)函数从QML字符串中创建QML对象，如以下示例所示：

```js
var newObject = Qt.createQmlObject('import QtQuick 2.0; Rectangle {color: "red"; width: 20; height: 20}',
                                   parentItem,
                                   "dynamicSnippet1");
```

第一个参数是要创建的QML字符串。就像在新文件中一样，您将需要导入想要使用的任何类型。第二个参数是新对象的父对象，适用于组件的父参数语义类似地适用于`createQmlObject()`。第三个参数是与新对象关联的文件路径；这用于错误报告。

如果QML字符串使用相对路径导入文件，则该路径应相对于定义了父对象（该方法的第二个参数）的文件。

**重要：**构建静态QML应用程序时，将扫描QML文件以检测导入依赖性。这样，所有必要的插件和资源都将在编译时解决。但是，仅考虑显式import语句（在QML文件顶部找到的那些），而不考虑用字符串文字括起来的import语句。为了支持静态构建，因此，您需要确保使用[Qt.createQmlObject（）的](https://doc.qt.io/qt-5/qml-qtqml-qt.html#createQmlObject-method) QML文件除了在字符串文字内部之外，还明确地在文件顶部包含所有必需的导入。



## 维护动态创建的对象

在管理动态创建的对象时，必须确保创建上下文的寿命超过创建的对象。否则，如果首先破坏了创建上下文，则动态对象中的绑定和信号处理程序将不再起作用。

实际的创建上下文取决于如何创建对象：

- 如果使用[Qt.createComponent（）](https://doc.qt.io/qt-5/qml-qtqml-qt.html#createComponent-method)，则创建上下文是在其中调用此方法的[QQmlContext](https://doc.qt.io/qt-5/qqmlcontext.html)。
- 如果[调用Qt.createQmlObject（）](https://doc.qt.io/qt-5/qml-qtqml-qt.html#createQmlObject-method)，则创建上下文是传递给此方法的父对象的上下文。
- 如果一个`Component{}`对象被定义和[的CreateObject（）](https://doc.qt.io/qt-5/qml-qtqml-component.html#createObject-method)或[incubateObject（）](https://doc.qt.io/qt-5/qml-qtqml-component.html#incubateObject-method)被调用该对象上，创建上下文是在其中的上下文`Component`被定义

另外，请注意，虽然动态创建的对象可以与其他对象一样使用，但它们在QML中没有ID。

## 动态删除对象

在许多用户界面中，将视觉对象的不透明度设置为0或将视觉对象移出屏幕而不是删除它就足够了。但是，如果有许多动态创建的对象，则删除未使用的对象可能会获得有价值的性能优势。

请注意，永远不要手动删除由便利QML对象工厂动态创建的对象（例如[Loader](https://doc.qt.io/qt-5/qml-qtquick-loader.html)和[Repeater](https://doc.qt.io/qt-5/qml-qtquick-repeater.html)）。另外，应避免删除不是动态创建的对象。

可以使用`destroy()`方法删除项目。此方法具有一个可选参数（默认为0），该参数指定要销毁对象之前的近似延迟（以毫秒为单位）。

这是一个例子。在`application.qml`创建的五个实例`SelfDestroyingRect.qml`组成部分。每个实例都运行一个[NumberAnimation](https://doc.qt.io/qt-5/qml-qtquick-numberanimation.html)，动画结束后，调用`destroy()`其根对象以销毁自身：

```js
//application.qml	

import QtQuick 2.0

Item {
    id: container
    width: 500; height: 100

    Component.onCompleted: {
        var component = Qt.createComponent("SelfDestroyingRect.qml");
        for (var i=0; i<5; i++) {
            var object = component.createObject(container);
            object.x = (object.width + 10) * i;
        }
    }
}
//SelfDestroyingRect.qml	
import QtQuick 2.0

Rectangle {
    id: rect
    width: 80; height: 80
    color: "red"

    NumberAnimation on opacity {
        to: 0
        duration: 1000

        onRunningChanged: {
            if (!running) {
                console.log("Destroying...")
                rect.destroy();
            }
        }
    }
}
```

或者，`application.qml`可以通过调用破坏创建的对象`object.destroy()`。

请注意，在该对象内的对象上调用destroy（）是安全的。不会在调用destroy（）的瞬间销毁对象，而是在该脚本块的末尾与下一帧之间的某个时间清除对象（除非您指定了非零延迟）。

还要注意，如果`SelfDestroyingRect`实例是这样静态创建的：

```js
Item {
    SelfDestroyingRect {
        // ...
    }
}
```

这将导致错误，因为对象只有在动态创建后才能动态销毁。

使用[Qt.createQmlObject（）](https://doc.qt.io/qt-5/qml-qtqml-qt.html#createQmlObject-method)创建的对象可以类似地使用销毁`destroy()`：

```js
var newObject = Qt.createQmlObject('import QtQuick 2.0; Rectangle {color: "red"; width: 20; height: 20}',
                                   parentItem,
                                   "dynamicSnippet1");
newObject.destroy(1000);
```

