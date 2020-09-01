&emsp;&emsp;QML提供的[JavaScript主机环境](https://doc.qt.io/qt-5/qtqml-javascript-hostenvironment.html)可以运行有效的标准JavaScript构造，例如条件运算符，数组，变量设置和循环。除了标准的JavaScript属性外，[QML全局对象](https://doc.qt.io/qt-5/qtqml-javascript-qmlglobalobject.html)还包括许多帮助程序方法，这些方法可以简化UI的构建以及与QML环境的交互。

## QML中可以使用JS的情况

&emsp;&emsp;在QML中如下几个地方可以使用js

### 1. [属性绑定](https://doc.qt.io/qt-5/qtqml-syntax-propertybinding.html)时

```js
import QtQuick 2.12

Rectangle {
    id: colorbutton
    width: 200; height: 80;
	
    //常用方法 直接进行绑定
    color: inputHandler.pressed ? "steelblue" : "lightsteelblue"

    TapHandler {
        id: inputHandler
        //不常用方法 使用Qt.binding函数返回一个函数进行绑定
        Component.onCompleted: {
        color = Qt.binding(function() { return inputHandler.pressed ? "steelblue" : "lightsteelblue" });
    }
    }
}
```



###  2. 使用[信号处理程序](https://doc.qt.io/qt-5/qtqml-syntax-objectattributes.html#signal-attributes)时

```js
Rectangle {
    id: button
    width: 200; height: 80; color: "lightsteelblue"

    TapHandler {
        id: inputHandler
        onTapped: {
            // arbitrary JavaScript expression
            console.log("Tapped!")
        }
    }

    Text {
        id: label
        anchors.centerIn: parent
        text: inputHandler.pressed ? "Pressed!" : "Press here!"
    }
}
```



### 3. 自定义方法时

程序逻辑也可以在JavaScript函数中定义。这些函数可以在QML文档中内联定义（作为自定义方法），也可以在导入的JavaScript文件中外部定义。

1. 自定义方法可以在QML文档中定义，并且可以用于处理一个信号，属性绑定或其他QML对象中的函数中调用。这时的函数通常称为内联JavaScript函数，因为它们的实现包含在QML对象类型定义（QML文档）中，而不是包含在外部JavaScript文件中。

   ```js
   Item {
       //注意：在QML文档中内联定义的自定义方法公开给其他对象，因此QML组件中根对象上的内联函数可以由组件外部的调用程序调用。如果不希望这样做，则可以将方法添加到非根对象，或者最好将其写入外部JavaScript文件中。
       function fibonacci(n){
           var arr = [0, 1];
           for (var i = 2; i < n + 1; i++)
               arr.push(arr[i - 2] + arr[i -1]);
   
           return arr;
       }
       TapHandler {
           onTapped: console.log(fibonacci(10))
       }
   }
   ```

2. 最好将程序逻辑分离到一个单独的JavaScript文件中。可以使用import语句将该文件导入QML.例如，可以将先前示例中的fibonacci（）方法移至名为fib.js的外部文件中，并按以下方式访问：

   ```js
   import QtQuick 2.12
   import "fib.js" as MathFunctions
   
   Item {
       TapHandler {
           onTapped: console.log(MathFunctions.fibonacci(10))
       }
   }
   ```

3. 作为槽函数使用

```js
// script.js

function jsFunction() {
    console.log("Called JavaScript function!")
}
//
import QtQuick 2.12
import "script.js" as MyScript

Item {
    id: item
    width: 200; height: 200

    TapHandler {
        id: inputHandler
    }

    Component.onCompleted: {
        inputHandler.tapped.connect(MyScript.jsFunction)
    }
}
```



### 4. 独立的[JavaScript资源（.js）文件](https://doc.qt.io/qt-5/qtqml-javascript-imports.html)

   

##  Component.onCompleted

有时有必要在应用程序（或组件实例）启动时运行一些命令性代码。尽管很容易将启动脚本作为*全局代码包含*在外部脚本文件中，但由于QML环境可能尚未完全建立，因此这可能会有严重的限制。例如，某些对象可能尚未创建，或者某些[属性绑定](https://doc.qt.io/qt-5/qtqml-syntax-propertybinding.html)可能尚未建立。有关全局脚本代码的确切限制，请参见[JavaScript环境限制](https://doc.qt.io/qt-5/qtqml-javascript-hostenvironment.html#javascript-environment-restrictions)。

当QML对象的实例化完成时，它会发出`Component.completed` [附加的信号](https://doc.qt.io/qt-5/qtqml-syntax-signals.html#attached-signal-handlers)。`Component.onCompleted`实例化对象后，将运行相应处理程序中的JavaScript代码。因此，编写应用程序启动代码的最佳位置是在`Component.onCompleted`顶级对象的处理程序中，因为`Component.completed`在完全建立QML环境时，此对象会发出。

```js
import QtQuick 2.0

Rectangle {
    function startupFunction() {
        // ... startup code
    }

    Component.onCompleted: startupFunction();
}
```



QML文件中的任何对象（包括嵌套对象和嵌套QML组件实例）都可以使用此附加属性。如果启动时有多个`onCompleted()`处理程序要执行，则它们将以不确定的顺序依次运行。

同样，每个对象在被销毁之前都会`Component`发出[destroy（）](https://doc.qt.io/qt-5/qml-qtqml-component.html#destruction-signal)信号。